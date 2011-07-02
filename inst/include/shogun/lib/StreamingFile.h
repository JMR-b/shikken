/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2011 Shashwat Lal Das
 * Copyright (C) 2011 Berlin Institute of Technology and Max-Planck-Society
 */
#ifndef __STREAMING_FILE_H__
#define __STREAMING_FILE_H__

#include <shogun/lib/config.h>
#include <shogun/base/DynArray.h>
#include <shogun/lib/common.h>
#include <shogun/lib/File.h>
#include <shogun/lib/io.h>

namespace shogun
{
/** @brief A Streaming File access class.
 *
 * - Vectors are read as one vector per line
 * - NOT YET IMPLEMENTED:
 * - Matrices are written out as one column per line
 * - Sparse Matrices are written out as one column per line (libsvm/svmlight
 *	 style format)
 * - Strings are written out as one string per line
 *
 */
	class CStreamingFile: public CFile
	{
	public:
		/** default constructor	 */
		CStreamingFile();

		/** constructor
		 *
		 * @param f already opened file
		 * @param name variable name (e.g. "x" or "/path/to/x")
		 */
		CStreamingFile(FILE* f, const char* name=NULL);

		/** constructor
		 *
		 * @param fname filename to open
		 * @param rw mode, 'r' or 'w'
		 * @param name variable name (e.g. "x" or "/path/to/x")
		 */
		CStreamingFile(char* fname, char rw='r', const char* name=NULL);

		/** default destructor */
		virtual ~CStreamingFile();

		/** @name Vector Access Functions
		 *
		 * Functions to access vectors of one of the several base data types.
		 * These functions are used when loading vectors from e.g. file
		 * and return the vector and its length len by reference
		 */
		//@{
		virtual void get_bool_vector(bool*& vector, int32_t& len);
		virtual void get_byte_vector(uint8_t*& vector, int32_t& len);
		virtual void get_char_vector(char*& vector, int32_t& len);
		virtual void get_int_vector(int32_t*& vector, int32_t& len);
		virtual void get_real_vector(float64_t*& vector, int32_t& len);
		virtual void get_shortreal_vector(float32_t*& vector, int32_t& len);
		virtual void get_short_vector(int16_t*& vector, int32_t& len);
		virtual void get_word_vector(uint16_t*& vector, int32_t& len);
		virtual void get_int8_vector(int8_t*& vector, int32_t& len);
		virtual void get_uint_vector(uint32_t*& vector, int32_t& len);
		virtual void get_long_vector(int64_t*& vector, int32_t& len);
		virtual void get_ulong_vector(uint64_t*& vector, int32_t& len);
		virtual void get_longreal_vector(floatmax_t*& vector, int32_t& len);
		//@}

		/** @name Label and Vector Access Functions
		 *
		 * Functions to access the label and vectors of examples
		 * one of the several base data types.
		 * These functions are used when loading vectors from e.g. file
		 * and return the vector, its length, and the label by reference
		 */
		//@{
		virtual void get_bool_vector_and_label(bool*& vector, int32_t& len, float64_t& label);
		virtual void get_byte_vector_and_label(uint8_t*& vector, int32_t& len, float64_t& label);
		virtual void get_char_vector_and_label(char*& vector, int32_t& len, float64_t& label);
		virtual void get_int_vector_and_label(int32_t*& vector, int32_t& len, float64_t& label);
		virtual void get_real_vector_and_label(float64_t*& vector, int32_t& len, float64_t& label);
		virtual void get_shortreal_vector_and_label(float32_t*& vector, int32_t& len, float64_t& label);
		virtual void get_short_vector_and_label(int16_t*& vector, int32_t& len, float64_t& label);
		virtual void get_word_vector_and_label(uint16_t*& vector, int32_t& len, float64_t& label);
		virtual void get_int8_vector_and_label(int8_t*& vector, int32_t& len, float64_t& label);
		virtual void get_uint_vector_and_label(uint32_t*& vector, int32_t& len, float64_t& label);
		virtual void get_long_vector_and_label(int64_t*& vector, int32_t& len, float64_t& label);
		virtual void get_ulong_vector_and_label(uint64_t*& vector, int32_t& len, float64_t& label);
		virtual void get_longreal_vector_and_label(floatmax_t*& vector, int32_t& len, float64_t& label);
		//@}

