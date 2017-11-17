#include "Element.h"

Element & Element::operator--()
{
	this->left--;
	if (this->left < -1) {
		this->left = 1;
		this->top--;
	}
	if (this->top < -1) {
		this->top = 1;
		this->right--;
	}
	if (this->right < -1) {
		this->right = 1;
		this->bottom--;
	}
	if (this->bottom < -1) {
		this->bottom = 1;
	}

	return *this;
}
