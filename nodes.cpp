#include <iostream>
#include <stdlib.h>
#include <vector>

#include "nodes.h"


Node::Node()
{
	count = 0;
	character = -1;
	Node *left = NULL;
	Node *right = NULL;
	std::string code;
	leaf = 1;
}

Node::Node(int name, int the_count, int l)
{
    character = name;
    count = the_count;
    Node *left = NULL;
	Node *right = NULL;
	std::string code;
	leaf = l;
}
/*
Node::Node(Node * left, Node * right)
{
	this->left = left;
	this->right = right;
	count = left->get_count() + right->get_count();
	character = -1;
	leaf = 1;
}*/

Node::~Node()
{
	// std::cout << "Node being deleted" << std::endl;
	if(left != NULL) delete left;
	if(right != NULL) delete right;
}

void Node::set_count(int c)
{
	count = c;
}

int Node::get_count()
{
	return count;
}

void Node::set_char(int c)
{
	character = c;
}

int Node::get_char()
{
	return character;
}

void Node::set_left(Node * l)
{
	left = l;
}

void Node::set_right(Node * r)
{
	right = r;
}

Node* Node::get_left()
{
	return left;
}

Node* Node::get_right()
{
	return right;
}

std::string Node::get_code()
{
	return code;
}

void Node::set_code(std::string data)
{
	code = data;
}
