Class Name: 
	CPSC 362 Section 1

Project Name: 
	VCS Project 2

Team Name: 
	TBD (this is our actual TLA)

Team Members:
	Joshua Gomberg, Michael Li, Frank Ngo, Wellson Pan

Intro: 
	The user is first prompted with a menu.
	They can choose if they'd like to crete a repo, push or pull to the repo, label a manifest, or exit.
	If the user chooses to create a repo then:
		The program then loops through the source directory to find out how many files are in it.
		It then loops through source directory and if the path is at a folder it is copied to the destination directory;			if it is at a file, the name is put through the checksum to get the ArtifactID. 
		The original file name is copied to make a new folder in the destination directory and the file with ArtifactID 			name is copied inside it.
	If the user chooses to push to the repo:
		The program 
	If the user chooses to pull from the repo:
		The program
	If the user chooses to label a manifest:
		The program prompts the user for the file name of the manifest they would like to label, then prompts them for 				the label they would like to give it.
	Once a command is executed the user is brought back to the main prompt and can exit or execute another command.

Files: 
	Header.h, main.cpp, README.txt

External Requirements: 
	Boost C++ Library, Version 1_68_0. 

Setup and Installation: 
	The Boost Library needs to be downloaded and installed.
	The project solution needs to point Additional Include Directories to \boost_1_68_0
	The project solution needs to point Additional Library Directories to \boost_1_68_0\stage\lib
	The program is compiled with Microsoft(R) Visual Studio 2017(R) and tested on Microsoft(R) Windows 10(R).

Sample/Results: 

	Input:
	SampleInput/
		this.txt
		is.c
		NextFolder/
			aTest.txt
			input.c
      
	Result:
	SampleInput/
		this.txt/
			2428-L4.txt
		is.c/
			450-L2.c
		NextFolder/
			aTest.txt/
				4293-L5.txt
			input.c/
				4478-L5.c

Features: 
	Copies all the files and folders from the given directory into a new repository.
	Converts the file names from the given directory using the checksum provided and saves those files under the ArtifactID into the new repository inside a folder withe the original file name. 
	There are no missing features.

Bugs: 
	The program only works with folders, plain text files (.txt, .c, .cpp, etc.), and binary files (.bin)
	The program does not truncate the path in the destination folder. For example: if the source is C:\Downloads\temp1\temp2 and the destination is C:\test, the repository is in C:\test\Downloads\temp1\temp2 instead of C:\test\temp2
