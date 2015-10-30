/*
 * ImageFileManager.cpp
 *
 *  Created on: 09.10.2015
 *      Author: andreas
 */

#include "Image/ImageFileManager.h"
#include <fstream>
#include "stdlib.h"
#include <string>
#include <vector>

namespace fcv {

std::string getNextValidLine(std::ifstream& file) {
	std::string line = "";

	while (getline(file,line) && (line.compare("") == 0 || line.find("#") != std::string::npos));
	return line;
}
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

Image ImageFileManager::loadImage(std::string fileName, ImageFileType type) {

	Image invalidImg;

	std::string tmp;

	if (type == UNKNOWN) {
		size_t pos = fileName.find(".");
		std::string fileExt(fileName.substr(pos + 1));
		if (fileExt.compare("pgm") == 0 || fileExt.compare("ppm") == 0) {
			std::ifstream file;
			file.open(fileName.c_str());
			if (!file.is_open())
				return invalidImg;

			file >> tmp;

			if (tmp.compare("P2") == 0)
				type = PGM_ASCII;
			else if (tmp.compare("P3") == 0)
				type = PPM_ASCII;
			else if (tmp.compare("P5") == 0)
				type = PGM_BINARY;
			else if (tmp.compare("P6") == 0)
				type = PPM_BINARY;
		} else
			return invalidImg;
	}

	switch (type) {
	case PPM_ASCII: {
		std::ifstream file;
		file.open(fileName.c_str());
		file >> tmp;
		if (tmp.compare("P3") != 0)
			return invalidImg;

		int width;
		int height;

		tmp = getNextValidLine(file);
		std::vector<std::string> s = split(tmp,' ');
		if (s.size() != 2)
			return invalidImg;

		width = atoi(s.at(0).c_str());
		height = atoi(s.at(1).c_str());
		tmp = getNextValidLine(file);

		Image img(width, height, Image::PF_RGB_888);
		unsigned char* data = img.getPtr<unsigned char>();

		for (int i = 0; i < width * height * 3; i++) {
			int tmp;
			file >> tmp;
			*data++ = tmp;
		}
		return img;

		break;
	}
	case PPM_BINARY: {
		std::ifstream file;
		file.open(fileName.c_str());
		file >> tmp;
		if (tmp.compare("P6") != 0)
			return invalidImg;

		int width;
		int height;

		tmp = getNextValidLine(file);
		std::vector<std::string> s = split(tmp,' ');
		if (s.size() != 2)
			return invalidImg;

		width = atoi(s.at(0).c_str());
		height = atoi(s.at(1).c_str());

		tmp = getNextValidLine(file);

		Image img(width, height, Image::PF_RGB_888);
		unsigned char* data = img.getPtr<unsigned char>();
		file.read((char*) data, width * height * 3);
		return img;

		break;
	}
	case PGM_ASCII: {
		std::ifstream file;
		file.open(fileName.c_str());
		file >> tmp;
		if (tmp.compare("P2") != 0)
			return invalidImg;

		int width;
		int height;

		tmp = getNextValidLine(file);
		std::vector<std::string> s = split(tmp,' ');
		if (s.size() != 2)
			return invalidImg;

		width = atoi(s.at(0).c_str());
		height = atoi(s.at(1).c_str());
		tmp = getNextValidLine(file);

		Image img(width, height, Image::PF_GRAYSCALE_8);
		unsigned char* data = img.getPtr<unsigned char>();

		for (int i = 0; i < width * height; i++) {
			int tmp;
			file >> tmp;
			*data++ = tmp;
		}
		return img;

		break;
	}
	case PGM_BINARY: {
		std::ifstream file;
		file.open(fileName.c_str());
		file >> tmp;
		if (tmp.compare("P5") != 0)
			return invalidImg;

		int width;
		int height;

		tmp = getNextValidLine(file);
		std::vector<std::string> s = split(tmp,' ');
		if (s.size() != 2)
			return invalidImg;

		width = atoi(s.at(0).c_str());
		height = atoi(s.at(1).c_str());
		tmp = getNextValidLine(file);

		Image img(width, height, Image::PF_GRAYSCALE_8);
		unsigned char* data = img.getPtr<unsigned char>();
		file.read((char*) data, width * height);
		return img;

		break;
	}
	}
	return invalidImg;

}
bool ImageFileManager::saveImage(Image* image, std::string fileName,
		ImageFileType type) {

	switch (image->getFormat()) {
	case Image::PF_RGB_888: {
		switch (type) {
		case PPM_ASCII: {
			std::ofstream f(fileName.c_str(),
					std::ios_base::out | std::ios_base::trunc);
			f << "P3\n" << image->getWidth() << " " << image->getHeight()
					<< "\n255\n";

			unsigned char* ptr = image->getPtr<unsigned char>();
			for (int j = 0; j < image->getHeight(); ++j) {
				for (int i = 0; i < image->getWidth(); ++i) {
					f << (int)*ptr++ << " " << (int)*ptr++ << " " << (int)*ptr++ << "\n";
				}
				f << "\n";
			}
			break;
		}
		case PPM_BINARY:
		case UNKNOWN: {
			std::ofstream f(fileName.c_str(),
					std::ios_base::binary | std::ios_base::out
							| std::ios_base::trunc);
			f << "P6\n" << image->getWidth() << " " << image->getHeight()
					<< "\n255\n";

			f.write((const char*) image->getPtr<unsigned char>(),
					image->getWidth() * image->getHeight() * 3);

		}

			break;
		default:
			break;
		}

		break;
	}
	case Image::PF_GRAYSCALE_8: {
		switch (type) {
		case PGM_ASCII: {
			std::ofstream f(fileName.c_str(),
					std::ios_base::out | std::ios_base::trunc);
			f << "P2\n" << image->getWidth() << " " << image->getHeight()
					<< "\n255\n";

			unsigned char* ptr = image->getPtr<unsigned char>();
			for (int j = 0; j < image->getHeight(); ++j) {
				for (int i = 0; i < image->getWidth(); ++i) {
					f << (int)*ptr++ << " ";
					if (i % 20 == 0)
						f << "\n";
				}
				f << "\n";
			}
			break;
		}
		case PGM_BINARY:
		case UNKNOWN: {
			std::ofstream f(fileName.c_str(),
					std::ios_base::binary | std::ios_base::out
							| std::ios_base::trunc);
			f << "P5\n" << image->getWidth() << " " << image->getHeight()
					<< "\n255\n";

			f.write((const char*) image->getPtr<unsigned char>(),
					image->getWidth() * image->getHeight());

		}

			break;
		default:
			return false;
			break;
		}
		break;
	}
	case Image::PF_FLOAT_32: {
		// Scale Float image
		float* ptr = image->getPtr<float>();
		float max = *ptr;
		float min = *ptr;
		for (int i = 0; i < image->getHeight() * image->getWidth(); i++) {
			float v = *ptr++;
			if (v < min)
				min = v;
			else if (v > max)
				max = v;
		}

		switch (type) {
		case PGM_ASCII: {
			std::ofstream f(fileName.c_str(),
					std::ios_base::out | std::ios_base::trunc);
			f << "P2\n" << image->getWidth() << " " << image->getHeight()
					<< "\n255\n";

			ptr = image->getPtr<float>();
			for (int j = 0; j < image->getHeight(); ++j) {
				for (int i = 0; i < image->getWidth(); ++i) {
					f << (char) ((*ptr++ - min) / (max - min) * 255) << " ";
					if (i % 20 == 0)
						f << "\n";
				}
				f << "\n";
			}
			break;
		}
		case PGM_BINARY:
		case UNKNOWN: {
			std::ofstream f(fileName.c_str(),
					std::ios_base::binary | std::ios_base::out
							| std::ios_base::trunc);
			f << "P5\n" << image->getWidth() << " " << image->getHeight()
					<< "\n255\n";

			ptr = image->getPtr<float>();
			for (int j = 0; j < image->getHeight(); ++j) {
				for (int i = 0; i < image->getWidth(); ++i) {
					const char v = ((*ptr++ - min) / (max - min) * 255);
					f.write(&v, 1);
				}
			}
		}

			break;
		default:
			return false;
			break;
		}
		break;
	}
	default:
		return false;
		break;
	}
	return true;
}

} /* namespace fcv */
