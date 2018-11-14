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
#include <boost/range.hpp>
namespace fs = boost::filesystem;

int prompt()
{
	int input;
	std::cout << "What would you like to do?" << std::endl;
	std::cout << "1. Create repo" << std::endl;
	std::cout << "2. Push to repo" << std::endl;
	std::cout << "3. Pull from repo" << std::endl;
	std::cout << "4. Label manifest" << std::endl;
	std::cout << "5. Exit" << std::endl;
	std::cout << "Enter the number corresponding with the command: ";
	std::cin >> input;
	std::cin.ignore(256, '\n');
	return input;
}

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

std::vector<std::string> compareFiles(std::vector<std::string> current, fs::path file)
{
	std::vector<std::string> changed;
	std::ifstream latest_manifest(file.string());
	std::string line;
	int i = 0;

	do 
	{
		std::getline(latest_manifest, line);
		changed.push_back(line);
		for (std::size_t j = 0; j < current.size(); j++)
		{
			if (current[j] == changed[i])
				changed.erase(current.begin() + i);
		}
		i++;
	} while (latest_manifest.good());
	return changed;
}

fs::path MostRecentManifest(fs::path man_dir)
{
	fs::path latest_manifest;
	std::time_t latest_tm{};

	for (auto&& entry : boost::make_iterator_range(fs::directory_iterator(man_dir), {})) 
	{
		fs::path p = entry.path();
		if (is_regular_file(p) && p.extension() == ".txt")
		{
			std::time_t timestamp = fs::last_write_time(p);
			if (timestamp > latest_tm) 
			{
				latest_manifest = p;
				latest_tm = timestamp;
			}
		}
	}
	if (latest_manifest.empty())
		std::cout << "Nothing found\n";
	else
		return latest_manifest;
}

void createManifest(fs::path source)
{
	std::string manifest_path = source.parent_path().string() + "\\" + "Manifest";
	std::chrono::duration<int, std::ratio<60 * 60 * 24> > one_day(1);
	std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
	time_t tt;
	tt = std::chrono::system_clock::to_time_t(today);
	std::string currentTime = ctime(&tt);
	std::string t = manifest_path + "\\" + currentTime;

	std::vector<fs::directory_entry> container(directorySize(source.string()));
	copy(fs::recursive_directory_iterator(source), fs::recursive_directory_iterator(), container.begin());


	//CREATING MANIFEST DIRECTORY
	if (fs::create_directory(manifest_path))
	{
		std::cerr << "Manifest directory already exists.\n";

		//CREATING MANIFEST FILE
		std::ofstream manifest;
		if (manifest.fail())
			std::cerr << "Couldn't open the file\n";
		tt = std::chrono::system_clock::to_time_t(today);
		std::string currentTime = ctime(&tt);
		manifest << currentTime << std::endl;
		for (int i = 0; i < container.size(); i++)
		{
			//std::string fullpath = source.string() +"\\" + container[i].path().relative_path().string() + "\\";
			manifest << container[i].path().relative_path().string() << "\t" << currentTime << std::endl;
		}

		manifest.close();
	}
	else
	{
		std::ofstream manifest;
		fs::path man(manifest_path);
		fs::path recent(MostRecentManifest(man).string());
		std::vector<std::string> vec;
		for (int i = 0; i < container.size(); i++)
		{
			vec.push_back(container[i].path().relative_path().string());
		}
		vec = compareFiles(vec, source.string()); //compare files function is not complete
		for (int i = 0; i < vec.size(); i++)
		{
			manifest << vec[i] << "\t" << currentTime << std::endl;
		}
		manifest.close();
	}
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

	while (labelFileIn.good()) 
	{

		std::getline(labelFileIn, line);
		pos = line.find(label);
		if (pos != std::string::npos)
			break;
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