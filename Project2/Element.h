#pragma once
class Element{
public:
	int id;
	int left;
	int top;
	int right;
	int bottom;
	operator int() const { return (this->left + 1) + (this->top + 1) * 10 + (this->right + 1) * 100 + (this->bottom + 1) * 1000; }
	Element& operator--();
};