#include <ctime>
#include "Element.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>  
#include <ctype>

//#include <math.h>  
using namespace std;

Element* ParseInput(string filename,int *num) {
	ifstream fin;
	fin.open(filename);
	bool isop=fin.is_open();

	if (Áisop==false) {//work ?????
		std::cout << "Error opening file";
		return NULL;
	}
	string line;
	getline(fin, line);
	string n = line.substr(line.find("=")+1, line.length());
	 *num = stoi(n);
	Element* elements = new Element[*num]; //remember to delete
	string id;
	string left;
	string top;
	string right;
	string bottom;
	for (int i = 0; i < *num; i++) {
		fin >> id;
		fin >> left;
		fin >> top;
		fin >> right;
		fin >> bottom;
		//we need to check errors like letters of -2
		if ((!id.empty() && std::find_if(id.begin(), id.end(), [](char c) { return !std::isdigit(c); }) == id.end()) || stoi(id) < 1 || stoi(id) > *num){
		
		
		}

		elements[i].id = stoi(id);
		elements[i].left = stoi(left);
		elements[i].top = stoi(top);
		elements[i].right = stoi(right);
		elements[i].bottom = stoi(bottom);
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

int main(int argc, char *argv[]) {
	clock_t begin = clock();
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
	bool solved = false;
	int width, height;
	for (height = 1; height <= sqrt(num_of_elements); height++) {
		if (num_of_elements%height != 0)
			continue;
		width = num_of_elements / height;
		if (!VerifyDimensions(elements, width, height))
			continue;
		std::sort(elements, elements+num_of_elements);
		while (std::next_permutation(elements, elements+(num_of_elements))) {
			//printPerm(elements, num_of_elements);
			if (Verify(elements, width, height)) {
				solved = true;
				cout << "Solvable\n";
				break;
			}
		}
		if (solved)
			break;
	}
	if (!solved)
		cout << "Unsolvable\n";
	//Print result to flie
	ofstream ofile;
	ofile.open(argv[2]);
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			ofile << elements[getIndex(j, i, width)].id;
			ofile << " ";
		}
		ofile << "\n";
	}
	ofile.close();
	delete[] elements;
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << elapsed_secs;
	int x;
	cin >> x;
}