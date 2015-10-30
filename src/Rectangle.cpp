/*
 * Rectangle.cpp
 *
 *  Created on: 12.10.2015
 *      Author: andreas
 */

#include "Core/Rectangle.h"

namespace fcv {

Rectangle::Rectangle(float x, float y, float width, float height){
	this->leftTop[0] = x;
	this->leftTop[1] = y;
	this->width = width;
	this->height = height;
}

Rectangle::~Rectangle() {

}

void Rectangle::setBounds(float x, float y, float width, float height)
{
	this->leftTop[0] = x;
	this->leftTop[1] = y;
	this->width = width;
	this->height = height;
}

} /* namespace fcv */
