Class Name: 
CPSC 362 Section 1

Project Name: 
VCS Project 1

Team Name: 
TBD (this is our actual TLA)

Team Members:
Joshua Gomberg, Michael Li, Frank Ngo, Wellson Pan

Intro: 
The source directory and destination directory are first prompted from the user.
The program then loops through the source directory to find out how many files are in it.
It then loops through source directory and if the path is at a folder it is copied to the destination directory;
if it is at a file, the name is put through the checksum to get the ArtifactID. 
The original file name is copied to make a new folder in the destination directory and the file with ArtifactID name is copied inside it.

Files: 
main.cpp, README.txt

External Requirements: 
Boost C++ Library, Version 1_68_0. 

Setup and Installation: 
The Boost Library needs to be downloaded and installed.

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
None
