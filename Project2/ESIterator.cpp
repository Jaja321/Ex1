#include "ESIterator.h"



ESIterator::ESIterator(ElementsStructure & _es,int top, int left, bool is_right_edge, bool is_bottom_edge):es(_es)
{
	current_index = 0;
	outer_index = es.get_outer_index(top, left);
	if (is_right_edge && is_bottom_edge)
		indices = { 4 };
	else if (is_bottom_edge)
		indices = { 3,4,5 };
	else if (is_right_edge)
		indices = { 1,4,7 };
	else
		indices = { 0,1,2,3,4,5,6,7,8 };

}

/*
	Returns the next unseen element with the given edges requirements, and marks it as seen.
*/
Element * ESIterator::getNext()
{
	Element *ptr = nullptr;
	while (ptr == nullptr && current_index < (int)indices.size()) {
		list<Element*> &lst = es.getListAt(outer_index, indices[current_index]);
		for (auto e_ptr:lst) {
			if (e_ptr->seen == false) {
				ptr= e_ptr;
				ptr->seen = true;
				break;
			}
		}
		current_index++;
	}
	return ptr;
}
