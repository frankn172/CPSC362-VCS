#pragma once
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

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
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

//get current Date and Time
std::string currentDateAndTime()
{
	time_t now = time(0);
	char* dt = ctime(&now);
	return dt;
}

void compareFiles()
{

}

void createManifest()
{
	//source 
	fs::path cwd(fs::current_path());

	//std::cout << "Current path is : " << source << std::endl;

	std::ofstream manifest;
	std::string t = currentDateAndTime();
	manifest.open(t);

	//put creation date in the file 
	manifest << t << std::endl;

	//all the files in the directory -> outputted into the manifest files. 
	std::vector<fs::directory_entry> container(directorySize(cwd));

	copy(fs::recursive_directory_iterator(from), fs::recursive_directory_iterator(), container.begin());

	for (int i = 0; i < container.size(); i++)
	{
		std::string fullpath = cwd + "\\" + container[i].path().relative_path().string() + "\\";
		manifest << fullpath << std::endl;
	}

	manifest.close();
}

void pushToRepo()
{
	//TODO
	//compare files -> see what changed -> add to repo

}

void pullFromRepo()
{
	//TODO
	//compare files -> see what's different -> add into cwd
	fs::path cwd(fs::current_path());

	std::vector<std::string> changed;

	//add to current directory changed files
	//create a manifest file ?
}

void labelManifest()
{
	//TODO
}