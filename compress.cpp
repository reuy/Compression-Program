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


using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3){
	return 1;
	}
	ifstream input;
	input.open(argv[1],ios::in);
	if( !input )
	        return 1;
	vector<int> characters = vector<int>(256, 0);
	unsigned char current;

	// until EOF, tally the number of symbols
	while(input.is_open()){
		current = input.get();

		if (input.eof()) {
			break;
		}

		characters[current]++;
	}

	HCTree Tree;
	//SET FREQMODE BEFORE CONTINUING
	int highestFreq = 0;
	for(int current : characters){
		if (current > highestFreq){
			highestFreq = current;
		}
	}
	Tree.freqMode=3;
	if(highestFreq < 1 << 24){
		Tree.freqMode = 2;
	}
	if(highestFreq < 1 << 16){
		Tree.freqMode = 1;
	}
	if(highestFreq < 1 << 8){
		Tree.freqMode = 0;
	}

	Tree.build(characters);



	//Open output stream and input header data
	ofstream outputfile(argv[2], ios::out);
	BitOutputStream out;
	out.connect(outputfile);
	//EC: Store Input File Readhead's position here.
	Tree.encodeHeader(out, characters);

	//Put the Readhead back at the stored value to start encoding
	input.clear();
	input.seekg(0, ios::beg);
	int index=0;

	//Grab each symbol and pass it to encode
	while(input.is_open()&&index < Tree.charCount){
			current = input.get();

			if (input.eof()) {
				break;
			}
			Tree.encode(current,out);
			index++;
		}
		out.close();

		return 0;
}


