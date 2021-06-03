// This program was made by Bannikov Nikita, group 9309 
#include "pch.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include <windows.h>
#include "../4sem_2lab/Header.h"
#include "../4sem_2lab/Implementation.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ShannonFano
{
	TEST_CLASS(UnitTest)
	{
	public:
		UnitTest() {
			dencoder = new ShannonFanoDecoder;
		}

		~UnitTest() {
			delete dencoder;
		}
		
		TEST_METHOD(OriginalNumChars)
		{
			string fullPath = workingdir("test_data.txt");
			int originalFileSize = dencoder->loadRawDataFromFile("../test_data.txt");
			Assert::AreEqual(17, originalFileSize);
			int total = dencoder->getNumTotal();
			Assert::AreEqual(17, total);
		}

		TEST_METHOD(UniqNumChars)
		{
			dencoder->loadRawDataFromFile("../test_data.txt");
			dencoder->findUnique();
			int unique = dencoder->getNumUnique();
			Assert::AreEqual(8, unique);
		}

		TEST_METHOD(Entropy)
		{
			dencoder->loadRawDataFromFile("../test_data.txt");
			dencoder->findUnique();
			float entropy = dencoder->computeEnt();

			bool test_success = false;

			if (entropy > 2.7) {
				test_success = true;
			}
			Assert::AreEqual(true, test_success);
		}

		TEST_METHOD(EncodingRatio)
		{
			int originalFileSize = dencoder->loadRawDataFromFile("../test_data.txt");
			dencoder->findUnique();
			dencoder->sortSymbolDescend();
			dencoder->buildShannonFanoTree(0, dencoder->getNumUnique(), 1, 1);
			dencoder->encodeShannonFano();
			int encodedFileSize = dencoder->encodeToFile("../test_data_encoded.txt");

			bool test_success = false;
			if (encodedFileSize < originalFileSize) {
				test_success = true;
			}
			Assert::AreEqual(true, test_success);
		}

		TEST_METHOD(EncodingDecodeValidity)
		{
			int originalFileSize = dencoder->loadRawDataFromFile("../test_data.txt");
			dencoder->findUnique();
			dencoder->sortSymbolDescend();
			dencoder->buildShannonFanoTree(0, dencoder->getNumUnique(), 1, 1);
			dencoder->encodeShannonFano();
			int encodedFileSize = dencoder->encodeToFile("../test_data_encoded.txt");
			dencoder->decodeEncodedFile("../test_data_encoded.txt", "../test_data_decoded.txt");

			bool test_success = false;
			if (encodedFileSize < originalFileSize) {
				test_success = true;
			}
			Assert::AreEqual(true, test_success);
		}

		TEST_METHOD(OriginalDecodedValidity)
		{
			int originalFileSize = dencoder->loadRawDataFromFile("../test_data.txt");
			dencoder->findUnique();
			dencoder->sortSymbolDescend();
			dencoder->buildShannonFanoTree(0, dencoder->getNumUnique(), 1, 1);
			dencoder->encodeShannonFano();
			int encodedFileSize = dencoder->encodeToFile("../test_data_encoded.txt");
			dencoder->decodeEncodedFile("../test_data_encoded.txt", "../test_data_decoded.txt");

			bool test_success = false;
			if (Utils::compareFiles("../test_data.txt", "../test_data_decoded.txt") == true) {
				test_success = true;
			}
			Assert::AreEqual(true, test_success);
		}

	private:
		std::string workingdir(const string& file_name)
		{
			char buf[256];
			GetCurrentDirectoryA(2048, buf);
			return std::string(buf) + '\\' + file_name;
		}

		ShannonFanoDecoder* dencoder;
	};
}
