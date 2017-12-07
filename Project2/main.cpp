#include "Element.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <list>
#include <vector>
#include <set>
#include <cctype>
using namespace std;


bool ParseInput(string filename, list<Element> &elems, ofstream &ofile) {
	ifstream fin;
	fin.open(filename);
	if (fin.fail()) {
		cout << "Error while opening input file\n";
		return false;
	}
	string line;
	getline(fin, line);
	string n = line.substr(line.find("=")+1, line.length());
	if (n == "") {
		cout << "No number of elements in input file\n";
		fin.close();
		return false;
	}
	for (auto itr = n.begin(); itr != n.end();) {
		if (*itr!=' ' && !std::isdigit(*itr)) {
			cout << "Number of elements is non-numeric\n";
			fin.close();
			return false;
		}
		++itr;
	}
	int num = stoi(n);
	int id, left, top, right, bottom;
	
	bool error = false;
	vector<int> wrongids;
	vector<string> msgs;
	set<int> ids;
	for (int i = 0; i < num; i++) {
		getline(fin, line);
		if (line == "" || line == " ")
			continue;
		std::istringstream iss(line);
		if (!(iss >> id)) {
			ofile << "The following line has non-numerical id: " + line+"\n";
			error = true;
			continue;
		}
		if (id > num) {
			error = true;
			wrongids.push_back(id);
			continue;
		}
		ids.insert(id);
		string str;
		if (!(iss >> left) ||left>1 || left<-1|| !(iss >> top) || top>1 || top<-1 || !(iss >> right) || right>1 || right<-1 || !(iss >> bottom) || bottom>1 || bottom<-1 || (iss>>str)) {
			msgs.push_back("Puzzle ID " + to_string(id) + " has wrong data: " + line+"\n");
			error = true;
			continue;
		}
		elems.push_back(Element(id, left, top, right, bottom));

	}
	fin.close();
	if ((int)ids.size() < num) {
		error = true;
		bool first = true;
		ofile << "Missing puzzle element(s) with the following IDs: ";
		for (int i = 1; i <= num; i++) {
			if (ids.find(i) == ids.end()) {
				if (!first)
					ofile << ", ";
				first = false;
				ofile << i;
			}
		}
		ofile << "\n";
	}
	if (wrongids.size() > 0) {
		error = true;
		sort(wrongids.begin(), wrongids.end());
		ofile << "Puzzle of size "<<num <<" cannot have the following IDs: ";
		bool first = true;
		for (int i : wrongids) {
			if (!first)
				ofile << ", ";
			first = false;
			ofile << i;
		}
		ofile << "\n";
	}
	for (string str : msgs) {
		ofile << str;
	}
	return !error;
}

int getIndex(int x, int y, int width) {
	return y*width + x;
}


//Verify that there are enough straight edges for the width and height to work.
bool verifyDimensions(const list<Element> &elems, int width, int height) {
	int countTop = 0, countRight = 0, countBottom = 0, countLeft = 0; //Count straight edges.
	for (auto e: elems) {
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

bool analyzeElements(const list<Element> &elems, list<string> &msgs, list<int> &possibleHeights) {
	//Check if there is a correct number of straight edges:
	bool correct_number = true;
	int num = (int)elems.size();
	for (int height = 1; height <= num; height++) {
		if (num%height != 0)
			continue;
		int width = num / height;
		if (verifyDimensions(elems, width, height))
			possibleHeights.push_back(height);
	}
	if (possibleHeights.size() == 0)
		correct_number = false;

	//Check that corners exist and that the sum of edges is zero:
	bool TL = false, TR = false, BL = false, BR = false;
	int sum = 0, left = 0, right = 0, bottom = 0, top = 0;
	for (auto e : elems) {
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
	if (sum != 0)
		msgs.push_back("Cannot solve puzzle: sum of edges is not zero\n");

	return msgs.size() == 0;
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
		if (!seen && (*itr).left == leftSide && (*itr).top == topSide && ((index%width) < (width - 1) || (*itr).right == 0) && ((index / width) < (height - 1) || (*itr).bottom == 0)) {	
			puzzle[index] = *itr;
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

bool solvePuzzleRotate(vector<Element> &puzzle, list<Element> elements, int index, int width, int height) {
	int topSide = 0, leftSide = 0;
	if (index%width > 0)
		leftSide = -1 * puzzle[index - 1].right;
	if (index / width > 0)
		topSide = -1 * puzzle[index - width].bottom;
	set<Element> seenElements;
	for (auto itr = elements.begin(); itr != elements.end();) {
		//Check if element was already seen in any rotation:
		bool seen = false;
		for (int i = 0; i < 4; i++) { 
			if (seenElements.find(*itr) != seenElements.end()) {
				seen = true;
				break;
			}
			(*itr).rotate();
		}
		if (seen) {
			++itr;
			continue;
		}
		//Check if element fits in any rotation:
		Element currentElement = *itr;
		itr= elements.erase(itr);
		for (int i = 0; i < 4; i++) {
			if (currentElement.left == leftSide && currentElement.top == topSide && ((index%width) < (width - 1) || currentElement.right == 0) && ((index / width) < (height - 1) || currentElement.bottom == 0)) {
				puzzle[index] = currentElement;
				if (index == (width*height - 1))
					return true;
				else if (solvePuzzleRotate(puzzle, elements, index + 1, width, height))
					return true;
			}
			currentElement.rotate();
		}
		seenElements.insert(currentElement);
		elements.insert(itr, currentElement);
	}
	return false;
}

void printPuzzle(const vector<Element> &puzzle, ofstream &ofile, int width, int height) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Element e = puzzle[getIndex(j, i, width)];
			ofile << e.id << " ";
			if (e.rotation != 0)
				ofile << "[" << e.rotation << "] ";
		}
		ofile << endl;
	}
}

int main(int argc, char *argv[]) {
	//Parse arguments:
	if (argc != 3 && argc !=4) {
		cout << "Incorrect number of arguments.\n";
		return 0;
	}
	vector<string> file_names;
	bool rotate = false;
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-rotate")
			rotate = true;
		else
			file_names.push_back(arg);
	}
	string input_file = file_names[0];
	string output_file = file_names[1];

	ofstream ofile;
	ofile.open(output_file);
	if (ofile.fail()) {
		cout << "Error while opening output file\n";
		return 0;
	}
	list<Element> elem_list;
	if (!ParseInput(input_file, elem_list, ofile)) {
		ofile.close();
		return 0;
	}
	list<string> msgs;
	list<int> possibleHeights;
	if (!rotate && !analyzeElements(elem_list, msgs, possibleHeights)) {
		for (string str : msgs)
			ofile << str;
		ofile.close();
		return 0;
	}
	int num_of_elements=(int)elem_list.size();
	vector<Element> puzzle(num_of_elements);
	if (!rotate) {
		for (int height : possibleHeights) {
			int width = num_of_elements / height;
			if (solvePuzzle(puzzle, elem_list, 0, width, height)) {
				printPuzzle(puzzle, ofile, width, height);
				ofile.close();
				return 0;
			}
		}
	}
	else {
		for (int height = 1; height <= num_of_elements; height++) {
			int width = num_of_elements / height;
			if (solvePuzzleRotate(puzzle, elem_list, 0, width, height)) {
				printPuzzle(puzzle, ofile, width, height);
				ofile.close();
				return 0;
			}
		}
	}
	
	ofile << "Cannot solve puzzle : it seems that there is no proper solution\n";
	ofile.close();
	return 0;
}