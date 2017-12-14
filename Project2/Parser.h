#pragma once
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>
#include <set>
#include <cctype>
#include <sstream>
#include <algorithm>
#include "Element.h"
using namespace std;
class Parser
{
public:
	string input_file, output_file;
	bool rotate;
	Parser(int argc, char *argv[]);
	bool parse_input(list<Element> &elems, ofstream &ofile);
};

