#include <queue>
#include <vector>
#include <fstream>
#include "HCTree.h"
#include "HCNode.h"
#include "BitInputStream.cpp"
#include "BitOutputStream.cpp"

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs) {
	if (freqs.empty())
		return;

	int highestFreq = 0;
	for (int index = 0; index < freqs.size(); index++) {
		if (freqs[index] > highestFreq) {
			highestFreq = freqs[index];
		}
	}
	if (!highestFreq) {
		cout << "Invalid Vector!";
		charCount = 0;
		freqMode = 0;
		return;
	}
	std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> Forest;
	int max2 = freqs.size();
	//Fill Leaves with nodes of associated chars
	for (int currByte = 0; currByte < freqs.size(); currByte++) {
		int currVal = freqs[currByte];
		//push it as a new node
		this->leaves.push_back(new HCNode(currVal, currByte, 0, 0, 0));
	}

	int max = leaves.size();

	//Push all the nonzero leaves to build the forest
	for (int index = 0; index < leaves.size(); index++) {

		if (leaves[index]->count != 0)
			Forest.push(leaves[index]);
	}
	int FSize = Forest.size();
	if (Forest.empty()) {
		cout << "Error!";
		return;
	}

	//Merge the two lowest nodes in the tree(as dictated by PQueue) until 1 remains
	HCNode* First;
	HCNode* Second;
	while (Forest.size() > 1) {
		int Diag = Forest.size();
		//Link the two lowest Nodes off the Queue
		First = Forest.top();
		Forest.pop();
		Second = Forest.top();
		Forest.pop();
		HCNode* New = new HCNode(First->count + Second->count, 0, First, Second,
				0);
		First->p = New;
		Second->p = New;
		Forest.push(New);
	}
	this->root = Forest.top();
	charCount = root->count;

}
;

/*Encode the header using a completed 256 character vector, saving charCount, and freqMode.
 * THESE MUST BE SET BEFOREHAND.*/
void HCTree::encodeHeader(BitOutputStream& out, vector<int>& characters) {
	//FreqMode BITS
	out.put(freqMode & 2); //Encode the first bit
	out.put(freqMode & 1); //Encode the second bit

	//Freqs BITS
	//Open output stream and input header data
	for (unsigned int index = 0; index < characters.size(); index++) {
		if (characters[index] == 0) {
			out.put(false);
		} else if (freqMode == 0) {
			out.put(1);
			out.write(characters[index], 8);
		} else if (freqMode == 1) {
			if (characters[index] < 256) { //CHARACTER CODE: 10-8 Digits
				out.put(1);
				out.put(0);
				out.write(characters[index], 8);
			} else { //CHARACTER CODE: 11-16 Digits
				out.put(1);
				out.put(1);
				out.write(characters[index], 16);
			}
		} else if (freqMode == 2) {
			if (characters[index] < 256) { //CHARACTER CODE: 10-8 Digits
				out.put(1);
				out.put(0);
				out.write(characters[index], 8);
			} else if (characters[index] < 65536) { //CHARACTER CODE: 110-16 Digits
				out.put(1);
				out.put(1);
				out.put(0);
				out.write(characters[index], 16);
			} else { //CHARACTER CODE: 111-24 Digits
				out.put(1);
				out.put(1);
				out.put(1);
				out.write(characters[index], 24);
			}
		} else if (freqMode == 3) {
			if (characters[index] < 256) { //CHARACTER CODE: 10-8 Digits
				out.put(1);
				out.put(0);
				out.write(characters[index], 8);
			} else if (characters[index] < 65536) { //CHARACTER CODE: 110-16 Digits
				out.put(1);
				out.put(1);
				out.put(0);
				out.write(characters[index], 16);
			} else if (characters[index] < (1 << 17) + 1) { //CHARACTER CODE: 1110-24 Digits
				out.put(1);
				out.put(1);
				out.put(1);
				out.put(0);
				out.write(characters[index], 24);
			} else { //CHARACTER CODE: 1111-32 Digits
				out.put(1);
				out.put(1);
				out.put(1);
				out.put(1);
				out.write(characters[index], 32);
			}
		}
	}
}
;

