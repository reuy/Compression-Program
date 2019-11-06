#ifndef BITINPUTSTREAM
#define BITINPUTSTREAM
/*
 * BitInputStream.cpp
 * Author: saito
 * Date:  May 17, 2018
 * About: 
 */

#include <fstream>
#include <iostream>
using namespace std;

class BitInputStream {
private:
	char buffer=0;
	char current=-1;
	ifstream* input;


public:
	//Return eof if the input stream ever threw eof, and
	bool eof(){
		return (input->eof() && current == 0);
	};
	//Retrieve the next bit from the buffer
	bool get(){
		bool returnval;
		if(current < 0)
			flush();
		//Create a bitmask by shifting current, and AND it with the buffer
		//If the resulting number >0, return true
		returnval = buffer & (1<<current);
/*		if(!returnval) {
			cout << "0";
		} else {
			cout << "1";
		}*/
		current--;
		return returnval;
	};

	/*Called with the knowledge the next size bits(!) are expected to represent a number
	 * Reads them and returns the stored value. Primarily used in reading the header.*/
	unsigned int read(char size){
		string bitStream = "";
		int value = 0;
		//Get each value and OR it to its appropriate location
		for (int index = size-1; index >= 0; index--){
			bool insert = get();
			if(insert){
				bitStream.append("1");
			} else {bitStream.append("0");}
			value |= insert << index;
		}
		return value;
	};
	void connect(ifstream& stream){
		input = &stream;
	};

	//Get a new set of bits and reset the buffer
	void flush(){
		input->get(buffer);
		current = 7;
	};

	//Wrapper to relay close() command
	void close(){
		input->close();
	};

};

#endif // BITINPUTSTREAM



