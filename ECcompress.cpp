/*
 * compress.cpp
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
#include "BitOutputStream.cpp"
#define BLOCK_SIZE 65535 //Number of chars to build a new header

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		return 1;
	}
	ifstream input;
	input.open(argv[1], ios::in);
	if (!input)
		return 1;

	ofstream outputfile(argv[2], ios::out);
	BitOutputStream out;
	out.connect(outputfile);

	int Block = 0;
	/*ENCODE LOOP
	 * Until the end of input...
	 * 		Save the current position in the file as Block * BLOCK_SIZE;
	 * 		Fill Characters with the number of chars, up to BLOCK_SIZE
	 * 		Set Freq_Mode and build the tree
	 * 		Set Readhead back to Block * BLOCK_SIZE
	 * 		Encode the header for this block
	 * 		Encode up to BLOCK_SIZE Characters, or until EOF
	 * 		*/
	while (!input.eof()) {
		unsigned char current; //The current character being read at input
		int index = 0; //The nth character read so far in this block
		vector<int> characters = vector<int>(256, 0); //Needed to build the tree for this block

		// until EOF, tally the number of symbols
		while (input.is_open() && index < BLOCK_SIZE) {

			current = input.get();

			//Don't store the eof
			if (input.eof()) {
				break;
			}
			index++;
			characters[current]++;
		}


		HCTree Tree;
		//SET FREQMODE BEFORE CONTINUING
		int highestFreq = 0;
		for (int current : characters) {
			if (current > highestFreq) {
				highestFreq = current;
			}
		}
		//Don't create an empty header, unless the file is empty.
		if(input.eof() && !highestFreq && Block){
			break;
		}
		Tree.freqMode = 3;
		if (highestFreq < 1 << 24) {
			Tree.freqMode = 2;
		}
		if (highestFreq < 1 << 16) {
			Tree.freqMode = 1;
		}
		if (highestFreq < 1 << 8) {
			Tree.freqMode = 0;
		}

		Tree.build(characters);

		//input header data

		//EC: Store Input File Readhead's position here.
		Tree.encodeHeader(out, characters,true);

		//Put the Readhead back at the stored value to start encoding
		input.clear();
		input.seekg(Block * BLOCK_SIZE, ios::beg);
		index = 0;

		//Grab each symbol and pass it to encode
		while (input.is_open() && index < Tree.charCount) {
			current = input.get();

			//Don't write the EOF
			if (input.eof()) {
				break;
			}
			Tree.encode(current, out);
			index++;
		}
		Block++;
	}

	out.close();

	return 0;
}