		/** @name Vector Access Functions
		 *
		 * Functions to access vectors of one of the several base data types.
		 * These functions are used when loading vectors from e.g. file
		 * and return the vector and its length len by reference
		 */
		//@{
		virtual void get_bool_string(bool*& vector, int32_t& len);
		virtual void get_byte_string(uint8_t*& vector, int32_t& len);
		virtual void get_char_string(char*& vector, int32_t& len);
		virtual void get_int_string(int32_t*& vector, int32_t& len);
		virtual void get_real_string(float64_t*& vector, int32_t& len);
		virtual void get_shortreal_string(float32_t*& vector, int32_t& len);
		virtual void get_short_string(int16_t*& vector, int32_t& len);
		virtual void get_word_string(uint16_t*& vector, int32_t& len);
		virtual void get_int8_string(int8_t*& vector, int32_t& len);
		virtual void get_uint_string(uint32_t*& vector, int32_t& len);
		virtual void get_long_string(int64_t*& vector, int32_t& len);
		virtual void get_ulong_string(uint64_t*& vector, int32_t& len);
		virtual void get_longreal_string(floatmax_t*& vector, int32_t& len);
		//@}

		/** @name Label and Vector Access Functions
		 *
		 * Functions to access the label and vectors of examples
		 * one of the several base data types.
		 * These functions are used when loading vectors from e.g. file
		 * and return the vector, its length, and the label by reference
		 */
		//@{
		virtual void get_bool_string_and_label(bool*& vector, int32_t& len, float64_t& label);
		virtual void get_byte_string_and_label(uint8_t*& vector, int32_t& len, float64_t& label);
		virtual void get_char_string_and_label(char*& vector, int32_t& len, float64_t& label);
		virtual void get_int_string_and_label(int32_t*& vector, int32_t& len, float64_t& label);
		virtual void get_real_string_and_label(float64_t*& vector, int32_t& len, float64_t& label);
		virtual void get_shortreal_string_and_label(float32_t*& vector, int32_t& len, float64_t& label);
		virtual void get_short_string_and_label(int16_t*& vector, int32_t& len, float64_t& label);
		virtual void get_word_string_and_label(uint16_t*& vector, int32_t& len, float64_t& label);
		virtual void get_int8_string_and_label(int8_t*& vector, int32_t& len, float64_t& label);
		virtual void get_uint_string_and_label(uint32_t*& vector, int32_t& len, float64_t& label);
		virtual void get_long_string_and_label(int64_t*& vector, int32_t& len, float64_t& label);
		virtual void get_ulong_string_and_label(uint64_t*& vector, int32_t& len, float64_t& label);
		virtual void get_longreal_string_and_label(floatmax_t*& vector, int32_t& len, float64_t& label);
		//@}

		/** @name Vector Access Functions
		 *
		 * Functions to access vectors of one of the several base data types.
		 * These functions are used when loading vectors from e.g. file
		 * and return the vector and its length len by reference
		 */
		//@{
		virtual void get_bool_sparse_vector(SGSparseVectorEntry<bool>*& vector, int32_t& len);
		virtual void get_byte_sparse_vector(SGSparseVectorEntry<uint8_t>*& vector, int32_t& len);
		virtual void get_char_sparse_vector(SGSparseVectorEntry<char>*& vector, int32_t& len);
		virtual void get_int_sparse_vector(SGSparseVectorEntry<int32_t>*& vector, int32_t& len);
		virtual void get_real_sparse_vector(SGSparseVectorEntry<float64_t>*& vector, int32_t& len);
		virtual void get_shortreal_sparse_vector(SGSparseVectorEntry<float32_t>*& vector, int32_t& len);
		virtual void get_short_sparse_vector(SGSparseVectorEntry<int16_t>*& vector, int32_t& len);
		virtual void get_word_sparse_vector(SGSparseVectorEntry<uint16_t>*& vector, int32_t& len);
		virtual void get_int8_sparse_vector(SGSparseVectorEntry<int8_t>*& vector, int32_t& len);
		virtual void get_uint_sparse_vector(SGSparseVectorEntry<uint32_t>*& vector, int32_t& len);
		virtual void get_long_sparse_vector(SGSparseVectorEntry<int64_t>*& vector, int32_t& len);
		virtual void get_ulong_sparse_vector(SGSparseVectorEntry<uint64_t>*& vector, int32_t& len);
		virtual void get_longreal_sparse_vector(SGSparseVectorEntry<floatmax_t>*& vector, int32_t& len);
		//@}

