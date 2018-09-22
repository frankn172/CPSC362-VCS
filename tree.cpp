#include "tree.h"

int Tree::size()const {
	return n;
}

bool Tree::empty()const {

	return (size() == 0);
}

Tree::Tree()
{
	int n = 0; 
	_root = NULL;

}

Tree::~Tree()
{
	
}

void Tree::InsertNode(int x , string str)
{

}