/*Encode the header using a completed 256 character vector, saving charCount, and freqMode.
 * THESE MUST BE SET BEFOREHAND.*/
void HCTree::encodeHeader(BitOutputStream& out, vector<int>& characters,
		bool EC) {
	//FreqMode BITS
	//EC MODE: DON'T WRITE THE FIRST BIT
	if (!EC)
		out.put(freqMode & 2); //Encode the first bit

	out.put(freqMode & 1); //Encode the second bit

	//Freqs BITS
	//Open output stream and input header data
	for (unsigned int index = 0; index < characters.size(); index++) {
		if (characters[index] == 0) {
			out.put(false);
		} else if (freqMode == 0) {
			out.put(1);
			out.write(characters[index], 8);
		} else if (freqMode == 1) {
			if (characters[index] < 256) { //CHARACTER CODE: 10-8 Digits
				out.put(1);
				out.put(0);
				out.write(characters[index], 8);
			} else { //CHARACTER CODE: 11-16 Digits
				out.put(1);
				out.put(1);
				out.write(characters[index], 16);
			}
		} else if (freqMode == 2) {
			if (characters[index] < 256) { //CHARACTER CODE: 10-8 Digits
				out.put(1);
				out.put(0);
				out.write(characters[index], 8);
			} else if (characters[index] < 65536) { //CHARACTER CODE: 110-16 Digits
				out.put(1);
				out.put(1);
				out.put(0);
				out.write(characters[index], 16);
			} else { //CHARACTER CODE: 111-24 Digits
				out.put(1);
				out.put(1);
				out.put(1);
				out.write(characters[index], 24);
			}
		} else if (freqMode == 3) {
			if (characters[index] < 256) { //CHARACTER CODE: 10-8 Digits
				out.put(1);
				out.put(0);
				out.write(characters[index], 8);
			} else if (characters[index] < 65536) { //CHARACTER CODE: 110-16 Digits
				out.put(1);
				out.put(1);
				out.put(0);
				out.write(characters[index], 16);
			} else if (characters[index] < (1 << 17) + 1) { //CHARACTER CODE: 1110-24 Digits
				out.put(1);
				out.put(1);
				out.put(1);
				out.put(0);
				out.write(characters[index], 24);
			} else { //CHARACTER CODE: 1111-32 Digits
				out.put(1);
				out.put(1);
				out.put(1);
				out.put(1);
				out.write(characters[index], 32);
			}
		}
	}
}
;

/*Read the header, updating charcount, freqmode and an EMPTY 256 character vector*/
void HCTree::decodeHeader(BitInputStream& in, vector<int>& characters) {
	//FreqMode
	freqMode = 0;
	if (in.get()) {
		freqMode += 2;
	};
	if (in.get()) {
		freqMode += 1;
	};

	//Freqs
	for (unsigned int index = 0; index < characters.size(); index++) {
		bool input = in.get();

		if (!input) {
			characters[index] = 0;
		} else if (freqMode == 0) {
			int value = in.read(8);
			characters[index] = value;
			value = characters[index];
			//cout << value;
		} else if (freqMode == 1) {
			if (!in.get()) { //CHARACTER CODE: 10-8 Digits
				characters[index] = in.read(8);
			} else { //CHARACTER CODE: 11-16 Digits
				characters[index] = in.read(16);
			}
		} else if (freqMode == 2) {

			if (!in.get()) { //CHARACTER CODE: 10-8 Digits
				characters[index] = in.read(8);
			} else if (!in.get()) { //CHARACTER CODE: 110-16 Digits
				characters[index] = in.read(16);
			} else { //CHARACTER CODE: 111-24 Digits
				characters[index] = in.read(24);
			}

		} else if (freqMode == 3) {

			if (!in.get()) { //CHARACTER CODE: 10-8 Digits
				characters[index] = in.read(8);
			} else if (!in.get()) { //CHARACTER CODE: 110-16 Digits
				characters[index] = in.read(16);
			} else if (!in.get()) { //CHARACTER CODE: 1110-24 Digits
				characters[index] = in.read(24);
			} else { //CHARACTER CODE: 1111-32 Digits
				characters[index] = in.read(32);
			}

		}

	}

}
;

