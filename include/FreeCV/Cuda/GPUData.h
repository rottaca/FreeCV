/*
 * GPUData.h
 *
 *  Created on: 27.04.2016
 *      Author: andreas
 */

#ifndef FREECV_INCLUDE_FREECV_CUDA_GPUDATA_H_
#define FREECV_INCLUDE_FREECV_CUDA_GPUDATA_H_


#include "stdio.h"
namespace fcv {

/**
 * Handles a piece of memory on the GPU and synchronizes it with CPU data.
 */
class GPUData {
public:
	GPUData();
	virtual ~GPUData();

	bool uploadData(size_t sizeNew, const void* data);
	void downloadData(void* data);
	size_t getSize(){return size;}

	const void* getGPUPtr(){	return gpuPtr;}

protected:
	void* gpuPtr;
	size_t size;
};

} /* namespace fcv */

#endif /* FREECV_INCLUDE_FREECV_CUDA_GPUDATA_H_ */
