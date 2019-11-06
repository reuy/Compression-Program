#ifndef HCTREE_H
#define HCTREE_H

#include <queue>
#include <vector>
#include <fstream>
#include "HCNode.h"
#include "BitInputStream.cpp"
#include "BitOutputStream.cpp"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return lhs->count > rhs->count;
    }
};

/** A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
    HCNode* root;
    vector<HCNode*> leaves;


public:
    int charCount; //Number of characters remaining before end/next header. Used differently by compress and decompress.
    char freqMode; //Informs which header writing/Reading algorithm to use
    /*
     *
//Signifies that the highest freq won't have a value above 255
FREQUENCIES(00): MODE 0
0 Case: 0: 1 bit
1-255 Case: 1-00000001: 9 bits

//Signifies that the highest freq won't have a value above 63555
FREQUENCIES(01): MODE 1
0 Case: 0: 1 bit
1-255 Case: 10-00000001: 10 bits
256-63555 Case: 11-00000001-00000001: 18 bits


//Signifies that the highest freq won't have a value above 4294967295 - NOT APPLICABLE IN EC
FREQUENCIES(10): MODE 2
0 Case: 0: 1 bit
1-255 Case: 10-00000001: 10 bits
256-63555 Case: 110-00000001-00000001: 19 bits
63556-4294967295 Case: 1110-00000001-00000000-10000001: 28 bits

//Signifies that any bit configuration is fair game - NOT APPLICABLE IN EC
FREQUENCIES(11): MODE 3
0 Case: 0: 1 bit
1-255 Case: 10-00000001: 10 bits
256-63555 Case: 110-00000001-00000001: 19 bits
63556-4294967295 Case: 1110-00000001-00000000-00000001: 28 bits
63556-INT-MAX-1 Case: 11110-00000001-00000001-0000000-100000001: 37 bits

*/
    // explicit keyword is used to avoid accidental implicit conversions
    explicit HCTree() : root(0) {
    	charCount = 0;
    	freqMode = -1;

        leaves = vector<HCNode*>();
    };



    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i. charCount and freqMode are set at this stage.
     */
    void build(const vector<int>& freqs);

    /*Encode the header using completed leaves, saving charCount, and FreqMode.
     * THESE MUST BE SET BY BUILD.*/
    void encodeHeader(BitOutputStream& out, vector<int>& characters);

    void encodeHeader(BitOutputStream& out, vector<int>& characters, bool EC);

    /*Read the header, updating charcount, freqMode, and leaves */
    void decodeHeader(BitInputStream& in,vector<int>& characters);

    void decodeHeader(BitInputStream& in,vector<int>& characters, bool EC);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /** Write to the given ofstream
     *  the sequence of bits (as ASCII) coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT 
     *  BE USED IN THE FINAL SUBMISSION.
     */
    void encode(byte symbol, ofstream& out) const;


    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in) const;

    /** Return the symbol coded in the next sequence of bits (represented as 
     *  ASCII text) from the ifstream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
     *  IN THE FINAL SUBMISSION.
     */
    int decode(ifstream& in) const;


    ~HCTree(){
    	if(this->root)
    		this->root->deleteNode();

    };
};

#endif // HCTREE_H
