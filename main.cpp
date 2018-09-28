/**
 * CPSC 362 VCS Project
 * Copyright(C) 2018 Team TBD
 * @author Josh Gomberg jgomberg93@gmail.com
 * @author Michael Li limichael1099419@gmail.com
 * @author Frank Ngo frank.ngo@csu.fullerton.edu
 * @author Wellson Pan dihydrogenmonoxide1337@gmail.com
 *
 * main.cpp contains the main source code
**/

#include "Header.h"

int main()
{
	std::string source;
	std::cout << "Enter the source directory: ";
	std::getline(std::cin, source);
	fs::path from{ source };

	std::string dest;
	std::cout << "Enter the destination directory: ";
	std::getline(std::cin, dest);
	fs::path to{ dest };

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
			
			//reopens the source file since it can't be iterated through a second time in one opening
			std::ifstream newInFile(container[i].path().string());
			//creates the name of the destination
			std::string outputFile = dest + "\\" + container[i].path().relative_path().string() + "\\";
			
			//creates the directories for the new checksum file
			fs::create_directories(outputFile);
			
			//creates the name for the destination file with the checksum as the new name
			outputFile = outputFile + std::to_string(checkSum) + "-L" + std::to_string(counter) + container[i].path().extension().string();
			
			//opens the new file at the new location and copies content of source file to desination file
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
