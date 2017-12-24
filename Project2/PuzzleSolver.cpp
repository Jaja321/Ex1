#include "PuzzleSolver.h"
#include <iostream>



PuzzleSolver::PuzzleSolver(list<Element> & _element_list, bool _rotate):element_list(_element_list),puzzle(_element_list.size()),rotate(_rotate)
{
	for (int i = 0; i < 4; i++) {
		//Build an element structure for each rotation:
		element_structures[i] = ElementsStructure(element_list);
		for (auto & e:element_list) {
			e.rotate();
		}
	}

}

/*
Tries to solves the puzzle with the specified width and height.
Returns false if no solution was found.
*/
bool PuzzleSolver::solve_puzzle_with_dimensions(int index, int width, int height)
{
	//find the appropriate edges for this position in the puzzle:
	int topSide = 0, leftSide = 0;
	if (index%width > 0)
		leftSide = -1 * puzzle[index - 1]->right;
	if (index / width > 0)
		topSide = -1 * puzzle[index - width]->bottom;
	bool is_bottom_edge = index / width == height - 1;
	bool is_right_edge = index%width == width - 1;

	for (int i = 0; i < (rotate ? 4 : 1); i++) { //for each rotation:
		ESIterator itr(element_structures[i], topSide, leftSide, is_right_edge, is_bottom_edge);
		Element *element_ptr = itr.getNext(); //get the next unseen element that fits this position
		while (element_ptr != nullptr) {
			for (int k = 0; k < i; k++)
				element_ptr->rotate();
			puzzle[index] = element_ptr;
			if (index == (width*height - 1))
				//puzzle is complete
				return true;
			else if (solve_puzzle_with_dimensions(index + 1, width, height)) //calls the function recursively
				return true;
			else {
				element_ptr->reset_rotation();
				element_ptr->seen = false;
				element_ptr = itr.getNext();
			}
		}
	}
	return false;
}


/*
Tries to solve the puzzle for every possible width/height.
*/
bool PuzzleSolver::solve_puzzle()
{

	for (int height : possible_heights) {
		int width = (int)element_list.size() / height;
		if (solve_puzzle_with_dimensions(0, width, height)) {
			puzzle_width = width;
			puzzle_height = height;
			return true;
		}
	}
	return false;
}

/*
Returns true if there are enough straight edges for the specified width and height to work.
*/
bool PuzzleSolver::verify_dimensions(int width, int height)
{
	int countTop = 0, countRight = 0, countBottom = 0, countLeft = 0; //Count straight edges.
	for (auto e : element_list) {
		if (e.top == 0)
			countTop++;
		if (e.right == 0)
			countRight++;
		if (e.bottom == 0)
			countBottom++;
		if (e.left == 0)
			countLeft++;
	}
	return countTop >= width && countBottom >= width && countRight >= height && countLeft >= height;
}

/*
Analayzes the elements in the list and checks for errors that weren't found in the Parser.
Pushes error messages to msgs.
Return True iff there are no errors
*/
bool PuzzleSolver::analyze_elements(list<string>& msgs)
{
	//Check if there is a correct number of straight edges:
	bool correct_number = true;
	int num = (int)element_list.size();
	for (int height = 1; height <= num; height++) {
		if (num%height != 0)
			continue;
		int width = num / height;
		if (rotate || verify_dimensions(width, height))
			possible_heights.push_back(height);
	}
	if (possible_heights.size() == 0)
		correct_number = false;

	//Check that corners exist and that the sum of edges is zero:
	bool TL = false, TR = false, BL = false, BR = false;
	int sum = 0, left = 0, right = 0, bottom = 0, top = 0;
	for (auto e : element_list) {
		sum += e.bottom + e.top + e.right + e.left;
		if (e.left == 0) {
			left++;
			if (e.top == 0)
				TL = true;
			if (e.bottom == 0)
				BL = true;
		}
		if (e.right == 0) {
			right++;
			if (e.top == 0)
				TR = true;
			if (e.bottom == 0)
				BR = true;
		}
		if (e.top == 0)
			top++;
		if (e.bottom == 0)
			bottom++;
	}
	if (!rotate) {
		if (top != bottom || left != right)
			correct_number = false;
		if (!correct_number)
			msgs.push_back("Cannot solve puzzle: wrong number of straight edges\n");
		if (!TL)
			msgs.push_back("Cannot solve puzzle: missing corner element: TL\n");
		if (!TR)
			msgs.push_back("Cannot solve puzzle: missing corner element: TR\n");
		if (!BL)
			msgs.push_back("Cannot solve puzzle: missing corner element: BL\n");
		if (!BR)
			msgs.push_back("Cannot solve puzzle: missing corner element: BR\n");
	}
	if (sum != 0)
		msgs.push_back("Cannot solve puzzle: sum of edges is not zero\n");

	return msgs.size() == 0;
}


/*
Prints the puzzle solution to the output file.
*/
void PuzzleSolver::print_puzzle(ofstream & ofile)
{
	for (int i = 0; i < puzzle_height; i++) {
		for (int j = 0; j <puzzle_width; j++) {
			Element e = *puzzle[getIndex(j, i, puzzle_width)];
			ofile << e.id;
			if (e.rotation != 0)
				ofile << " [" << e.rotation << "]";
			if (j < puzzle_width - 1)
				ofile << " ";
		}
		ofile << endl;
	}
}


