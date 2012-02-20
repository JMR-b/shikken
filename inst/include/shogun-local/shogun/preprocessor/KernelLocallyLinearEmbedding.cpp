/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2011 Sergey Lisitsyn
 * Copyright (C) 2011 Berlin Institute of Technology and Max-Planck-Society
 */

#include <shogun/preprocessor/KernelLocallyLinearEmbedding.h>
#ifdef HAVE_LAPACK
#include <shogun/mathematics/arpack.h>
#include <shogun/mathematics/lapack.h>
#include <shogun/lib/FibonacciHeap.h>
#include <shogun/lib/common.h>
#include <shogun/mathematics/Math.h>
#include <shogun/io/SGIO.h>
#include <shogun/lib/Signal.h>

#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif

using namespace shogun;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
struct LK_RECONSTRUCTION_THREAD_PARAM
{
	/// starting index of loop
	int32_t idx_start;
	/// end loop index
	int32_t idx_stop;
	/// step of loop
	int32_t idx_step;
	/// number of neighbors
	int32_t m_k;
	/// number of vectors
	int32_t N;
	/// matrix containing indexes of ith neighbors of jth vector in ith column
	const int32_t* neighborhood_matrix;
	/// local gram matrix 
	float64_t* local_gram_matrix;
	/// gram matrix
	const float64_t* kernel_matrix;
	/// vector used for solving equation 
	float64_t* id_vector;
	/// weight matrix
	float64_t* W_matrix;
};

struct K_NEIGHBORHOOD_THREAD_PARAM
{
	int32_t idx_start;
	int32_t idx_step;
	int32_t idx_stop;
	int32_t N;
	int32_t m_k;
	CFibonacciHeap* heap;
	const float64_t* kernel_matrix;
	int32_t* neighborhood_matrix;
};
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

CKernelLocallyLinearEmbedding::CKernelLocallyLinearEmbedding() :
		CLocallyLinearEmbedding()
{
	init();
}

CKernelLocallyLinearEmbedding::CKernelLocallyLinearEmbedding(CKernel* kernel)
{
	init();
	SG_REF(kernel);
	m_kernel = kernel;
}

void CKernelLocallyLinearEmbedding::init()
{
	m_kernel = NULL;
}

CKernelLocallyLinearEmbedding::~CKernelLocallyLinearEmbedding()
{
	SG_UNREF(m_kernel);
}

bool CKernelLocallyLinearEmbedding::init(CFeatures* features)
{
	return true;
}

void CKernelLocallyLinearEmbedding::cleanup()
{
}

