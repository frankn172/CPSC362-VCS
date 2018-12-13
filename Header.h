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

void push_or_pull(std::string, std::string);

void removeSubstrs(std::string& destination, std::string source) {
	std::string::size_type n = source.length();
	for (std::string::size_type i = destination.find(source);
		i != std::string::npos;
		i = destination.find(source))
		destination.erase(i, n);
}

/**
* Show the menu prompt and ask for user's input
*
* @return int user's input
**/
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

/*
* Get the current time from system's clock
*
* @return string the current time
**/
std::string getCurrentTime()
{
	std::chrono::duration<int, std::ratio<60 * 60 * 24> > one_day(1);
	std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
	time_t tt;
	tt = std::chrono::system_clock::to_time_t(today);
	std::string currentTime = ctime(&tt);
	std::string currentTimeFixed = "";
	for (int i = 0; i < currentTime.length() - 1; i++)
	{
		if (currentTime[i] == ' ')
		{
			currentTimeFixed += "_";
		}
		else if (currentTime[i] == ':')
		{
			currentTimeFixed += "-";
		}
		else
		{
			currentTimeFixed += currentTime[i];
		}
	}

	return currentTimeFixed;
}

/**
* Compares the file contents to see what has changed
*
* @param string destination path to destination folder
* @param string source path to source folder
*
* @return vector<string> changed elements
**/
std::vector<std::string> compareFiles(std::vector<std::string> destination, std::string source)
{
	std::ifstream latest_manifest(source);
	std::string line;
	std::vector<std::string> changed, changed1, dest(destination);
	std::string time = getCurrentTime();
	std::vector<bool> isNew(destination.size(), true);

	do
	{
		std::getline(latest_manifest, line);
		if (!line.empty())
		{
			changed1.push_back(line);
			std::size_t pos = line.find("\t");
			std::string filename = line.substr(0, pos);
			changed.push_back(filename);
			//std::cout << "Repo: " << line << std::endl;
			//std::cout << "stuff: " << filename << std::endl;
		}
	} while (latest_manifest.good());

	for (std::size_t a = 0; a < dest.size(); a++)
	{
		for (std::size_t b = 0; b < changed.size(); b++)
		{
			std::size_t pos = dest[a].find("\t");
			std::string filename = dest[a].substr(0, pos);

			if (filename == changed[b])
			{
				//std::cout << "Unchanged: " << changed[b] << std::endl;
				changed.erase(changed.begin() + b);
				isNew[a] = false;
			}
		}
	}
	for (int a = 0; a < changed.size(); a++)
	{
		for (int b = 0; b < changed1.size(); b++)
		{
			std::size_t pos = changed1[b].find("\t");
			std::string filename = changed1[b].substr(0, pos);

			if (filename == changed[a])
			{
				changed1[b] = changed[a] + "\t" + time;
				//std::cout << "Changed: " << changed1[b] << std::endl;
			}
		}
	}

	for (std::size_t a = 0; a < destination.size(); a++)
	{
		if (isNew[a])
		{
			//std::cout << "New: " << destination[a] << std::endl;
			changed1.push_back(destination[a] + "\t" + time);
		}
	}

	return changed1;
}


/**
* Compares the file names to see what has changed
*
* @param string destination_f filename in destination folder
* @param string source_f filename in source folder
*
* @return vector<string> changed elements
**/
std::vector<std::string> compareFiles(std::string destination_f, std::string source_f)
{
	std::ifstream source(source_f);
	std::ifstream destination(destination_f);
	std::string line;

	std::vector<std::string> src, dest;

	do {
		std::getline(source, line);
		std::size_t pos = line.find("\t");
		std::string filename = line.substr(0, pos);
		src.push_back(filename);
	} while (source.good());

	do {
		std::getline(destination, line);
		std::size_t pos = line.find("\t");
		std::string filename = line.substr(0, pos);
		dest.push_back(filename);
	} while (destination.good());

	for (std::size_t a = 0; a < src.size(); a++)
	{
		for (std::size_t b = 0; b < dest.size(); b++)
		{
			if (dest[b] == src[a])
			{
				src.erase(src.begin() + a);
			}
		}
	}

	return src;
}

