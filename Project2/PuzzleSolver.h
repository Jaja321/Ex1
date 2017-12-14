#pragma once
#include "Element.h"
#include "ElementsStructure.h"
#include "ESIterator.h"
#include <list>
#include <vector>
#include <fstream>

class PuzzleSolver
{
	const list<Element> & element_list; //List of puzzle elements.
	ElementsStructure elements; //Special data structure to hold elements.
	vector<Element*> puzzle; //The puzzle "board".
	list<int> possible_heights; //Possible heights of the puzzle
	int puzzle_width, puzzle_height;
	
	

public:
	PuzzleSolver(list<Element> & _element_list);
	bool solve_puzzle_with_dimensions(int index, int width, int height);
	bool solve_puzzle();
	bool verify_dimensions(int width, int height);
	bool analyze_elements(list<string> &msgs);
	void print_puzzle(ofstream &ofile);
	int getIndex(int x, int y, int w) {return y*w + x;} //Returns the index of the element in the puzzle.
};