SGMatrix<float64_t> CKernelLocallyLinearEmbedding::apply_to_feature_matrix(CFeatures* features)
{
	ASSERT(features);
	ASSERT(m_kernel);
	SG_REF(features);

	// get dimensionality and number of vectors of data
	int32_t N = features->get_num_vectors();
	if (m_k>=N)
		SG_ERROR("Number of neighbors (%d) should be less than number of objects (%d).\n",
		         m_k, N);

	// loop variables
	int32_t i,j,t;

	// compute kernel matrix
	m_kernel->init(features,features);
	Parallel* kernel_parallel = m_kernel->parallel;
	m_kernel->parallel = this->parallel;
	SGMatrix<float64_t> kernel_matrix = m_kernel->get_kernel_matrix();
	SGMatrix<int32_t> neighborhood_matrix = get_neighborhood_matrix(kernel_matrix);
	m_kernel->parallel = kernel_parallel;
	m_kernel->cleanup();

	// init W (weight) matrix
	float64_t* W_matrix = SG_CALLOC(float64_t, N*N);

#ifdef HAVE_PTHREAD
	int32_t num_threads = parallel->get_num_threads();
	ASSERT(num_threads>0);
	// allocate threads
	pthread_t* threads = SG_MALLOC(pthread_t, num_threads);
	LK_RECONSTRUCTION_THREAD_PARAM* parameters = SG_MALLOC(LK_RECONSTRUCTION_THREAD_PARAM, num_threads);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
#else
	int32_t num_threads = 1;
#endif 
	// init matrices and norm factor to be used
	float64_t* local_gram_matrix = SG_MALLOC(float64_t, m_k*m_k*num_threads);
	float64_t* id_vector = SG_MALLOC(float64_t, m_k*num_threads);

#ifdef HAVE_PTHREAD
	for (t=0; t<num_threads; t++)
	{
		parameters[t].idx_start = t;
		parameters[t].idx_step = num_threads;
		parameters[t].idx_stop = N;
		parameters[t].m_k = m_k;
		parameters[t].N = N;
		parameters[t].neighborhood_matrix = neighborhood_matrix.matrix;
		parameters[t].kernel_matrix = kernel_matrix.matrix;
		parameters[t].local_gram_matrix = local_gram_matrix+(m_k*m_k)*t;
		parameters[t].id_vector = id_vector+m_k*t;
		parameters[t].W_matrix = W_matrix;
		pthread_create(&threads[t], &attr, run_linearreconstruction_thread, (void*)&parameters[t]);
	}
	for (t=0; t<num_threads; t++)
		pthread_join(threads[t], NULL);
	pthread_attr_destroy(&attr);
	SG_FREE(parameters);
	SG_FREE(threads);
#else
	LK_RECONSTRUCTION_THREAD_PARAM single_thread_param;
	single_thread_param.idx_start = 0;
	single_thread_param.idx_step = 1;
	single_thread_param.idx_stop = N;
	single_thread_param.m_k = m_k;
	single_thread_param.N = N;
	single_thread_param.neighborhood_matrix = neighborhood_matrix.matrix;
	single_thread_param.local_gram_matrix = local_gram_matrix;
	single_thread_param.kernel_matrix = kernel_matrix.matrix;
	single_thread_param.id_vector = id_vector;
	single_thread_param.W_matrix = W_matrix;
	run_linearreconstruction_thread((void*)single_thread_param);
#endif

	// clean
	SG_FREE(id_vector);
	neighborhood_matrix.destroy_matrix();
	kernel_matrix.destroy_matrix();
	SG_FREE(local_gram_matrix);

	// W=I-W
	for (i=0; i<N; i++)
	{
		for (j=0; j<N; j++)
			W_matrix[j*N+i] = (i==j) ? 1.0-W_matrix[j*N+i] : -W_matrix[j*N+i];
	}

	// compute M=(W-I)'*(W-I)
	SGMatrix<float64_t> M_matrix(N,N);
	cblas_dgemm(CblasColMajor,CblasTrans, CblasNoTrans,
	            N,N,N,
	            1.0,W_matrix,N,
	            W_matrix,N,
	            0.0,M_matrix.matrix,N);

	SG_FREE(W_matrix);

	SGMatrix<float64_t> nullspace = find_null_space(M_matrix,m_target_dim,false);

	if ((features->get_feature_class()==C_SIMPLE) &&
	    (features->get_feature_type()==F_DREAL))
	{
		((CSimpleFeatures<float64_t>*)features)->set_feature_matrix(nullspace);
		M_matrix.destroy_matrix();
		SG_UNREF(features);
		return ((CSimpleFeatures<float64_t>*)features)->get_feature_matrix();
	}
	else
	{
		SG_UNREF(features);
		SG_WARNING("Can't set feature matrix, returning feature matrix.\n");
		return nullspace;
	}
}

SGVector<float64_t> CKernelLocallyLinearEmbedding::apply_to_feature_vector(SGVector<float64_t> vector)
{
	SG_NOTIMPLEMENTED;
	return vector;
}

void* CKernelLocallyLinearEmbedding::run_linearreconstruction_thread(void* p)
{
	LK_RECONSTRUCTION_THREAD_PARAM* parameters = (LK_RECONSTRUCTION_THREAD_PARAM*)p;
	int32_t idx_start = parameters->idx_start;
	int32_t idx_step = parameters->idx_step;
	int32_t idx_stop = parameters->idx_stop;
	int32_t m_k = parameters->m_k;
	int32_t N = parameters->N;
	const int32_t* neighborhood_matrix = parameters->neighborhood_matrix;
	float64_t* local_gram_matrix = parameters->local_gram_matrix;
	const float64_t* kernel_matrix = parameters->kernel_matrix;
	float64_t* id_vector = parameters->id_vector;
	float64_t* W_matrix = parameters->W_matrix;

	int32_t i,j,k;
	float64_t norming,trace;

	for (i=idx_start; i<idx_stop; i+=idx_step)
	{
		for (j=0; j<m_k; j++)
		{
			for (k=0; k<m_k; k++)
				local_gram_matrix[j*m_k+k] = 
					kernel_matrix[i*N+i] -
					kernel_matrix[i*N+neighborhood_matrix[j*N+i]] -
					kernel_matrix[i*N+neighborhood_matrix[k*N+i]] +
					kernel_matrix[neighborhood_matrix[j*N+i]*N+neighborhood_matrix[k*N+i]];
		}

		for (j=0; j<m_k; j++)
			id_vector[j] = 1.0;

		// compute tr(C)
		trace = 0.0;
		for (j=0; j<m_k; j++)
			trace += local_gram_matrix[j*m_k+j];
		
		// regularize gram matrix
		for (j=0; j<m_k; j++)
			local_gram_matrix[j*m_k+j] += 1e-3*trace/m_k;

		clapack_dposv(CblasColMajor,CblasLower,m_k,1,local_gram_matrix,m_k,id_vector,m_k);

		// normalize weights
		norming=0.0;
		for (j=0; j<m_k; j++)
			norming += id_vector[j];

		for (j=0; j<m_k; j++)
			id_vector[j]/=norming;

		// put weights into W matrix
		for (j=0; j<m_k; j++)
			W_matrix[N*neighborhood_matrix[j*N+i]+i]=id_vector[j];
	}
	return NULL;
}

