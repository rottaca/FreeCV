/*
 * GPUData.cpp
 *
 *  Created on: 27.04.2016
 *      Author: andreas
 */

#include "include/FreeCV/Cuda/GPUData.h"
#include "include/FreeCV/Cuda/CudaError.h"

#include <cuda.h>
#include <cuda_runtime.h>

namespace fcv {

GPUData::GPUData(){
	gpuPtr = NULL;
	size = 0;
}

GPUData::~GPUData()
{
	if(gpuPtr != NULL)
	{
		cudaFree(gpuPtr);
		gpuPtr = NULL;
	}
}
bool GPUData::uploadData(size_t sizeNew, const void* data) {
	if (sizeNew != size) {
		// Release old buffer
		if(gpuPtr != NULL)
			cudaFree(gpuPtr);

		// Allocate new GPU buffer
		CudaSafeCall(cudaMalloc(&gpuPtr, sizeNew));
		size = sizeNew;
	}

	cudaMemcpy(gpuPtr,data,size,cudaMemcpyHostToDevice);
	return true;
}

void GPUData::downloadData(void* data) {
	CudaSafeCall(cudaMemcpy(data,gpuPtr,size,cudaMemcpyDeviceToHost));
}
}