/**
* Get the path to the most recent manifest
*
* @param path man_dir path to the manifest folder
*
* @return path path to the most recent manifest
**/
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

	return latest_manifest;
}

/**
* Create the manifest file
*
* @param path source path to the source folder
**/
void createManifest(fs::path source)
{
	std::string manifest_path = source.string() + "\\Manifest\\";
	std::string currentTime = getCurrentTime();
	std::string t = manifest_path + currentTime + ".txt";

	std::vector<fs::directory_entry> container(directorySize(source.string()));
	copy(fs::recursive_directory_iterator(source), fs::recursive_directory_iterator(), container.begin());

	//CREATING MANIFEST DIRECTORY
	if (fs::create_directory(manifest_path))
	{

		//CREATING MANIFEST FILE
		std::ofstream manifest(t);
		if (manifest.fail())
			std::cerr << "Couldn't open the file\n";

		for (int i = 0; i < container.size(); i++)
		{
			std::string s = container[i].path().string();
			removeSubstrs(s, source.parent_path().string());
			manifest << s << "\t" << currentTime << std::endl;
		}
		manifest.close();
	}

	else
	{
		std::cerr << "Manifest already exists\n";

		fs::path man(manifest_path);
		fs::path recent(MostRecentManifest(man));
		std::vector<std::string> vec;



		for (int i = 0; i < container.size(); i++)
		{
			std::string s = container[i].path().string();
			removeSubstrs(s, source.parent_path().string());
			vec.push_back(s);
		}

		vec = compareFiles(vec, recent.string());

		std::ofstream manifest(t);

		for (int i = 0; i < vec.size(); i++)
		{
			manifest << vec[i] << std::endl;
		}

		manifest.close();
	}
}

/**
* Push to the Repository
Parameters : source directory and the Repository
-Incomplete
bug: it makes duplicate files.
**/
void pushToRepo(std::string repo, std::string dir)
{
	fs::path from{ dir };
	fs::path to{ repo };

	std::vector<fs::directory_entry> container(directorySize(dir));

	copy(fs::recursive_directory_iterator(from), fs::recursive_directory_iterator(), container.begin());

	for (int i = 0; i < container.size(); i++)
	{
		//if the path points to a file
		//checksum the file to get ArtifactID
		if (fs::is_regular_file(container[i].path()))
		{
			size_t found = container[i].path().string().find("\\Manifest\\");
			if (found != std::string::npos)
			{
				std::cerr << "Don't need to copy Manifest files\n";
			}
			else
			{
				//go through directory path to file and get its checksum
				std::ifstream infile(container[i].path().string());
				char c;
				int checkSum = 0;
				int counter = 0;
				while (infile.get(c))
				{
					checkSum += checksum(c, counter);
					counter++;
				}
				infile.close();

				std::string s = container[i].path().string();

				removeSubstrs(s, from.parent_path().string());

				std::ifstream newInFile(container[i].path().string());

				std::string outputFile = repo + "\\" + s + "\\";

				fs::path temp{ outputFile.c_str() };
				fs::create_directories(outputFile);

				outputFile = outputFile + std::to_string(checkSum) + "-L" + std::to_string(counter) + container[i].path().extension().string();

				std::ofstream outFile(outputFile);
				std::string d;
				while (std::getline(newInFile, d))
				{
					outFile << d;
					outFile << "\n";
				}
				newInFile.close();
				outFile.close();
			}
		}

		createManifest(repo);
	}
}

std::vector<std::string> ReadManifest(std::string manifest)
{
	std::ifstream man_file;
	man_file.open(manifest);

	std::string line;
	std::vector<std::string> stuff;

	do
	{
		std::getline(man_file, line);
		if (!line.empty())
		{
			std::size_t pos = line.find("\t");
			std::string filename = line.substr(0, pos);
			stuff.push_back(filename);
		}
	} while (man_file.good());

	return stuff;
}

