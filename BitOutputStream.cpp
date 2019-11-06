#ifndef BITOUTPUTSTREAM_H_
#define BITOUTPUTSTREAM_H_


/*
 * BitOutputStream.cpp
 * Author: saito
 * Date:  May 18, 2018
 * About: 
 */

#include <fstream>
#include <iostream>
using namespace std;

class BitOutputStream {
private:
	unsigned char buffer=0;
	char current=7;
	ofstream* output;


public:
	//insert the next bit into the buffer. Flush write if full
	void put(bool bit){

/*		if(!bit) {
			cout << "0";
		} else {
			cout << "1";
		}*/

		if(current <= -1){
			flush();
		}
		buffer |= (bit << current);

		current--;
	};

	/*Prorate unnecessary bytes, and put them bit-by-bit*/
	void write(unsigned int value){
		char size = 1;
		if(value >= 1 << 8)
			size++;
		if(value >= 1 << 16)
			size++;
		if (value >= 1 << 24)
			size++;

		for (int index = (size * 8) - 1; index >= 0;index--){
			//Shift each bit such that it is in the 1 position to AND it.
			put((value >> index) & 1);
		}
	};

	//MANUAL SIZING VERSION
	void write(unsigned int value, char size){

		for (int index = size - 1; index >= 0;index--){
			//Shift each bit such that it is in the 1 position to AND it.
			bool bit = (value >> index) & 1;
			put(bit);
		}
	};

	//Get a new set of bits and reset the buffer
	void flush(){
		output->put(buffer);
		buffer = 0;
		current = 7;
	};

	void connect(ofstream& stream){
		output = &stream;
	};

	//Wrapper to relay close() command. FLUSH THE BUFFER.
	void close(){
		if(current >= -1){
			flush();
		}
		output->close();
	};
	//Wrapper to relay close() command. DO NOT Flush the buffer.
	void ForceClose(){

		output->close();
	};

};

#endif /* BITOUTPUTSTREAM_H_ */

