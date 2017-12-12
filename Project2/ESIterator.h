#pragma once
#include "ElementsStructure.h"
#include <vector>
/*
ElementsStructure Iterator
*/
class ESIterator
{
	vector<int> indices;
	int	outer_index, current_index;
	ElementsStructure es;
public:
	ESIterator(const ElementsStructure & _es, int top, int left, bool is_right_edge, bool is_bottom_edge);
	
	Element* getNext();
};