/**
* Pull from a repository
Parameters: your own source directory and the Repository
-Incomplete : Have yet to find a way to an efficient way to compare the directories.
-because the paths are full paths and we need to reduce them into related paths.
*/
void pullFromRepo(std::string dir, std::string repo)
{
	std::string line, label, manName;
	std::size_t pos;
	int labeled = 0;

	std::ifstream labelFileIn("labels.txt");

	std::cout << "What is the label or manifest name you would like to pull? ";
	std::cin >> label;

	while (labelFileIn.good())
	{
		std::getline(labelFileIn, line);
		pos = line.find(label);
		if (pos != std::string::npos) {
			labeled = 1;
			break;
		}
	}

	if (labeled == 1) {
		pos = line.find("\t");
		manName = line.substr(0, pos);
	}

	else
		manName = label;

	std::string manifest_file = repo + "\\Manifest\\" + manName + ".txt";

	std::vector<std::string> files = ReadManifest(manifest_file);

	//std::cout << manifest_file << std::endl;

	fs::path dest(repo);

	for (size_t i = 0; i < files.size(); i++)
	{
		fs::path one(files[i]);
		fs::path sauce = dest.parent_path().string() + one.string();

		std::cout << "Sauce:" << sauce.string() << std::endl;

		if (fs::is_regular_file(sauce))
		{
			size_t found = sauce.string().find("\\Manifest\\");
			if (found != std::string::npos)
			{
				std::cerr << "Don't need to copy Manifest files\n";
			}
			
			else
			{

				files[i] = one.parent_path().string();

				//std::size_t pos = files[i].find_last_of("\\");
				//files[i] = files[i].erase(0, pos);

				std::cout << "Files" << files[i] << std::endl;
				
				fs::path destinado = dir + files[i];

				std::cout << "Destinado:" << destinado << std::endl;

				fs::create_directories(destinado.parent_path().string());
				
				std::ifstream newInFile(sauce.string());
				std::ofstream outFile(destinado.string());
				
				std::string d;
				while (std::getline(newInFile, d))
				{
					outFile << d;
					outFile << "\n";
				}

				newInFile.close();
				outFile.close();
			}
		}
	}
	
	createManifest(repo);
}
/**
* Create label for the manifest file
**/
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

/**
* Helper function for push and pull that we will eventually use.
-parameters: given a destination directory and a source directory
-push or pulls based on your parameters.
**/
void push_or_pull(std::string destination, std::string source)
{
	fs::path dest_man = MostRecentManifest(destination);
	fs::path source_man = MostRecentManifest(source);

	std::ifstream file(dest_man.string());
	std::string line;

	std::vector <std::string> dest_vec;

	do
	{
		std::getline(file, line);
		if (!line.empty())
		{
			dest_vec.push_back(line);
		}
	} while (file.good());

	std::vector<std::string> different = compareFiles(dest_vec, source_man.string());

	for (int i = 0; i < different.size(); i++)
	{
		fs::path different_file(source + "\\" + different[i]);

		std::cout << source + "\\" + different[i] << std::endl;

		if (fs::is_regular_file(different_file))
		{
			std::ifstream infile(different_file.string());
			char c;
			int checkSum = 0, counter = 0;

			while (infile.get(c))
			{
				checkSum += checksum(c, counter);
				counter++;
			}

			infile.close();

			fs::path destination = destination;
			fs::path source = source;

			std::ifstream newInFile(source.parent_path().string() + "\\" + different_file.string() + "\\");
			std::string outputFile = destination.string() + "\\" + different[i] + "\\";

			fs::path temp(outputFile.c_str());
			fs::create_directories(outputFile);
			outputFile = outputFile + std::to_string(checkSum) + "-L" + std::to_string(counter) + different_file.extension().string();

			std::ofstream outFile(outputFile);
			std::string d;

			while (std::getline(newInFile, d))
			{
				outFile << d;
				outFile << "\n";
			}
			newInFile.close();
			outFile.close();
		}
	}
}
