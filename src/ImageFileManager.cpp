/*
 * ImageFileManager.cpp
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#include "ImageFileManager.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

namespace fcv {

Image ImageFileManager::loadImage(std::string fileName) {

	std::ifstream file;
	file.open(fileName.c_str());
	Image invalidImg;
	if (!file.is_open())
		return invalidImg;

	std::string type,tmp;
	file >> type;

	if(type.compare("P2")==0)
	{
		int width;
		int height;
		file >> width;
		file >> height;
		file >> tmp;
		Image img(width,height,Image::GRAYSCALE_8);
		unsigned char* data = img.getPtr<unsigned char>();
		int tmp;
		for(int i = 0; i < width*height;i++){
			file >> tmp;
			*data++ = tmp;
		}
		return img;
	}
	else if(type.compare("P3")==0){
		int width;
		int height;
		file >> width;
		file >> height;
		file >> tmp;
		Image img(width,height,Image::RGB_888);
		unsigned char* data = img.getPtr<unsigned char>();
		int tmp;
		for(int i = 0; i < width*height*3;i++){
			file >> tmp;
			*data++ = tmp;
		}
		return img;

	}else{
		return invalidImg;
	}
}
bool ImageFileManager::saveImage(Image& image, std::string fileName) {

	switch (image.getFormat()) {
	case Image::RGB_888:{
		FILE *fp = fopen(fileName.c_str(), "w"); /* b - binary mode */
		fprintf(fp, "P3\n%d %d\n255\n", image.getWidth(), image.getHeight());
		unsigned char* ptr = image.getPtr<unsigned char>();
		for (int j = 0; j < image.getHeight(); ++j) {
			for (int i = 0; i < image.getWidth(); ++i) {

				fprintf(fp, "%d %d %d ", *ptr++, *ptr++, *ptr++);
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
		break;
	}
	case Image::GRAYSCALE_8:{
		FILE *fp = fopen(fileName.c_str(), "w"); /* b - binary mode */
		fprintf(fp, "P2\n%d %d\n255\n", image.getWidth(), image.getHeight());
		unsigned char* ptr = image.getPtr<unsigned char>();
		for (int j = 0; j < image.getHeight(); ++j) {
			for (int i = 0; i < image.getWidth(); ++i) {

				fprintf(fp, "%d ", *ptr++);
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
		break;
	}
	default:
		return false;
		break;
	}
	return true;
}

} /* namespace fcv */
