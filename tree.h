#pragma once
/*

* C++ Program to Implement Tree

*/

#include<stdio.h>
#include<conio.h>
#include<iostream>
#include <string>

using namespace std;

struct TreeNode

{

	TreeNode **child_ptr;
	string *address;
	bool leaf;			//is there a leaf below?
	int n;	//number of child nodes
	//isExternal/isInternal?
	
};

class Tree
{
public:
	Tree();
	~Tree();
	int size() const;
	bool empty() const;
	void Split(TreeNode);
	TreeNode Root(TreeNode); //returns the Root
	void InsertNode(int, string);

protected:
	void traverse(TreeNode);

private: 
	TreeNode *_root;
	int n; 
};






