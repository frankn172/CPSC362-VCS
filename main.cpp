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

int prompt()
{
	int input;
	std::cout << "What would you like to do?" << std::endl;
	std::cout << "Enter the number corresponding with the command" << std::endl;
	std::cout << "1. Create repo" << std::endl;
	std::cout << "2. Push to repo" << std::endl;
	std::cout << "3. Pull from repo" << std::endl;
	std::cout << "4. Label manifest" << std::endl;
	std::cout << "5. Exit" << std::endl;
	std::cin >> input;
	std::cin.ignore(256, '\n');
	return input;
}

void createRepo()
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
		/*if (fs::is_directory(container[i].path()))
		{
			std::string to2 = dest + "\\" + container[i].path().relative_path().string();
			fs::path temp{ to2 };
			fs::create_directories(temp);
		}*/
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

			std::ifstream newInFile(container[i].path().string());
			std::string outputFile = dest + "\\" + container[i].path().relative_path().string() + "\\";
			std::cout << container[i].path().filename().string() << std::endl;

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
}

int main()
{
	int input = prompt();

	while (input != 5)
	{
		if (input == 1)
		{
			createRepo();
			input = prompt();
		}
		else if (input == 2)
		{
			prompt();
			input = prompt();
		}
		else if (input == 3)
		{
			pullFromRepo();
			input = prompt();
		}
		else if (input == 4)
		{
			labelManifest();
			input = prompt();
		}
		else
		{
			std::cout << "Please enter a valid input" << std::endl;
			input = prompt();
		}
	}
	std::cout << "Closing program" << std::endl;


	system("Pause");
	return 0;
}
