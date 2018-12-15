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

void createRepo(std::string source, std::string dest)
{
	fs::path from{ source };
	fs::path to{ dest };

	std::vector<fs::directory_entry> container(directorySize(source));

	copy(fs::recursive_directory_iterator(from), fs::recursive_directory_iterator(), container.begin());

	for (int i = 0; i < container.size(); i++)
	{
		//if the path points to a file
		//checksum the file to get ArtifactID
		if (fs::is_regular_file(container[i].path()))
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

			std::string outputFile = dest + "\\" + s + "\\";

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

	createManifest(dest);
}

int main()
{
	int input = prompt();

	std::string source;
	std::cout << "Enter the source directory: ";
	std::getline(std::cin, source);

	std::string dest;
	std::cout << "Enter the repo directory: ";
	std::getline(std::cin, dest);

	while (input != 6)
	{
		if (input == 1)
			createRepo(source, dest);
		else if (input == 2)
			pushToRepo(dest, source);
		else if (input == 3)
			pullFromRepo(source, dest);
		else if (input == 4)
			labelManifest();
		else if (input == 5)
			Merge(dest, source);
		else
			std::cout << "Please enter a valid input" << std::endl;
		input = prompt();
	}
	std::cout << "Closing program" << std::endl;

	system("Pause");
	return 0;
}