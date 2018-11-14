/**
 * CPSC 362 VCS Project
 * Copyright(C) 2018 Team TBD
 * @author Josh Gomberg jgomberg93@gmail.com
 * @author Michael Li limichael1099419@gmail.com
 * @author Frank Ngo frank.ngo@csu.fullerton.edu
 * @author Wellson Pan dihydrogenmonoxide1337@gmail.com
 *
 * Header.h contains the libraries and functions used in main.cpp
**/

#pragma warning(disable : 4996)
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <chrono>
#include <ratio>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

/**
 * Find the total number of files within a folder and its sub-folders.
 *
 * @param string p path to the folder
 * @return int the size of that folder
**/
int directorySize(std::string p)
{
	int i = 0;
	for (fs::recursive_directory_iterator it(p); it != fs::recursive_directory_iterator(); ++it)
	{
		i++;
	}
	return i;
}

/**
 * Calculate the weighted checksum.
 *
 * @param char c the character to get the checksum from based on its ASCII value
 * @param int counter the index of char c within a file
 * @return int the weighted checksum
**/
int checksum(char c, int counter)
{
	int checkSum = 0;
	switch (counter % 5)
	{
	case 0:
		checkSum += (c * 1);
		break;
	case 1:
		checkSum += (c * 3);
		break;
	case 2:
		checkSum += (c * 7);
		break;
	case 3:
		checkSum += (c * 11);
		break;
	case 4:
		checkSum += (c * 17);
		break;
	}
	//prevent overflowing in case the checksum gets too large
	int m = (std::pow(2, 31)) - 1;
	checkSum = checkSum % m;
	return checkSum;
}

void createManifest(fs::path source)
{
	std::string manifest_path = source.parent_path().string() + "\\" + "Manifest";
	fs::path man(manifest_path);

	std::vector<fs::directory_entry> container(directorySize(source.string()));
	copy(fs::recursive_directory_iterator(source), fs::recursive_directory_iterator(), container.begin());

	//CREATING MANIFEST DIRECTORY
	if (!fs::create_directory(man.string()))
		std::cerr << "Manifest directory already exists.\n";

	//CREATING MANIFEST FILE
	std::chrono::duration<int, std::ratio<60 * 60 * 24> > one_day(1);
	std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
	time_t tt;
	tt = std::chrono::system_clock::to_time_t(today);
	std::string t = man.string() + "\\" + ctime(&tt);
	std::ofstream manifest;
	//if (!manifest(t))
		//std::cerr << "Couldn't open the file\n";

	for (int i = 0; i < container.size(); i++)
	{
		//std::string fullpath = source.string() +"\\" + container[i].path().relative_path().string() + "\\";
		manifest << container[i].path().relative_path().string() << std::endl;
	}
	manifest.close();
}

void pushToRepo()
{
	//TODO
}

void pullFromRepo()
{
	std::string line, label, manName;
	std::size_t pos;

	std::ifstream labelFileIn("labels.txt");

	std::cout << "What is the label or manifest name you would like to pull? ";
	std::cin >> label;

	while (labelFileIn.good()) {

		std::getline(labelFileIn, line);

		pos = line.find(label);

		if (pos != std::string::npos) {
			break;
		}
	}

	pos = line.find("\t");
	manName = line.substr(0, pos);

	std::cout << manName << std::endl;
}

void labelManifest()
{
	std::string manName, label;

	std::cout << "What is the name of the manifest file? ";
	std::cin >> manName;

	std::cout << "What label would you like to give the manifest file? ";
	std::cin >> label;

	std::ofstream labelFile("labels.txt", std::ios_base::app);

	labelFile << manName << "\t" << label << " \n";
}

std::vector<std::string> compareFiles(std::string source_f, std::string destination_f)
{
	std::ifstream source(source_f);
	std::ifstream destination(destination_f);
	std::string line;

	std::vector<std::string> src, dest;

	int i = 0, j = 0;

	do {
		std::getline(source, line);
		src.push_back(line);
	} while (source.good());

	do {
		std::getline(destination, line);
		dest.push_back(line);
	} while (destination.good());

	for (std::size_t a = 0; a < src.size(); a++)
	{
		for (std::size_t b = 0; b < dest.size(); b++)
		{
			if (dest[b] == src[a])
			{
				dest.erase(dest.begin() + b);
				std::cout << dest[b] << std::endl;
			}
		}
	}
	return dest;
}