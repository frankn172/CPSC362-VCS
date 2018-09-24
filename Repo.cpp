#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

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
	std::cin >> source;
	fs::path from{source};
	/*std::string source = "D:\\Z";
	fs::path from{ source };*/
	
	std::string dest;
	std::cout << "Enter the destination directory: ";
	std::cin >> dest;
	fs::path to{dest};

	std::vector<fs::directory_entry> container(directorySize(source));

	//copy(fs::recursive_directory_iterator(p), fs::recursive_directory_iterator(), std::ostream_iterator<fs::directory_entry>(std::cout, "\n"));
	copy(fs::recursive_directory_iterator(from), fs::recursive_directory_iterator(), container.begin());

	/*for (int i = 0; i < container.size(); i++)
	{
		std::cout << container[i].path().relative_path().string() << std::endl;
	}*/
	
	for (int i = 0; i < container.size(); i++)
	{
		//std::cout << container[i].path().string() << std::endl;
		if (fs::is_directory(container[i].path()))
		{
		std::string to2 = dest + "\\" + container[i].path().relative_path().string();
		fs::path temp{ to2 };
		fs::create_directories(temp);
		}
		else if (fs::is_regular_file(container[i].path()))
		{
			std::ifstream infile(container[i].path().string());
			char c;
			int checkSum = 0;
			int counter = 1;
			while (infile.get(c))
			{
				//TODO checksum might go here
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
			std::string outputFile = dest  + "\\" + container[i].path().relative_path().string() + "\\";
			//Create new directory if it doesn't exist
			fs::path temp{outputFile.c_str()};
			fs::create_directories(outputFile);

			outputFile = outputFile + std::to_string(checkSum) + container[i].path().extension().string();

			//temp += std::to_string(checkSum);
			//temp += ".cpp" //or whatever the file extension is. I have no idea how to get it.
			
			std::ofstream outFile(outputFile);
			char d;
			while (!infile.eof())
			{
				infile.get(d);//this will work once outFile gets the checksum name for the directory
				outFile << d;
			}
			infile.close();
			outFile.close(); //uncomment this when you have an argument for the outFile
			
		}
	}

	system("Pause");
	return 0;
}