		/** @name Vector Access Functions
		 *
		 * Functions to access vectors of one of the several base data types.
		 * These functions are used when loading vectors from e.g. file
		 * and return the vector and its length len by reference
		 */
		//@{
		virtual void get_bool_sparse_vector_and_label(SGSparseVectorEntry<bool>*& vector, int32_t& len, float64_t& label);
		virtual void get_byte_sparse_vector_and_label(SGSparseVectorEntry<uint8_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_char_sparse_vector_and_label(SGSparseVectorEntry<char>*& vector, int32_t& len, float64_t& label);
		virtual void get_int_sparse_vector_and_label(SGSparseVectorEntry<int32_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_real_sparse_vector_and_label(SGSparseVectorEntry<float64_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_shortreal_sparse_vector_and_label(SGSparseVectorEntry<float32_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_short_sparse_vector_and_label(SGSparseVectorEntry<int16_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_word_sparse_vector_and_label(SGSparseVectorEntry<uint16_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_int8_sparse_vector_and_label(SGSparseVectorEntry<int8_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_uint_sparse_vector_and_label(SGSparseVectorEntry<uint32_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_long_sparse_vector_and_label(SGSparseVectorEntry<int64_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_ulong_sparse_vector_and_label(SGSparseVectorEntry<uint64_t>*& vector, int32_t& len, float64_t& label);
		virtual void get_longreal_sparse_vector_and_label(SGSparseVectorEntry<floatmax_t>*& vector, int32_t& len, float64_t& label);
		//@}


	/** @name Matrix Access Functions
	 *
	 * Functions to access matrices of one of the several base data types.
	 * These functions are used when loading matrices from e.g. file
	 * and return the matrices and its dimensions num_feat and num_vec
	 * by reference
	 */
//@{
		virtual void get_byte_matrix(
			uint8_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_int8_matrix(
			int8_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_char_matrix(
			char*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_int_matrix(
			int32_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_uint_matrix(
			uint32_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_long_matrix(
			int64_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_ulong_matrix(
			uint64_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_shortreal_matrix(
			float32_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_real_matrix(
			float64_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_longreal_matrix(
			floatmax_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_short_matrix(
			int16_t*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_word_matrix(
			uint16_t*& matrix, int32_t& num_feat, int32_t& num_vec);
//@}

/** @name N-Dimensional Array Access Functions
 *
 * Functions to access n-dimensional arrays of one of the several base
 * data types. These functions are used when loading n-dimensional arrays
 * from e.g. file and return the them and its dimensions dims and num_dims
 * by reference
 */
//@{
		virtual void get_byte_ndarray(
			uint8_t*& array, int32_t*& dims, int32_t& num_dims);
		virtual void get_char_ndarray(
			char*& array, int32_t*& dims, int32_t& num_dims);
		virtual void get_int_ndarray(
			int32_t*& array, int32_t*& dims, int32_t& num_dims);
		virtual void get_shortreal_ndarray(
			float32_t*& array, int32_t*& dims, int32_t& num_dims);
		virtual void get_real_ndarray(
			float64_t*& array, int32_t*& dims, int32_t& num_dims);
		virtual void get_short_ndarray(
			int16_t*& array, int32_t*& dims, int32_t& num_dims);
		virtual void get_word_ndarray(
			uint16_t*& array, int32_t*& dims, int32_t& num_dims);
//@}

/** @name Sparse Matrix Access Functions
 *
 * Functions to access sparse matrices of one of the several base data types.
 * These functions are used when loading sparse matrices from e.g. file
 * and return the sparse matrices and its dimensions num_feat and num_vec
 * by reference
 */
//@{
		virtual void get_bool_sparsematrix(
			SGSparseVector<bool>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_byte_sparsematrix(
			SGSparseVector<uint8_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_int8_sparsematrix(
			SGSparseVector<int8_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_char_sparsematrix(
			SGSparseVector<char>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_int_sparsematrix(
			SGSparseVector<int32_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_uint_sparsematrix(
			SGSparseVector<uint32_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_long_sparsematrix(
			SGSparseVector<int64_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_ulong_sparsematrix(
			SGSparseVector<uint64_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_short_sparsematrix(
			SGSparseVector<int16_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_word_sparsematrix(
			SGSparseVector<uint16_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_shortreal_sparsematrix(
			SGSparseVector<float32_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_real_sparsematrix(
			SGSparseVector<float64_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
		virtual void get_longreal_sparsematrix(
			SGSparseVector<floatmax_t>*& matrix, int32_t& num_feat, int32_t& num_vec);
//@}


/** @name String Access Functions
 *
 * Functions to access strings of one of the several base data types.
 * These functions are used when loading variable length datatypes
 * from e.g. file and return the strings and their number
 * by reference
 */
//@{
		virtual void get_byte_string_list(
			SGString<uint8_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_int8_string_list(
			SGString<int8_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_char_string_list(
			SGString<char>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_int_string_list(
			SGString<int32_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_uint_string_list(
			SGString<uint32_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_short_string_list(
			SGString<int16_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_word_string_list(
			SGString<uint16_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_long_string_list(
			SGString<int64_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_ulong_string_list(
			SGString<uint64_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_shortreal_string_list(
			SGString<float32_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_real_string_list(
			SGString<float64_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
		virtual void get_longreal_string_list(
			SGString<floatmax_t>*& strings, int32_t& num_str,
			int32_t& max_string_len);
//@}

/** @name Vector Access Functions
 *
 * Functions to access vectors of one of the several base data types.
 * These functions are used when writing vectors of length len
 * to e.g. a file
 */
//@{
		virtual void set_byte_vector(const uint8_t* vector, int32_t len);
		virtual void set_char_vector(const char* vector, int32_t len);
		virtual void set_int_vector(const int32_t* vector, int32_t len);
		virtual void set_shortreal_vector( const float32_t* vector, int32_t len);
		virtual void set_real_vector(const float64_t* vector, int32_t len);
		virtual void set_short_vector(const int16_t* vector, int32_t len);
		virtual void set_word_vector(const uint16_t* vector, int32_t len);
//@}


/** @name Matrix Access Functions
 *
 * Functions to access matrices of one of the several base data types.
 * These functions are used when writing matrices of num_feat rows and
 * num_vec columns to e.g. a file
 */
//@{
		virtual void set_byte_matrix(
			const uint8_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_int8_matrix(
			const int8_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_char_matrix(
			const char* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_int_matrix(
			const int32_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_uint_matrix(
			const uint32_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_long_matrix(
			const int64_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_ulong_matrix(
			const uint64_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_shortreal_matrix(
			const float32_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_real_matrix(
			const float64_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_longreal_matrix(
			const floatmax_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_short_matrix(
			const int16_t* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_word_matrix(
			const uint16_t* matrix, int32_t num_feat, int32_t num_vec);
//@}

/** @name Sparse Matrix Access Functions
 *
 * Functions to access sparse matrices of one of the several base data types.
 * These functions are used when writing sparse matrices of num_feat rows and
 * num_vec columns to e.g. a file
 */
//@{
		virtual void set_bool_sparsematrix(
			const SGSparseVector<bool>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_byte_sparsematrix(
			const SGSparseVector<uint8_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_int8_sparsematrix(
			const SGSparseVector<int8_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_char_sparsematrix(
			const SGSparseVector<char>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_int_sparsematrix(
			const SGSparseVector<int32_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_uint_sparsematrix(
			const SGSparseVector<uint32_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_long_sparsematrix(
			const SGSparseVector<int64_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_ulong_sparsematrix(
			const SGSparseVector<uint64_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_short_sparsematrix(
			const SGSparseVector<int16_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_word_sparsematrix(
			const SGSparseVector<uint16_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_shortreal_sparsematrix(
			const SGSparseVector<float32_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_real_sparsematrix(
			const SGSparseVector<float64_t>* matrix, int32_t num_feat, int32_t num_vec);
		virtual void set_longreal_sparsematrix(
			const SGSparseVector<floatmax_t>* matrix, int32_t num_feat, int32_t num_vec);
//@}

/** @name String Access Functions
 *
 * Functions to access strings of one of the several base data types.
 * These functions are used when writing variable length datatypes
 * like strings to a file. Here num_str denotes the number of strings
 * and strings is a pointer to a string structure.
 */
//@{
		virtual void set_byte_string_list(
			const SGString<uint8_t>* strings, int32_t num_str);
		virtual void set_int8_string_list(
			const SGString<int8_t>* strings, int32_t num_str);
		virtual void set_char_string_list(
			const SGString<char>* strings, int32_t num_str);
		virtual void set_int_string_list(
			const SGString<int32_t>* strings, int32_t num_str);
		virtual void set_uint_string_list(
			const SGString<uint32_t>* strings, int32_t num_str);
		virtual void set_short_string_list(
			const SGString<int16_t>* strings, int32_t num_str);
		virtual void set_word_string_list(
			const SGString<uint16_t>* strings, int32_t num_str);
		virtual void set_long_string_list(
			const SGString<int64_t>* strings, int32_t num_str);
		virtual void set_ulong_string_list(
			const SGString<uint64_t>* strings, int32_t num_str);
		virtual void set_shortreal_string_list(
			const SGString<float32_t>* strings, int32_t num_str);
		virtual void set_real_string_list(
			const SGString<float64_t>* strings, int32_t num_str);
		virtual void set_longreal_string_list(
			const SGString<floatmax_t>* strings, int32_t num_str);
//@}


		/** @return object name */
		inline virtual const char* get_name() const { return "StreamingFile"; }
	};
}
#endif //__STREAMING_FILE_H__