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
	//fs::path p{ "D:\\Z" };
	
	std::string dest;
	std::cout << "Enter the destination directory: ";
	std::cin >> dest;
	fs::path to{dest};

	std::vector<fs::directory_entry> container(directorySize(source));

	//copy(fs::recursive_directory_iterator(p), fs::recursive_directory_iterator(), std::ostream_iterator<fs::directory_entry>(std::cout, "\n"));
	copy(fs::recursive_directory_iterator(from), fs::recursive_directory_iterator(), container.begin());
	
	for (int i = 0; i < container.size(); i++)
	{
		//std::cout << container[i].path().string() << std::endl;
		if (fs::is_regular_file(container[i].path()))
		{
			std::ifstream infile(container[i].path().string());
			char c;
			while (infile.get(c))
			{
				//TODO checksum might go here
			}
			std::string outputFile = container[i].path().string() + "\\" ;
			//Create new directory if it doesn't exist
			to += container[i].path().relative_path().string();
			fs::create_directory(to);

			/*to += checksum.toString() ??????
			to += ".cpp" //or whatever the file extension is. I have no idea how to get it.*/

			std::ofstream outFile(/*the new directory with the checksum file name goes here*/);
			while (infile.get(c))
			{
				/*outFile << c;  //this will work once outFile gets the checksum name for the directory */
			}
			infile.close();
			//outFile.close(); //uncomment this when you have an argument for the outFile
			
		}
	}

	system("Pause");
	return 0;
}