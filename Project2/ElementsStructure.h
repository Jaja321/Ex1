#pragma once
#include <array>
#include <list>
#include "Element.h"
using namespace std;

/*
Used to store puzzle elements
*/
class ElementsStructure
{
public:
	array<array<list<Element*>, 9>, 9> elements; //Outer array has a cell for each top/left value, inner array for each right/bottom value.
	ElementsStructure(list<Element>& elements_list);
	int get_outer_index(int top, int left) { return (top + 1) * 3 + (left + 1); }
	int get_inner_index(int bottom, int right) { return (bottom + 1) * 3 + (right + 1); }
	list<Element*> &getListAt(int outer_index, int inner_index) { return elements[outer_index][inner_index]; }
};