void HCTree::decodeHeader(BitInputStream& in, vector<int>& characters,
		bool EC) {
	//FreqMode
	freqMode = 0;
	//EC MODE: IGNORE THE FIRST BIT
	if (!EC) {
		if (in.get()) {
			freqMode += 2;
		};

	}
	if (in.get()) {
		freqMode += 1;
	};

	//Freqs
	for (unsigned int index = 0; index < characters.size(); index++) {
		bool input = in.get();

		if (!input) {
			characters[index] = 0;
		} else if (freqMode == 0) {
			int value = in.read(8);
			characters[index] = value;
			value = characters[index];
			//cout << value;
		} else if (freqMode == 1) {
			if (!in.get()) { //CHARACTER CODE: 10-8 Digits
				characters[index] = in.read(8);
			} else { //CHARACTER CODE: 11-16 Digits
				characters[index] = in.read(16);
			}
		} else if (freqMode == 2) {

			if (!in.get()) { //CHARACTER CODE: 10-8 Digits
				characters[index] = in.read(8);
			} else if (!in.get()) { //CHARACTER CODE: 110-16 Digits
				characters[index] = in.read(16);
			} else { //CHARACTER CODE: 111-24 Digits
				characters[index] = in.read(24);
			}

		} else if (freqMode == 3) {

			if (!in.get()) { //CHARACTER CODE: 10-8 Digits
				characters[index] = in.read(8);
			} else if (!in.get()) { //CHARACTER CODE: 110-16 Digits
				characters[index] = in.read(16);
			} else if (!in.get()) { //CHARACTER CODE: 1110-24 Digits
				characters[index] = in.read(24);
			} else { //CHARACTER CODE: 1111-32 Digits
				characters[index] = in.read(32);
			}

		}

	}

}
;
/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
	//Use the tree to find the binary code the symbol belongs to
	vector<bool> code;
	string print = "";
	HCNode* current = leaves[symbol];
	HCNode* last = leaves[symbol];
	while (current->p) {
		current = current->p;
		if (last == current->c0) {
			code.push_back(false);
			print.append("0");
		} else if (last == current->c1) {
			code.push_back(true);
			print.append("1");
		} else
			cout << "Error! \n";
		last = current;
	}
	/* NOT NECESSARY as code is read from end-to-front:
	 //reverse the code
	 int length = print.size();
	 for(int index = 0; index <length/2; index++){
	 swap(print[index],print[length-index-1]);
	 };*/
	//Print the code to out, with the root(most recent) being put first.
	for (int index = print.size() - 1; index >= 0; index--) {
		out.put(code[index]);
	};

}
;

/** Write to the given ofstream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT
 *  BE USED IN THE FINAL SUBMISSION.
 */
void HCTree::encode(byte symbol, ofstream& out) const {
	//Use the tree to find the binary code the symbol belongs to
	string print = "";
	HCNode* current = leaves[symbol];
	HCNode* last = leaves[symbol];
	while (current->p) {
		current = current->p;
		if (last == current->c0)
			print += '0';
		else if (last == current->c1)
			print += '1';
		else
			cout << "Error! \n";
		last = current;
	}
	//REVERSE THE CODE
	int length = print.length();
	for (int index = 0; index < length / 2; index++) {
		swap(print[index], print[length - index - 1]);
	};
	//Print the code to out
	out << print;
}
;

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
int HCTree::decode(BitInputStream& in) const {
	HCNode* current = root;
	bool input;

	//Iterate down a tree until a childless node is reached
	while (current->c0 && current->c1) {
		input = in.get();
		if (!input)
			current = current->c0;
		else
			current = current->c1;
	}
	return current->symbol;
}
;

/** Return the symbol coded in the next sequence of bits (represented as
 *  ASCII text) from the ifstream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
 *  IN THE FINAL SUBMISSION.
 */
int HCTree::decode(ifstream& in) const {
	HCNode* current = root;
	char input;

	//Iterate down a tree until a childless node is reached
	while (current->c0 && current->c1) {
		input = in.get();
		if (input == '0')
			current = current->c0;
		else if (input == '1')
			current = current->c1;
		else
			break;
	}
	return current->symbol;
}
;

