/*
 * ColorConvert.cpp
 *
 *  Created on: 01.11.2015
 *      Author: andreas
 */

#include <FreeCV/Image/ColorConvert.h>
#include <algorithm>
namespace fcv {

#define CLIP(X)	X > 255? 255: X<0? 0 : (int) X

bool convertGRAY_RGB(Image* input, Image* output);
bool convertRGB_GRAY(Image* input, Image* output);
bool convertYUYV_RGB(Image* input, Image* output);
bool convertYUYV_GRAY(Image* input, Image* output);
bool convertFLOAT_GRAY(Image* input, Image* output);
bool convertGRAY_FLOAT(Image* input, Image* output);

bool convertPxFormat(Image* input, Image* output, PixelFormatConvert mode) {
	switch (mode) {
	case GRAY8_TO_RGB_888:
		return convertGRAY_RGB(input, output);
		break;
	case RGB_888_TO_GRAY8:
		return convertRGB_GRAY(input, output);
		break;
	case YUYV_TO_RGB_888:
		return convertYUYV_RGB(input, output);
		break;
	case YUYV_TO_GRAY:
		return convertYUYV_RGB(input, output);
		break;
	case FLOAT_TO_GRAY:
		return convertFLOAT_GRAY(input, output);
		break;
	case GRAY_TO_FLOAT:
		return convertGRAY_FLOAT(input, output);
		break;
	default:
		break;
	}
	return false;
}
unsigned char* calculateColorLUT(int maxGray, int minH, int maxH) {
	maxGray++;
	unsigned char* lutData = new unsigned char[maxGray * 3];
	unsigned char* color = lutData;

	for (int grayVal = 0; grayVal < maxGray; grayVal++) {
		int h, s, v, r, b, g;
		// h: 0 - 359 (0 = rot, 120 = grün)
		// s: 0 - 100
		// v: 0 - 100
		s = 100;
		v = 100;
		h = (100 * grayVal / maxGray) * (maxH - minH) / 100 + minH;

		// HSV to RGB
		//Winkel im Farbkeis 0 - 360 in 1 Grad Schritten
		//h = (englisch hue) Farbwert
		//1 Grad Schrittweite, 4.25 Steigung pro Schritt bei 60 Grad
		if (h < 61) {
			r = 255;
			b = 0;
			g = 4.25 * h;
		} else if (h < 121) {
			g = 255;
			b = 0;
			r = 255 - (4.25 * (h - 60));
		} else if (h < 181) {
			r = 0;
			g = 255;
			b = 4.25 * (h - 120);
		} else if (h < 241) {
			r = 0;
			b = 255;
			g = 255 - (4.25 * (h - 180));
		} else if (h < 301) {
			g = 0;
			b = 255;
			r = 4.25 * (h - 240);
		} else if (h < 360) {
			r = 255;
			g = 0;
			b = 255 - (4.25 * (h - 300));
		}
		//Berechnung der Farbsättigung
		//s = (englisch saturation) Farbsättigung
		int diff;
		s = 100 - s; //Kehrwert berechnen
		diff = ((255 - r) * s) / 100;
		r = r + diff;
		diff = ((255 - g) * s) / 100;
		g = g + diff;
		diff = ((255 - b) * s) / 100;
		b = b + diff;

		//Berechnung der Dunkelstufe
		//v = (englisch value) Wert Dunkelstufe einfacher Dreisatz 0..100%
		r = (r * v) / 100;
		g = (g * v) / 100;
		b = (b * v) / 100;

		*color++ = r;
		*color++ = g;
		*color++ = b;
	}
	return lutData;
}
bool convertToPseudoColor(Image* input, Image* output, int minGray, int maxGray,
		int minH, int maxH) {
	output->init(input->getWidth(), input->getHeight(), Image::PF_RGB_888);

	unsigned char* outputPtr = output->getPtr<unsigned char>();

	if (input->getFormat() == Image::PF_GRAYSCALE_8) {
		//	unsigned char* colorLUT = calculateColorLUT(maxGray,minH,maxH);
		unsigned char* inputPtr = input->getPtr<unsigned char>();

		for (int i = 0; i < input->getWidth() * input->getHeight(); i++) {
			int h, s, v, r, b, g, grayVal;
			grayVal = *inputPtr;

			if (grayVal <= minGray || grayVal >= maxGray) {
				*outputPtr++ = 0;
				*outputPtr++ = 0;
				*outputPtr++ = 0;
				inputPtr++;
				continue;
			}
			// h: 0 - 359 (0 = rot, 120 = grün)
			// s: 0 - 100
			// v: 0 - 100
			s = 100;
			v = 100;
			h = (100 * grayVal / (maxGray - minGray + 1)) * (maxH - minH) / 100	+ minH;

			// HSV to RGB
			//Winkel im Farbkeis 0 - 360 in 1 Grad Schritten
			//h = (englisch hue) Farbwert
			//1 Grad Schrittweite, 4.25 Steigung pro Schritt bei 60 Grad
			if (h < 61) {
				r = 255;
				b = 0;
				g = 4.25 * h;
			} else if (h < 121) {
				g = 255;
				b = 0;
				r = 255 - (4.25 * (h - 60));
			} else if (h < 181) {
				r = 0;
				g = 255;
				b = 4.25 * (h - 120);
			} else if (h < 241) {
				r = 0;
				b = 255;
				g = 255 - (4.25 * (h - 180));
			} else if (h < 301) {
				g = 0;
				b = 255;
				r = 4.25 * (h - 240);
			} else if (h < 360) {
				r = 255;
				g = 0;
				b = 255 - (4.25 * (h - 300));
			}
			//Berechnung der Farbsättigung
			//s = (englisch saturation) Farbsättigung
			int diff;
			s = 100 - s; //Kehrwert berechnen
			diff = ((255 - r) * s) / 100;
			r = r + diff;
			diff = ((255 - g) * s) / 100;
			g = g + diff;
			diff = ((255 - b) * s) / 100;
			b = b + diff;

			//Berechnung der Dunkelstufe
			//v = (englisch value) Wert Dunkelstufe einfacher Dreisatz 0..100%
			r = (r * v) / 100;
			g = (g * v) / 100;
			b = (b * v) / 100;

			*outputPtr++ = r;
			*outputPtr++ = g;
			*outputPtr++ = b;
			//		int gray = *inputPtr;
			//
			//		if(gray > maxGray)
			//			gray = maxGray;
			//
			//		*outputPtr++ = colorLUT[gray];
			//		*outputPtr++ = colorLUT[gray + 1];
			//		*outputPtr++ = colorLUT[gray + 2];
			inputPtr++;
		}
	} else if (input->getFormat() == Image::PF_FLOAT_32) {
		//	unsigned char* colorLUT = calculateColorLUT(maxGray,minH,maxH);
		float* inputPtr = input->getPtr<float>();

		for (int i = 0; i < input->getWidth() * input->getHeight(); i++) {
			int h, s, v, r, b, g;
			float grayVal = *inputPtr;

			if (grayVal < minGray || grayVal > maxGray) {
				*outputPtr++ = 0;
				*outputPtr++ = 0;
				*outputPtr++ = 0;
				inputPtr++;
				continue;
			}
			// h: 0 - 359 (0 = rot, 120 = grün)
			// s: 0 - 100
			// v: 0 - 100
			s = 100;
			v = 100;
			h = (100 * grayVal / (maxGray - minGray + 1)) * (maxH - minH) / 100
					+ minH;

			// HSV to RGB
			//Winkel im Farbkeis 0 - 360 in 1 Grad Schritten
			//h = (englisch hue) Farbwert
			//1 Grad Schrittweite, 4.25 Steigung pro Schritt bei 60 Grad
			if (h < 61) {
				r = 255;
				b = 0;
				g = 4.25 * h;
			} else if (h < 121) {
				g = 255;
				b = 0;
				r = 255 - (4.25 * (h - 60));
			} else if (h < 181) {
				r = 0;
				g = 255;
				b = 4.25 * (h - 120);
			} else if (h < 241) {
				r = 0;
				b = 255;
				g = 255 - (4.25 * (h - 180));
			} else if (h < 301) {
				g = 0;
				b = 255;
				r = 4.25 * (h - 240);
			} else if (h < 360) {
				r = 255;
				g = 0;
				b = 255 - (4.25 * (h - 300));
			}
			//Berechnung der Farbsättigung
			//s = (englisch saturation) Farbsättigung
			int diff;
			s = 100 - s; //Kehrwert berechnen
			diff = ((255 - r) * s) / 100;
			r = r + diff;
			diff = ((255 - g) * s) / 100;
			g = g + diff;
			diff = ((255 - b) * s) / 100;
			b = b + diff;

			//Berechnung der Dunkelstufe
			//v = (englisch value) Wert Dunkelstufe einfacher Dreisatz 0..100%
			r = (r * v) / 100;
			g = (g * v) / 100;
			b = (b * v) / 100;

			*outputPtr++ = r;
			*outputPtr++ = g;
			*outputPtr++ = b;
			//		int gray = *inputPtr;
			//
			//		if(gray > maxGray)
			//			gray = maxGray;
			//
			//		*outputPtr++ = colorLUT[gray];
			//		*outputPtr++ = colorLUT[gray + 1];
			//		*outputPtr++ = colorLUT[gray + 2];
			inputPtr++;
		}
	} else {
		LOG_ERROR("Invalid pixel format!");
	}
//	delete colorLUT;
	return true;
}

bool convertGRAY_RGB(Image* input, Image* output) {
	output->init(input->getWidth(), input->getHeight(), Image::PF_RGB_888);

	unsigned char* inputPtr = input->getPtr<unsigned char>();
	unsigned char* outputPtr = output->getPtr<unsigned char>();

	for (int i = 0; i < input->getWidth() * input->getHeight(); i++) {

		*outputPtr++ = *inputPtr;
		*outputPtr++ = *inputPtr;
		*outputPtr++ = *inputPtr++;
	}
	return true;
}
bool convertRGB_GRAY(Image* input, Image* output) {
	output->init(input->getWidth(), input->getHeight(), Image::PF_GRAYSCALE_8);

	unsigned char* inputPtr = input->getPtr<unsigned char>();
	unsigned char* outputPtr = output->getPtr<unsigned char>();

	for (int i = 0; i < input->getWidth() * input->getHeight(); i++) {

		*outputPtr++ = *inputPtr++ * 0.144 + *inputPtr++ * 0.587
				+ *inputPtr++ * 0.299;
	}
	return true;
}
bool convertYUYV_RGB(Image* input, Image* output) {
	output->init(input->getWidth(), input->getHeight(), Image::PF_RGB_888);

	unsigned char* inputPtr = input->getPtr<unsigned char>();
	unsigned char* outputPtr = output->getPtr<unsigned char>();

	for (int i = 0; i < input->getWidth() * input->getHeight(); i += 2) {
		int u, v, y1, y2;

		y1 = *inputPtr++;
		u = *inputPtr++;
		y2 = *inputPtr++;
		v = *inputPtr++;

		// Integer operation of ITU-R standard for YCbCr is (from Wikipedia)
		// https://en.wikipedia.org/wiki/YUV#Y.27UV422_to_RGB888_conversion
		u = u - 128;
		v = v - 128;

		*outputPtr++ = CLIP(y1 + 45 * v / 32);
		*outputPtr++ = CLIP(y1 - (11 * u + 23 * v) / 32);
		*outputPtr++ = CLIP(y1 + 113 * u / 64);

		*outputPtr++ = CLIP(y2 + 45 * v / 32);
		*outputPtr++ = CLIP(y2 - (11 * u + 23 * v) / 32);
		*outputPtr++ = CLIP(y2 + 113 * u / 64);
	}
	return true;
}
bool convertYUYV_GRAY(Image* input, Image* output) {
	output->init(input->getWidth(), input->getHeight(), Image::PF_GRAYSCALE_8);

	unsigned char* inputPtr = input->getPtr<unsigned char>();
	unsigned char* outputPtr = output->getPtr<unsigned char>();

	for (int i = 0; i < input->getWidth() * input->getHeight(); i += 2) {
		int u, v, y1, y2;

		y1 = *inputPtr++;
		u = *inputPtr++;
		y2 = *inputPtr++;
		v = *inputPtr++;

		// Integer operation of ITU-R standard for YCbCr is (from Wikipedia)
		// https://en.wikipedia.org/wiki/YUV#Y.27UV422_to_RGB888_conversion
		u = u - 128;
		v = v - 128;

		*outputPtr++ = y1;
		*outputPtr++ = y1;
		*outputPtr++ = y1;

		*outputPtr++ = y2;
		*outputPtr++ = y2;
		*outputPtr++ = y2;
	}
	return true;
}
bool convertFLOAT_GRAY(Image* input, Image* output) {
	output->init(input->getWidth(), input->getHeight(), Image::PF_GRAYSCALE_8);

	float* inputPtr = input->getPtr<float>();
	unsigned char* outputPtr = output->getPtr<unsigned char>();
	float max = *std::max_element(inputPtr,
			inputPtr + input->getWidth() * input->getHeight());

	for (int i = 0; i < input->getWidth() * input->getHeight(); i++) {
		*outputPtr++ = *inputPtr++ * 255 / max;
	}
	return true;
}
bool convertGRAY_FLOAT(Image* input, Image* output) {
	output->init(input->getWidth(), input->getHeight(), Image::PF_FLOAT_32);

	unsigned char* inputPtr = input->getPtr<unsigned char>();
	float* outputPtr = output->getPtr<float>();

	for (int i = 0; i < input->getWidth() * input->getHeight(); i++) {
		*outputPtr++ = *inputPtr++;
	}
	return true;
}

} /* namespace fcv */
