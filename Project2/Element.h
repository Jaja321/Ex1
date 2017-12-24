#pragma once

/*
Class representing puzzle piece/element.
*/
class Element{
public:
	int id,left,top,right,bottom, rotation;
	bool seen;
	Element() :id(0), left(0), top(0), right(0), bottom(0), rotation(0) {}
	Element(int _id, int _left, int _top, int _right, int _bottom):id(_id),left(_left),top(_top),right(_right),bottom(_bottom), rotation(0),seen(false){}
	operator int() const { return (this->left + 1) + (this->top + 1) * 10 + (this->right + 1) * 100 + (this->bottom + 1) * 1000; }
	void rotate();
	void reset_rotation();
	
};