#pragma once
#include "ElementsStructure.h"
#include <vector>

/*
Used to iterate over ElementStructure
*/
class ESIterator
{
	vector<int> indices; //Indices in inner array to iterate over
	int	outer_index, current_index;
	ElementsStructure& es;
public:
	ESIterator(ElementsStructure & _es, int top, int left, bool is_right_edge, bool is_bottom_edge);
	
	Element* getNext();
};

