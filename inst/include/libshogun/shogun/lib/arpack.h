/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2011 Sergey Lisitsyn
 * Copyright (C) 2011 Berlin Institute of Technology and Max-Planck-Society
 */

#ifndef ARPACK_H_
#define ARPACK_H_
#ifdef HAVE_ARPACK
#ifdef HAVE_ATLAS
#include <cblas.h>
#include <shogun/lib/io.h>

/** external ARPACK routine DSAUPD */
extern "C" void dsaupd_(int *ido, char *bmat, int *n, char *which,
			int *nev, double *tol, double *resid, int *ncv,
			double *v, int *ldv, int *iparam, int *ipntr,
			double *workd, double *workl, int *lworkl,
			int *info);

/** external ARPACK routine DSEUPD */
extern "C" void dseupd_(int *rvec, char *All, int *select, double *d,
			double *v, int *ldv, double *sigma, 
			char *bmat, int *n, char *which, int *nev,
			double *tol, double *resid, int *ncv, double *tv,
			int *tldv, int *iparam, int *ipntr, double *workd,
			double *workl, int *lworkl, int *ierr);
namespace shogun
{

/** Wrapper for ARPACK's dsaupd routine. This wrapper and ARPACK 
 * routine is used to compute specified number of eigenpairs. 
 * Routine under wrapper involves a variant of Arnoldi process
 * called the IRAM (Implicitly Restarted Arnoldi Method) reduced
 * to IRLM (Implicitly Restarted Lanczos Method). A strategy 
 * specifying which eigenpairs to compute should be provided 
 * as parameter.
 *
 * Please note that some of routine parameters are hard-coded:
 * e.g. ncv and workaround arrays sizes. 
 *
 * @param matrix symmetric real matrix of size n*n
 * @param n size of matrix
 * @param nev number of eigenpairs to compute
 * @param which eigenvalue finding strategy. Possible values:
 *        - "LM": nev Largest Magnitude eigenvalues
 *        - "SM": nev Smallest Magnitude eigenvalues
 *        - "LA": nev Largest Algebraic eigenvalues
 *        - "SA": nev Smallest Algebraic eigenvalues
 *        - "BE": half of nev from each end of the spectrum, i.e. nev%2
 *                smallest and nev%2 largest eigenvalues. If nev is odd,
 *                one more largest eigenvalue will be computed
 * @param eigenvalues array of size nev to hold computed eigenvalues
 * @param eigenvectors array of size nev*n to hold computed eigenvectors
 */
void arpack_dsaupd(double* matrix, int n, int nev, char* which,
		   double* eigenvalues, double* eigenvectors, int& status);

}
#endif /* HAVE_ATLAS */
#endif /* HAVE_ARPACK */
#endif /* ARPACK_H_ */

