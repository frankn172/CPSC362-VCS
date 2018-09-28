/**
 * CPSC 362 VCS Project
 * Copyright(C) 2018 Josh Gomberg, Michael Li, Frank Ngo, & Wellson Pan
**/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
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

int main()
{
	std::string source;
	std::cout << "Enter the source directory: ";
	std::getline(std::cin, source);
	fs::path from{source};
	
	std::string dest;
	std::cout << "Enter the destination directory: ";
	std::getline(std::cin, dest);
	fs::path to{dest};

	std::vector<fs::directory_entry> container(directorySize(source));

	copy(fs::recursive_directory_iterator(from), fs::recursive_directory_iterator(), container.begin());
	
	for (int i = 0; i < container.size(); i++)
	{
		//if the path points to a valid directory within "from"
		//duplicate it in "dest" inside the correct parent directory
		if (fs::is_directory(container[i].path()))
		{
			std::string to2 = dest + "\\" + container[i].path().relative_path().string();
			fs::path temp{ to2 };
			fs::create_directories(temp);
		}
		//if the path points to a file
		//checksum the file to get ArtifactID
		else if (fs::is_regular_file(container[i].path()))
		{
			/*Should put the checksum in another file as a function an call it or something*/
			std::ifstream infile(container[i].path().string());
			char c;
			int checkSum = 0;
			int counter = 1;
			while (infile.get(c))
			{
				if (counter % 5 == 1)
				{
					checkSum += (c * 1);
				}
				else if (counter % 5 == 2)
				{
					checkSum += (c * 3);
				}
				else if (counter % 5 == 3)
				{
					checkSum += (c * 7);
				}
				else if (counter % 5 == 4)
				{
					checkSum += (c * 11);
				}
				else if (counter % 5 == 0)
				{
					checkSum += (c * 17);
				}
				counter++;
			}
			infile.close();

			std::ifstream newInFile(container[i].path().string());
			std::string outputFile = dest  + "\\" + container[i].path().relative_path().string() + "\\";

			fs::path temp{outputFile.c_str()};
			fs::create_directories(outputFile);

			int m = (std::pow(2, 31)) - 1;
			checkSum = checkSum % m;
			outputFile = outputFile + std::to_string(checkSum) + "-L" + std::to_string(counter - 1) + container[i].path().extension().string();
			
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

	system("Pause");
	return 0;
}
