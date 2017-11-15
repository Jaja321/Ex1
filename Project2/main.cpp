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
	string id;
	string left;
	string top;
	string right;
	string bottom;
	for (int i = 0; i < num; i++) {
		fin >> id;
		fin >> left;
		fin >> top;
		fin >> right;
		fin >> bottom;
		//we need to check errors like letters of -2
		elements[i].id = stoi(id);
		elements[i].left = stoi(left);
		elements[i].top = stoi(top);
		elements[i].right = stoi(right);
		fin >> elements[i].bottom;
	}
	return elements;
}

int main(int argc, char *argv[]) {
	Element* elements = ParseInput(argv[1]);

	delete[] elements;
	int x;
	cin >> x;
}