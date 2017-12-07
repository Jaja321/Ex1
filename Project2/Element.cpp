#include "Element.h"

void Element::rotate() {
	int temp = right;
	right = top;
	top = left;
	left = bottom;
	bottom = temp;
	rotation += 90;
	if (rotation == 360)
		rotation = 0;
}