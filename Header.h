// This program was made by Bannikov Nikita, group 9309 

#pragma once
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <stdexcept>
#include <bitset>
#include <list>

using namespace std;

typedef struct Character {
	Character() : value('\0'), count(0), relativeFreq(0) {}
	Character(unsigned char v, int c, float f, bool h) :
		value(v), count(c), relativeFreq(f), isHead(h) {}
	unsigned char value;
	int count;
	float relativeFreq;
	bool isHead;
}*_Character;

class CharacterVector
{
protected:
	ifstream inFile;
	vector<_Character> symbol_in_data;
	vector<_Character> sortedSymbolDescend;
	vector<_Character> sortedSymbolAscend;
	vector<unsigned char> rawData;
	void computeFreq() {
		for (int i = 1; i < symbol_in_data.size(); ++i)
			symbol_in_data[i]->relativeFreq = symbol_in_data[i]->count / float(this->getNumTotal());
	}
public:
	CharacterVector() { symbol_in_data.push_back(new Character('\0', 0, 0, true)); }
	int getNumUnique();
	int getNumTotal();

	// Load data from a file
	int loadRawDataFromFile(string fileName);

	void printFile();
	void findUnique();
	void listUnique();
	float computeEnt();

	// Sort characters (descending order)
	void sortSymbolDescend();
    // Sort characters (asceding order)
	void sortSymbolAscend();
	// Prints characters frequency to console (asceding order)
	void listSortedDescend();
	// Prints characters frequency to console (descending order)
	void listSortedAscend();
};

typedef struct treeNode {
	int index;
	unsigned char attribute;
	float relativeFreq;
	bool isLeaf;
	treeNode* left_child;
	treeNode* right_child;
	treeNode* parent;
	treeNode(int idx, unsigned char att, float freq, treeNode* left, treeNode* right, treeNode* prt, bool l) :
		index(idx), attribute(att), relativeFreq(freq),
		left_child(left), right_child(right), parent(prt), isLeaf(l) {}
	treeNode(int idx, unsigned char att, float freq, treeNode* left, treeNode* right, treeNode* prt) :
		index(idx), attribute(att), relativeFreq(freq),
		left_child(left), right_child(right), parent(prt) {}
	treeNode(int idx, unsigned char att, treeNode* left, treeNode* right, treeNode* prt) :
		index(idx), attribute(att), left_child(left), right_child(right), parent(prt) {}
	treeNode(unsigned char att, float freq, treeNode* left, treeNode* right, treeNode* prt) :
		attribute(att), relativeFreq(freq), left_child(left), right_child(right), parent(prt) {}
	treeNode(unsigned char att, float freq, bool l) : attribute(att), relativeFreq(freq), isLeaf(l) {}
}*Node;

class BinaryTree
{
protected:
	vector<Node> nodeVector;	// binary tree stored in vector
	Node nodeList;				// binary tree stored in list
public:
	BinaryTree() {
	}
	void splitNode(int);
	void assignValue(int, unsigned char, float);
	int getLength();
	void getNode(int);
	void printTree();
	Node mergeNode(Node&, Node&);
};

typedef struct ShannonFanoCodeWord {
	unsigned char symbol;
	int decIndex;			// code word index in decimal
	string binCode;			// a string representation of code word
	float relativeFreq;
	ShannonFanoCodeWord() : symbol('\0'), binCode("0"), relativeFreq(0) {}
}*_ShannonFanoCodeWord;

class ShannonFanoEncoder : public CharacterVector, public BinaryTree
{
protected:
	string dec2binEncode(int);
	int bin2dec(list<unsigned char>);
	ofstream outFile;
public:
	vector<_ShannonFanoCodeWord> CodesTable;
	ShannonFanoEncoder() {
		CodesTable.push_back(new ShannonFanoCodeWord);
		nodeVector.push_back(new treeNode(1, '\0', 1, 0, 0, 0));
	}
	void buildShannonFanoTree(int, int, float, int);
	void encodeShannonFano();

	// Print assigned binary codes
	void printCodes();

	// Encode data to a file
	int encodeToFile(string fileName);
};

class ShannonFanoDecoder : public ShannonFanoEncoder
{
private:
	ifstream readEncodedFile;
	string dec2binDecode(int n);
public:
	void decodeEncodedFile(string inFileName, string outFileName);
};

class Utils {
public:
	static bool compareFiles(const std::string& p1, const std::string& p2);
};