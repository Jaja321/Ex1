#include "Parser.h"



Parser::Parser(int argc, char *argv[])
{
	//Parse arguments:
	rotate = false;
	vector<string> file_names;
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-rotate")
			rotate = true;
		else
			file_names.push_back(arg);
	}
	input_file = file_names[0];
	output_file = file_names[1];
}

/*
Parses the input file. Returns false if there was an error in the input file.
*/
bool Parser::parse_input( list<Element>& elems, ofstream & ofile)
{
	//open input file:
	ifstream fin;
	fin.open(input_file);
	if (fin.fail()) {
		cout << "Error while opening input file\n";
		return false;
	}

	string line;
	//parse first line:
	getline(fin, line);
	string n = line.substr(line.find("=") + 1, line.length());
	if (n == "") {
		cout << "No number of elements in input file\n";
		fin.close();
		return false;
	}
	for (auto itr = n.begin(); itr != n.end();) {
		if (*itr != ' ' && !std::isdigit(*itr)) {
			cout << "Number of elements is non-numeric\n";
			fin.close();
			return false;
		}
		++itr;
	}
	int num = stoi(n);
	int id, left, top, right, bottom;

	bool error = false;
	vector<int> wrongids; //wrong ids
	vector<string> msgs; //error messages
	set<int> ids; //seen ids
	for (int i = 0; i < num; i++) {
		//for each line in input file:
		getline(fin, line);
		if (line == "" || line == " ")
			continue;
		std::istringstream iss(line);
		if (!(iss >> id)) {
			ofile << "The following line has non-numerical id: " + line + "\n";
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
		//Check if line contains wrong data:
		if (!(iss >> left) || left>1 || left<-1 || !(iss >> top) || top>1 || top<-1 || !(iss >> right) || right>1 || right<-1 || !(iss >> bottom) || bottom>1 || bottom<-1 || (iss >> str)) {
			msgs.push_back("Puzzle ID " + to_string(id) + " has wrong data: " + line + "\n");
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
		ofile << "Puzzle of size " << num << " cannot have the following IDs: ";
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