#include <ctime>
#include "Element.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>  
#include <ctype.h>
#include <list>
#include <vector>
#include <set>
//#include <math.h>  
using namespace std;


Element* ParseInput(string filename, int *num, ofstream ofile) {
	ifstream fin;
	string errmessage = "";
	fin.open(filename);
	bool isop=fin.is_open();
	std::ostringstream buffer;

	if (isop==false) {//work ?????
		std::cout << "Error while opening file";
		return NULL;
	}
	
	string line;
	getline(fin, line);
	string cline;
	strcpy(cline, line);// work!!!!!!??????
	string n = line.substr(line.find("=")+1, line.length());
	 *num = stoi(n);
	Element* elements = new Element[*num]; //remember to delete
	string id;
	string left;
	string top;
	string right;
	string bottom;
	string Missing_elements_error = "Missing puzzle element(s) with the following IDs :";
	string	Wrong_element_IDs_error = "Puzzle of size ";
	Wrong_element_IDs_error.append(*num);
	Wrong_element_IDs_error.append(" cannot have the following IDs :");


	string	Wrong_elements_format_error  ;
	getline(fin, line);
	

	for (int i = 0; i < *num; i++) {
		buffer << line;
		id=buffer.str(); 
		if ((!id.empty() && std::find_if(id.begin(), id.end(), [](char c) { return !std::isdigit(c); }) == id.end()) || stoi(id) < 1 || stoi(id) > *num) {
			Wrong_element_IDs_error.append(id);
			Wrong_element_IDs_error.append(",");
			continue;  
		
		}
	
		buffer << line;
		left = buffer.str();

		if (!isdigit(left) || stoi(left) < -1 || stoi(left) > 1) {
			
			Wrong_elements_format_error.append("Puzzle ID ");			
			Wrong_elements_format_error.append(id);
			Wrong_elements_format_error.append("has wrong data : ");
			Wrong_elements_format_error.append(cline);
			Wrong_elements_format_error.append("\n");

			continue;

		}
		


		
		buffer << line;
		top= buffer.str();

		if (!isdigit(top) || stoi(top) < -1 || stoi(top) > 1) {
			Wrong_elements_format_error.append("Puzzle ID ");
			Wrong_elements_format_error.append(id);
			Wrong_elements_format_error.append("has wrong data : ");
			Wrong_elements_format_error.append(cline);
			Wrong_elements_format_error.append("\n");

			continue;
		}
		

			
		buffer << line;
		 right= buffer.str();
		if (!isdigit(right) || stoi(right) < -1 || stoi(right) > 1) {
			Wrong_elements_format_error.append("Puzzle ID ");
			Wrong_elements_format_error.append(id);
			Wrong_elements_format_error.append("has wrong data : ");
			Wrong_elements_format_error.append(cline);
			Wrong_elements_format_error.append("\n");
			continue;

		}
		

		
		buffer << line;
		bottom= buffer.str();
		if (!isdigit(bottom) || stoi(bottom) < -1 || stoi(bottom) > 1) {
			Wrong_elements_format_error.append("Puzzle ID ");
			Wrong_elements_format_error.append(id);
			Wrong_elements_format_error.append("has wrong data : ");
			Wrong_elements_format_error.append(cline);
			Wrong_elements_format_error.append("\n");
			continue;
			
		}
		buffer << line; //works!!!!!!?????-------
		if(buffer.str()=="\n")
		elements[i].id = stoi(id);
		elements[i].right = stoi(right);
			elements[i].bottom = stoi(bottom);
			elements[i].top = stoi(top);
			elements[i].left = stoi(left);

		

	}
	fin.close();//i added

	return elements;
}

int getIndex(int x, int y, int width) {
	return y*width + x;
}

bool VerifyElement(int x, int y, int width, int height, Element *elems) {
	//Every element is responsible to check its top and left neighbors.
	Element e = elems[getIndex(x, y, width)];
	
	if (x == 0 && e.left != 0)
		return false;
	if (x == width - 1 && e.right != 0)
		return false;
	if (y == 0 && e.top != 0)
		return false;
	if (y == height - 1 && e.bottom != 0)
		return false;
	if (x > 0 && (e.left + elems[getIndex(x - 1, y, width)].right) != 0)
		return false;
	if (y > 0 && (e.top + elems[getIndex(x, y-1, width)].bottom) != 0)
		return false;
	return true;
}

