#pragma once
#ifndef HEADER_H
#define HEADER_H

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

void createManifest(fs::path source)
{
	fs::path parent = source.parent_path();	//parent folder 
	
	fs::path cwd(fs::current_path());	//initial position 

	//inside the parent folder 
	fs::current_path(parent);
	if (!fs::create_directory("Manifest"))
	{
		//compare files 

	}
	else
	{	//create a new Manifest files 

		//inside the Manifest folder 
		std::string m = parent.string() + "Manifest";
		fs::current_path(m);

		std::ofstream manifest;
		std::string t = currentDateAndTime() + ".txt";
		manifest.open(t);

		//all the files in the directory -> outputted into the manifest files. 
		std::vector<fs::directory_entry> container(directorySize(source.string()));

		copy(fs::recursive_directory_iterator(source), fs::recursive_directory_iterator(), container.begin());

		for (int i = 0; i < container.size(); i++)
		{
			//std::string fullpath = source.string() +"\\" + container[i].path().relative_path().string() + "\\";
			//manifest << fullpath << std::endl;

			manifest << container[i].path().relative_path().string() << std::endl;
		}

		manifest.close();
	}
}

void pushToRepo()
{
	//TODO
	//compare files -> see what changed -> add to repo

}

void pullFromRepo(fs::path source, fs::path dest)
{
	//TODO
	if (!fs::exists("Manifest"))
	{
		createRepo();
	}
	else
	{
		std::ifstream manifest1;
		std::ifstream manifest2;

		manifest1.open("");//first manifest file 
		manifest2.open("");//second
	}

}

void labelManifest()
{
	//TODO
}





#endif