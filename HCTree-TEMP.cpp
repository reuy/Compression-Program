#include <queue>
#include <vector>
#include <fstream>
#include "HCTree.h"
#include "HCNode.h"
#include "BitInputStream.h"
#include "BitOutputStream.h"

using namespace std;

class HCTree;
/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */

/** A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void HCTree::build(const vector<int>& freqs){
    	if(freqs.empty())
    		return;
    	std::priority_queue<HCNode*> Forest;

    	//Fill Leaves with nodes of associated chars
    	for (char currByte = 0; currByte < freqs.size(); currByte++){
    		//ush it as a new node
    		this->leaves.push_back(new HCNode(freqs[currByte],currByte,0,0,0));
    	}

    	//Push all the nonzeroleaves to build the forest
    	for(HCNode* current : leaves){
    		if(current->count !=0)
    		Forest.push(current);
    	}

    	//Merge the two lowest nodes in the tree(as dictated by PQueue) until 1 remains
    	HCNode* First;
    	HCNode* Second;
    	while(Forest.size() < 1){
    		//Link the two lowest Nodes off the Queue
    		First=Forest.top();
    		Forest.pop();
    		Second=Forest.top();
    		Forest.pop();
    		HCNode* New = new HCNode(First->count+Second->count,0,First,Second,0);
    		First->p=New;
    		Second->p=New;
    		Forest.push(New);
    	}
    	this->root = Forest.top();



    };

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
   // void encode(byte symbol, BitOutputStream& out) const{};

    /** Write to the given ofstream
     *  the sequence of bits (as ASCII) coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
     *  BE USED IN THE FINAL SUBMISSION.
     */
    void HCTree::encode(byte symbol, ofstream& out) const{
    	//Use the tree to find the binary code the symbol belongs to
    	 string print;
    	 HCNode* current = leaves[symbol];
    	 HCNode* last = leaves[symbol];
    	 while(!current->p){
    		 current = current->p;
    		 if(last == current->c0)
    			 print+= '0';
    		 else if(last == current->c1)
    			 print += '1';
    		 else cout << "Error! \n";
    	 }

    	//REVERSE THE CODE
    	 int length = print.length();
    	 for(int index = 0; index <length/2; index++){
    		 swap(print[index],print[length-index-1]);
    	 };
    	//Print the code to out
    	out << print;
    };


    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    //int decode(BitInputStream& in) const{};

    /** Return the symbol coded in the next sequence of bits (represented as 
     *  ASCII text) from the ifstream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
     *  IN THE FINAL SUBMISSION.
     */
    int HCTree::decode(ifstream& in) const{
    	HCNode* current = root;
    	char input;
    	//Iterate down a tree until a childless node is reached
    	while(!current->c0 && !current->c1){
    		input = in.get();
    		if (input == '0')
    			current = current->c0;
    		else if (input == '1')
    			current = current->c1;
    		else {cout << "Error!\n"; return 0;}
    	}
    	return current->symbol;
    };