SGMatrix<int32_t> CKernelLocallyLinearEmbedding::get_neighborhood_matrix(SGMatrix<float64_t> kernel_matrix)
{
	int32_t t;
	int32_t N = kernel_matrix.num_cols;
	// init matrix and heap to be used
	int32_t* neighborhood_matrix = SG_MALLOC(int32_t, N*m_k);
#ifdef HAVE_PTHREAD
	int32_t num_threads = parallel->get_num_threads();
	ASSERT(num_threads>0);
	K_NEIGHBORHOOD_THREAD_PARAM* parameters = SG_MALLOC(K_NEIGHBORHOOD_THREAD_PARAM, num_threads);
	pthread_t* threads = SG_MALLOC(pthread_t, num_threads);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
#else
	int32_t num_threads = 1;
#endif
	CFibonacciHeap** heaps = SG_MALLOC(CFibonacciHeap*, num_threads);
	for (t=0; t<num_threads; t++)
		heaps[t] = new CFibonacciHeap(N);

#ifdef HAVE_PTHREAD
	for (t=0; t<num_threads; t++)
	{
		parameters[t].idx_start = t;
		parameters[t].idx_step = num_threads;
		parameters[t].idx_stop = N;
		parameters[t].m_k = m_k;
		parameters[t].N = N;
		parameters[t].heap = heaps[t];
		parameters[t].neighborhood_matrix = neighborhood_matrix;
		parameters[t].kernel_matrix = kernel_matrix.matrix;
		pthread_create(&threads[t], &attr, run_neighborhood_thread, (void*)&parameters[t]);
	}
	for (t=0; t<num_threads; t++)
		pthread_join(threads[t], NULL);
	pthread_attr_destroy(&attr);
	SG_FREE(threads);
	SG_FREE(parameters);
#else
	K_NEIGHBORHOOD_THREAD_PARAM single_thread_param;
	single_thread_param.idx_start = 0;
	single_thread_param.idx_step = 1;
	single_thread_param.idx_stop = N;
	single_thread_param.m_k = m_k;
	single_thread_param.N = N;
	single_thread_param.heap = heaps[0]
	single_thread_param.neighborhood_matrix = neighborhood_matrix;
	single_thread_param.kernel_matrix = kernel_matrix.matrix;
	run_neighborhood_thread((void*)&single_thread_param);
#endif

	for (t=0; t<num_threads; t++)
		delete heaps[t];
	SG_FREE(heaps);

	return SGMatrix<int32_t>(neighborhood_matrix,m_k,N);
}


void* CKernelLocallyLinearEmbedding::run_neighborhood_thread(void* p)
{
	K_NEIGHBORHOOD_THREAD_PARAM* parameters = (K_NEIGHBORHOOD_THREAD_PARAM*)p;
	int32_t idx_start = parameters->idx_start;
	int32_t idx_step = parameters->idx_step;
	int32_t idx_stop = parameters->idx_stop;
	int32_t N = parameters->N;
	int32_t m_k = parameters->m_k;
	CFibonacciHeap* heap = parameters->heap;
	const float64_t* kernel_matrix = parameters->kernel_matrix;
	int32_t* neighborhood_matrix = parameters->neighborhood_matrix;

	int32_t i,j;
	float64_t tmp;
	for (i=idx_start; i<idx_stop; i+=idx_step)
	{
		for (j=0; j<N; j++)
		{
			heap->insert(j,kernel_matrix[i*N+i]-2*kernel_matrix[i*N+j]+kernel_matrix[j*N+j]);
		}

		heap->extract_min(tmp);

		for (j=0; j<m_k; j++)
			neighborhood_matrix[j*N+i] = heap->extract_min(tmp);

		heap->clear();
	}

	return NULL;
}
#endif /* HAVE_LAPACK */
