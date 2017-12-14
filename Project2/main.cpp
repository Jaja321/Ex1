#include "Element.h"
#include "Parser.h"
#include "PuzzleSolver.h"
#include <fstream>
#include <iostream>
#include <string>
#include <list>

using namespace std;


int main(int argc, char *argv[]) {
	if (argc != 3 && argc !=4) {
		cout << "Incorrect number of arguments.\n";
		return 0;
	}
	
	Parser parser(argc, argv);

	ofstream ofile;
	ofile.open(parser.output_file);
	if (ofile.fail()) {
		cout << "Error while opening output file"<<endl;
		return 0;
	}

	list<Element> element_list;
	if (!parser.parse_input(element_list, ofile)) {
		ofile.close();
		return 0;
	}

	PuzzleSolver solver(element_list);

	list<string> msgs;
	if (!solver.analyze_elements(msgs)) {
		for (string str : msgs)
			ofile << str;
		ofile.close();
		return 0;
	}

	if (solver.solve_puzzle()) 
		solver.print_puzzle(ofile);
	else 
		ofile << "Cannot solve puzzle : it seems that there is no proper solution"<<endl;

	ofile.close();
	return 0;
}