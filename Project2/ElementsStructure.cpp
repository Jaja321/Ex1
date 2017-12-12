#include "ElementsStructure.h"


/*
Initialize structure from elements list.
*/
ElementsStructure::ElementsStructure(list<Element>& elements_list)
{
	for (auto itr = elements_list.begin(); itr != elements_list.end(); itr++) {
		elements[get_outer_index((*itr).top, (*itr).left)][get_inner_index((*itr).bottom, (*itr).right)].push_back(&(*itr));
	}
}
