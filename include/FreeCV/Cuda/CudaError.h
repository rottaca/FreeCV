/*
 * CudaError.h
 *
 *  Created on: 27.04.2016
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_CUDA_CUDAERROR_H_
#define FREECV_INCLUDE_FREECV_CUDA_CUDAERROR_H_


#include "stdio.h"
#include <cuda.h>
#include <cuda_runtime.h>

#ifndef NDEBUG
#define CudaSafeCall( err ) __cudaSafeCall( err, __FILE__, __LINE__ )
#define CudaCheckError()    __cudaCheckError( __FILE__, __LINE__ )
#else
#define CudaSafeCall( err ) err
#define CudaCheckError()
#endif

inline void __cudaSafeCall(cudaError err, const char *file, const int line) {
#ifndef NDEBUG
	if (cudaSuccess != err) {
		fprintf(stderr, "cudaSafeCall() failed at %s:%i : %s\n", file, line,
				cudaGetErrorString(err));
		exit(-1);
	}
#endif

	return;
}

inline void __cudaCheckError(const char *file, const int line) {
#ifndef NDEBUG
	cudaError err = cudaGetLastError();
	if (cudaSuccess != err) {
		fprintf(stderr, "cudaCheckError() failed at %s:%i : %s\n", file, line,
				cudaGetErrorString(err));
		exit(-1);
	}

	// More careful checking. However, this will affect performance.
	// Comment away if needed.
	err = cudaDeviceSynchronize();
	if (cudaSuccess != err) {
		fprintf(stderr, "cudaCheckError() with sync failed at %s:%i : %s\n",
				file, line, cudaGetErrorString(err));
		exit(-1);
	}
#endif

	return;
}

#endif /* FREECV_INCLUDE_FREECV_CUDA_CUDAERROR_H_ */
