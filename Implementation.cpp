// This program was made by Bannikov Nikita, group 9309 

#include "Header.h"
#include<iostream>
#include<algorithm>
#include<cmath>

bool compareByCountDescend(const _Character& sym_a, const _Character& sym_b)
{
	return (sym_a->count > sym_b->count);
}

bool compareByCountAscend(const _Character& sym_a, const _Character& sym_b)
{
	return (sym_a->count < sym_b->count);
}

static std::ifstream::pos_type filesize(string filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

int CharacterVector::loadRawDataFromFile(string fileName)
{
	inFile.open(fileName, ios::binary);
	unsigned char temp;
	while (inFile >> noskipws >> temp) {
		rawData.push_back(temp);
	}
	inFile.close();
	return filesize(fileName);
}

void CharacterVector::printFile()
{
	for (int i = 0; i < rawData.size(); ++i)
		cout << rawData[i];
	cout << endl;
}

void CharacterVector::findUnique()
{
	for (long i = 0; i < rawData.size(); ++i) {
		int j;
		for (j = 0; j < symbol_in_data.size(); ++j) {
			if (symbol_in_data[j]->value == rawData[i] && !symbol_in_data[j]->isHead) {
				++symbol_in_data[j]->count;
				break;
			}
		}
		if (j == symbol_in_data.size()) {
			symbol_in_data.push_back(new Character(rawData[i], 1, 0, false));
		}
	}

	computeFreq();
}

void CharacterVector::listUnique()
{
	for (int i = 1; i < symbol_in_data.size(); ++i) {
		cout << symbol_in_data[i]->value << '\t'
			<< symbol_in_data[i]->count << '\t'
			<< symbol_in_data[i]->relativeFreq << endl;
	}
}

int CharacterVector::getNumUnique()
{
	return symbol_in_data.size() - 1;
}

int CharacterVector::getNumTotal()
{
	return rawData.size();
}

float CharacterVector::computeEnt()
{
	float entropy = 0;
	for (int i = 1; i < symbol_in_data.size(); ++i) {
		entropy -= symbol_in_data[i]->relativeFreq * log2(symbol_in_data[i]->relativeFreq);
	}
	return entropy;
}

void CharacterVector::sortSymbolDescend()
{
	sortedSymbolDescend.assign(symbol_in_data.begin(), symbol_in_data.end());
	sort(sortedSymbolDescend.begin() + 1, sortedSymbolDescend.end(), compareByCountDescend);
}

void CharacterVector::sortSymbolAscend()
{
	sortedSymbolAscend.assign(symbol_in_data.begin(), symbol_in_data.end());
	sort(sortedSymbolAscend.begin() + 1, sortedSymbolAscend.end(), compareByCountAscend);
}

// Prints characters frequency to console
void CharacterVector::listSortedDescend()
{
	for (int i = 1; i < sortedSymbolDescend.size(); ++i) {
		cout << "  " << sortedSymbolDescend[i]->value << '\t'
			<< sortedSymbolDescend[i]->count << '\t'
			<< sortedSymbolDescend[i]->relativeFreq << endl;
	}
}

// Prints characters frequency to console
void CharacterVector::listSortedAscend()
{
	for (int i = 1; i < sortedSymbolAscend.size(); ++i) {
		cout << "  "  << sortedSymbolAscend[i]->value << '\t'
			<< sortedSymbolAscend[i]->count << '\t'
			<< sortedSymbolAscend[i]->relativeFreq << endl;
	}
}

void BinaryTree::splitNode(int n)
{
	for (int i = nodeVector.size() + 1; i <= 2 * n + 1; ++i) {
		nodeVector.push_back(new treeNode(i, '\0', 0, 0, 0));
	}
	for (int i = 0; i <= n - 1; ++i) {
		nodeVector[i]->left_child = nodeVector[2 * i + 1];
		nodeVector[i]->right_child = nodeVector[2 * i + 2];
	}
}

void BinaryTree::assignValue(int n, unsigned char att, float freq)
{
	nodeVector[n - 1]->attribute = att;
	nodeVector[n - 1]->relativeFreq = freq;
}

int BinaryTree::getLength()
{
	return nodeVector.size();
}

void BinaryTree::getNode(int n)
{
	cout << nodeVector[n - 1]->index << '\t' << nodeVector[n - 1]->attribute <<
		'\t' << nodeVector[n - 1]->relativeFreq << endl;
}

void BinaryTree::printTree()
{
	for (int i = 0; i < nodeVector.size(); ++i) {
		if (nodeVector[i]->attribute != '\0') {
			cout << nodeVector[i]->index << '\t' << nodeVector[i]->attribute
				<< '\t' << nodeVector[i]->relativeFreq << endl;
		}
	}
}

Node BinaryTree::mergeNode(Node& node_a, Node& node_b)
{
	Node newParent = new treeNode(0, '\0', node_a->relativeFreq + node_b->relativeFreq,
		node_a, node_b, 0, false);
	node_a->parent = newParent;
	node_b->parent = newParent;
	return newParent;
}

string ShannonFanoEncoder::dec2binEncode(int n)
{
	vector<bool> binNum;
	while (n / 2 != 0) {
		binNum.push_back(n % 2);
		n /= 2;
	}
	binNum.push_back(n % 2);

	string bitString;
	for (int i = binNum.size(); i > 0; --i) {
		if (binNum[i - 1])
			bitString.push_back('1');
		else
			bitString.push_back('0');
	}

	return bitString;
}

int ShannonFanoEncoder::bin2dec(list<unsigned char> buffer)
{
	int symbolDec = 0, countDigit = buffer.size() - 1;
	for (list<unsigned char>::iterator i = buffer.begin(); i != buffer.end(); ++i) {
		symbolDec += int((*i) == '1') * pow(2, countDigit--);
	}

	return symbolDec;
}

void ShannonFanoEncoder::buildShannonFanoTree(int start, int end, float maxFreq, int currentNode)
{
	if (start >= end)
		cerr << "Error! No symbol input." << endl;
	else if (start == end - 1) {
		assignValue(currentNode, sortedSymbolDescend[end]->value, sortedSymbolDescend[end]->relativeFreq);
	}
	else {
		splitNode(currentNode);

		float sumFreq = 0;
		int i = start, separateIdx;
		while (sumFreq < maxFreq / 2 && i < end - 1) {
			sumFreq += sortedSymbolDescend[i + 1]->relativeFreq;
			++i;
		}

		if ((sumFreq < maxFreq / 2) ||
			((sumFreq - maxFreq / 2) < (maxFreq / 2 - sumFreq + sortedSymbolDescend[i]->relativeFreq))) {
			separateIdx = i;
		}
		else {
			separateIdx = i - 1;
			sumFreq -= sortedSymbolDescend[i]->relativeFreq;
		}

		buildShannonFanoTree(start, separateIdx, sumFreq, 2 * currentNode);
		buildShannonFanoTree(separateIdx, end, maxFreq - sumFreq, 2 * currentNode + 1);
	}
}

void ShannonFanoEncoder::encodeShannonFano()
{
	for (int i = 1; i < nodeVector.size(); ++i) {
		if (nodeVector[i]->relativeFreq > 0 && nodeVector[i]->relativeFreq < 1) {
			CodesTable.push_back(new ShannonFanoCodeWord);
			CodesTable.back()->symbol = nodeVector[i]->attribute;
			CodesTable.back()->relativeFreq = nodeVector[i]->relativeFreq;
			CodesTable.back()->decIndex = nodeVector[i]->index;
			CodesTable.back()->binCode = dec2binEncode(nodeVector[i]->index).erase(0, 1);
		}
	}
}

// Print assigned binary codes
void ShannonFanoEncoder::printCodes()
{
	for (int i = 1; i < CodesTable.size(); ++i) {
		cout << "  " << i << '\t' << CodesTable[i]->symbol <<
			'\t' << CodesTable[i]->binCode << endl;
	}
}

int ShannonFanoEncoder::encodeToFile(string fileName)
{
	outFile.open(fileName, ios::binary);
	string tempEncodedString;
	list<unsigned char> outputBuffer;
	for (long i = 0; i < rawData.size(); ++i) {
		for (int j = 0; j < CodesTable.size(); ++j) {
			if (rawData[i] == CodesTable[j]->symbol) {
				tempEncodedString.append(CodesTable[j]->binCode);
			}
		}
	}

	for (long i = 0; i < tempEncodedString.size(); ++i) {
		if (i == 0) {
			outputBuffer.push_back(tempEncodedString[i]);
		}
		else if (i == tempEncodedString.size() - 1 && i % 8 != 0) {
			outputBuffer.push_back(tempEncodedString[i]);
			outFile << unsigned char(bin2dec(outputBuffer));
		}
		else if (i % 8 == 0 && i != 0) {
			outFile << unsigned char(bin2dec(outputBuffer));
			outputBuffer.clear();
			outputBuffer.push_back(tempEncodedString[i]);
		}
		else {
			outputBuffer.push_back(tempEncodedString[i]);
		}
	}

	outFile.close();

	return filesize(fileName);
}

string ShannonFanoDecoder::dec2binDecode(int n)
{
	vector<bool> binNum;
	while (n / 2 != 0) {
		binNum.push_back(n % 2);
		n /= 2;
	}
	binNum.push_back(n % 2);

	if (binNum.size() < 8) {
		for (int i = binNum.size(); i < 8; ++i)
			binNum.push_back(0);
	}

	string bitString;
	for (int i = binNum.size(); i > 0; --i) {
		if (binNum[i - 1])
			bitString.push_back('1');
		else
			bitString.push_back('0');
	}

	return bitString;
}

void ShannonFanoDecoder::decodeEncodedFile(string inFileName, string outFileName)
{
	readEncodedFile.open(inFileName, ios::binary);
	string tempDecodedString, decodeBuffer;
	vector<unsigned char> tempCharVector;
	unsigned char readNext;
	while (readEncodedFile >> noskipws >> readNext) {
		tempCharVector.push_back(readNext);
	}
	readEncodedFile.close();

	for (int i = 0; i < tempCharVector.size(); ++i) {
		if (i != tempCharVector.size() - 1) {
			tempDecodedString.append(dec2binDecode(int(tempCharVector[i])));
		}
		else
			tempDecodedString.append(dec2binEncode(int(tempCharVector[i])));
	}

	outFile.open(outFileName, ios::binary);
	for (int i = 0; i < tempDecodedString.size(); ++i) {
		decodeBuffer.push_back(tempDecodedString[i]);
		for (int j = 1; j < CodesTable.size(); ++j) {
			if (CodesTable[j]->binCode == decodeBuffer) {
				outFile << CodesTable[j]->symbol;
				decodeBuffer.clear();
				break;
			}
		}
	}
	outFile.close();
}

// Compare files
bool Utils::compareFiles(const std::string& p1, const std::string& p2) {
	std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
	std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

	if (f1.fail() || f2.fail()) {
		return false;
	}

	if (f1.tellg() != f2.tellg()) {
		return false;
	}

	f1.seekg(0, std::ifstream::beg);
	f2.seekg(0, std::ifstream::beg);
	return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::istreambuf_iterator<char>(f2.rdbuf()));
}