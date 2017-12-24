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

void Element::reset_rotation()
{
	int n = rotation / 90;
	if (n != 0) {
		for (int i = 0; i < 4 - n; i++)
			rotate();
		rotation = 0;
	}
}
