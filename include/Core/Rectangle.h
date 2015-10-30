/*
 * Rectangle.h
 *
 *  Created on: 12.10.2015
 *      Author: andreas
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_
#include "Core/Vector.h"
namespace fcv {

class Rectangle {
public:
	Rectangle(float x, float y, float width, float height);
	virtual ~Rectangle();

	void setBounds(float x, float y, float width, float height);

	float getLeft(){return leftTop[0];}
	float getTop(){return leftTop[1];}
	float getRight(){return leftTop[0] + width;}
	float getBottom(){return leftTop[1] + height;}

	float getWidth(){return width;}
	float getHeight(){return height;}

private:
	Vector2f leftTop;
	float width, height;
};

} /* namespace fcv */

#endif /* RECTANGLE_H_ */
