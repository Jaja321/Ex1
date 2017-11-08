#include "Element.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

Element* ParseInput(string filename) {
	int x = 90;
	ifstream fin(filename);
	string line;
	getline(fin, line);
	size_t num = stoi(line);
	Element* elements = new Element[num]; //remember to delete
	for (int i = 0; i < num; i++) {
		fin >> elements[i].id;
		fin >> elements[i].left;
		fin >> elements[i].top;
		fin >> elements[i].right;
		fin >> elements[i].bottom;
	}
	return elements;
}

int main(int argc, char *argv[]) {
	Element* elements = ParseInput(argv[1]);

	delete [] elements;
	int x;
	cin >> x;
}