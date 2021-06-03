// This program was made by Bannikov Nikita, group 9309 
#include "Header.h"

void sd_test(const char* input_file_name, const char* encoded_file_name, const char* decoded_file_name) {
	ShannonFanoDecoder* dencoder = new ShannonFanoDecoder;
	
	// Load original data
	cout << "Loading data from file: " << input_file_name << endl;
	int originalFileSize = dencoder->loadRawDataFromFile(input_file_name);

	// Calculate unique
	dencoder->findUnique();

	cout << " Characters number: " << dencoder->getNumTotal() << endl;
	cout << " Bits/character: " << dencoder->computeEnt() << endl;
	cout << " Original FileSize: " << originalFileSize << endl;
	
	cout << " Characters frequency:" << endl;
	dencoder->sortSymbolDescend();
	dencoder->listSortedDescend();

	cout << " Unique characters number: " << dencoder->getNumUnique() << endl;
	dencoder->buildShannonFanoTree(0, dencoder->getNumUnique(), 1, 1);
	dencoder->encodeShannonFano();

	// Print assigned binary codes
	cout << " Binary codes:" << endl;
	dencoder->printCodes();

	cout << endl << " Encoded data in file: " << encoded_file_name << endl;
	int encodedFileSize = dencoder->encodeToFile(encoded_file_name);
	cout << " Encoded FileSize: " << encodedFileSize << endl;
	cout << " Encoded/Original ratio: " << (float)encodedFileSize/(float)originalFileSize << endl;

	cout << " Decode encoded data to file: " << decoded_file_name << endl;
	dencoder->decodeEncodedFile(encoded_file_name, decoded_file_name);
	cout << endl;

	if (Utils::compareFiles(input_file_name, decoded_file_name) == false) {
		cout << "ERROR: Encoded/Decoded file is not equal to original" << endl;
	}

	delete dencoder;
}

void manual_input() {
	cout << "Enter test phrase: ";
	string input;
	getline(cin, input);
	cout << "Your test phrase: " << input;

	std::ofstream out("user_data.txt");
	out << input;
	out.flush();
	out.close();

	cout << endl;
	sd_test("user_data.txt", "user_data_encoded.txt", "user_data_decoded.txt");
}

int main() {
	sd_test("data_one.txt", "data_one_encoded.txt", "data_one_decoded.txt");
	sd_test("data_second.txt", "data_second_encoded.txt", "data_second_decoded.txt");
	sd_test("data_third.txt", "data_third_encoded.txt", "data_third_decoded.txt");
	manual_input();
}
