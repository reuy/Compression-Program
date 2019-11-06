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
#define BLOCK_SIZE 65535 //Number of chars to build a new header

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		return 1;
	}
	ifstream input;
	BitInputStream in;
	input.open(argv[1], ios::in);
	in.connect(input);
	if (!input)
		return 1;

	unsigned char current;

	// Prepare output file to insert data
	string outputfile = argv[2];
	ofstream output(argv[2], ios::out);

	int Block = 0;

	/*DECODE LOOP
	 * Until the end of input...
	 * 		Save the current position in the file as Block * BLOCK_SIZE;
	 * 		Decode the Header for this Block
	 * 		Set Freq_Mode and build the tree
	 * 		Decode up to BLOCK_SIZE Characters, or until EOF
	 * 		*/

	while (!in.eof()) {
		vector<int> characters = vector<int>(256, 0);
		// Using the header, construct the number of symbols

		HCTree Tree;

		Tree.decodeHeader(in, characters,true);
		int highestFreq = 0;
		for (int index = 0; index < characters.size(); index++) {
			if (characters[index] > highestFreq) {
				highestFreq = characters[index];
			}
		}

		Tree.build(characters);

		int currentChar = 0;

		//Read from input, then pass into decode to get current
		while (currentChar < Tree.charCount) {
			char inputchar;
			inputchar = (char) Tree.decode(in);
			currentChar++;
			if (!in.eof() && currentChar <= Tree.charCount) {
				output << inputchar;
			}
		}
		input.peek();
		if(input.eof()){
			break;
		}
	}
	in.close();
	output.close();

	return 0;
}

