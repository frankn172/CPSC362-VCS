#include "read.h"
#include "Tree.h"

int ReadPath(string directory, BTreeNode *btn)   //feel free to change to a tree directory 
{
	path dir(directory);

	try
	{
		if (exists(dir))
		{
			current_path(dir);
			//print current location
			cout << current_path() << '\n';

			if (is_directory(dir))      //check if it is a directory 
			{
				cout << dir.string() << " is a directory containing:\n";
				int count = 1;
				for (directory_iterator it(dir); it != directory_iterator(); ++it)
				{	//INSERT STRINGS INTO A DATA STRUCTURE 

					insert(count, (complete(it->path())).string());

					cout << complete(it->path()) << '\n';  //show absolute path

					cout << it->path().string() << "\n";	//show file 

					count++;
				}
			}

			else if (is_regular_file(dir)) //check if it is a file instead
			{
				cout << dir.string() << " is actually a file " << '\n';
				return -1;
			}
			else                                 //neither file nor folder
			{
				cout << dir.string() << " exists, but is neither a regular file nor a directory\n";
				return -1;
			}
		}
		else
		{
			cout << dir.string() << " does not exist\n";
			return -1;
		}
	}
	//additional check
	catch (const filesystem_error& e)
	{
		cerr << e.what() << '\n';
		return -1;
	}


	return 0;
}