#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<assert.h>
using namespace std;

void checksum(string filename)
{
	vector<char> v;
	int sum = 0;
	size_t fileSize;
	string outFileName;
	fstream inFile, outFile;
	inFile.open(filename, ios::in);
	assert(inFile.is_open());

	//read file content into vector<char> v
	if (!inFile.eof() && !inFile.fail())
	{
		inFile.seekg(0, ios_base::end);
		streampos fileSize = inFile.tellg();
		v.resize(fileSize);
		inFile.seekg(0, ios_base::beg);
		inFile.read(&v[0], fileSize);
	}
	inFile.close();

	//calculate the weighted checksum
	for (int i = 0; i < v.size(); ++i)
	{
		switch (i % 5)
		{
		case 0:
			sum += (int)v[i] * 1;
			break;
		case 1:
			sum += (int)v[i] * 3;
			break;
		case 2:
			sum += (int)v[i] * 7;
			break;
		case 3:
			sum += (int)v[i] * 11;
			break;
		case 4:
			sum += (int)v[i] * 17;
			break;
		}
	}
	sum = sum % (2 ^ 31 - 1);

	outFileName = to_string(sum) + "-L" + to_string(v.size);
}