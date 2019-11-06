/*
 * uncompress.cpp
 * Author: saito
 * Date:  May 18, 2018
 * About: 
 */
#include<iostream>
#include "fstream"
#include <vector>
#include <string>
#include <cstdlib>
#include "HCTree.h"


using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3){
	return 1;
	}
	ifstream input;
	BitInputStream in;
	input.open(argv[1],ios::in);
	in.connect(input);
	if( !input )
	        return 1;
	vector<int> characters = vector<int>(256, 0);
	unsigned char current;




	// Using the header, construct the number of symbols

	HCTree Tree;

	Tree.decodeHeader(in, characters);
	int highestFreq = 0;
	for(int index = 0; index < characters.size();index++){
		if (characters[index] > highestFreq){
			highestFreq = characters[index];
		}
	}

	Tree.build(characters);

	// Prepare output file to insert data
	string outputfile = argv[2];
	ofstream output(argv[2], ios::out);
int currentChar = 0;

	//Read from input, then pass into decode to get current
	while(currentChar < Tree.charCount){
		char inputchar;
		inputchar = (char)Tree.decode(in);
		currentChar++;
		if (!input.eof() || currentChar != Tree.charCount){
		output<<inputchar;
		}
	}

	output.close();

	return 0;
}