//Verify that the permutaion of elements makes a valid puzzle.
bool Verify(Element *elems, int width, int height) {
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			if (!VerifyElement(i, j, width, height, elems))
				return false;
	return true;
}

//Verify that there are enough straight edges for the width and height to work.
bool VerifyDimensions(Element *elems, int width, int height) {
	int num = width*height;
	int countTop = 0, countRight = 0, countBottom = 0, countLeft = 0; //Count straight edges.
	for (int i = 0; i < num; i++) {
		if (elems[i].top == 0)
			countTop++;
		if (elems[i].right == 0)
			countRight++;
		if (elems[i].bottom == 0)
			countBottom++;
		if (elems[i].left == 0)
			countLeft++;
	}
	return countTop >= width && countBottom >= width && countRight >= height && countLeft >= height;
}

void printPerm(Element *elems, int num) {
	for (int i = -0; i < num; i++)
		cout << elems[i].id << ",";
	cout << "\n";
}

bool solvePuzzle(vector<Element> &puzzle, list<Element> elements, int index, int width,int height) {
	int topSide = 0, leftSide = 0;
	if (index%width > 0)
		leftSide =-1 *  puzzle[index - 1].right;
	if (index / width > 0)
		topSide =-1* puzzle[index - width].bottom;
	set<Element> seenElements;
	for (auto itr = elements.begin(); itr != elements.end();) {
		bool seen= seenElements.find(*itr) != seenElements.end();
		if ((*itr).left == leftSide && (*itr).top == topSide && ((index%width) < (width - 1) || (*itr).right == 0) && ((index / width) < (height - 1) || (*itr).bottom == 0) && !seen) {	
			puzzle[index] = *itr;
			//cout << "id: " << puzzle[index].id << "\n";
			itr = elements.erase(itr);
			if (index == (width*height - 1))
				return true;
			else if (solvePuzzle(puzzle, elements, index + 1, width, height))
				return true;
			else {
				seenElements.insert(puzzle[index]);
				elements.insert(itr, puzzle[index]);
			}
		}
		else {
			++itr;
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	clock_t begin = clock();
	ofstream ofile;
	ofile.open(argv[2]);
	if (argc != 3) {
		cout << "Incorrect number of arguments.\n";
		return 0;

	}
	int num_of_elements;
	Element* elements = ParseInput(argv[1], &num_of_elements);
	if (elements == NULL) {
		return 0;
	}
	cout << num_of_elements <<"\n";
	
	list<Element> elem_list;
	for (int i = 0; i < num_of_elements; i++) {
		elem_list.push_back(elements[i]);
	}
	vector<Element> puzzle(num_of_elements);
	bool solved = false;
	int width, height;
	for (height = 1; height <= num_of_elements; height++) {
		if (num_of_elements%height != 0)
			continue;
		width = num_of_elements / height;
		if (!VerifyDimensions(elements, width, height))
			continue;
		/*
		std::sort(elements, elements+num_of_elements);
		while (std::next_permutation(elements, elements+(num_of_elements))) {
			//printPerm(elements, num_of_elements);
			if (Verify(elements, width, height)) {
				solved = true;
				cout << "Solvable\n";
				break;
			}
		}
		*/
		//cout << "width: " << width << ", height: " << height << "\n";
		if (solvePuzzle(puzzle, elem_list, 0, width, height)) {
			solved = true;
			cout << "Solvable\n";
		}
		if (solved)
			break;

	}
	if (!solved)
		cout << "Unsolvable\n";
	

	
	//Print result to flie
	if (solved) {
		ofstream ofile;
		ofile.open(argv[2]);

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				ofile << puzzle[getIndex(j,i,width)].id;
				ofile << " ";
			}
			ofile << "\n";
		}
		ofile.close();
	}
	delete[] elements;
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << elapsed_secs;
	int x;
	cin >> x;
}