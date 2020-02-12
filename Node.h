#pragma once
#ifndef NODE_H
#define NODE_H
#include<string>
#include<iostream>

class Node {
	friend class AVLTree;
private:
	int state[3][3];
	Node* parent;
	Node* left;
	Node* down;
	Node* right;
	Node* up;

public:
	Node(int state[3][3],Node* par, int cost);
	Node* getParent();
	void printState();
	void setLeft(Node* child);
	void setDown(Node* child);
	void setRight(Node* child);
	void setUp(Node* child);
	int cost;
};
#endif // !NODE_H