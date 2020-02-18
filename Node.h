#pragma once
#ifndef NODE_H
#define NODE_H
#include<string>
#include<iostream>

class Node {
private:
	
	int cost;
	Node* parent;
	Node* left;
	Node* down;
	Node* right;
	Node* up;

public:
	int state[3][3];
	Node(int state[3][3],Node* par, int cost);
	Node* getParent();
	int getCost();
	int getBlankX();
	int getBlankY();
	void printState();
	void setLeft(Node* child);
	void setDown(Node* child);
	void setRight(Node* child);
	void setUp(Node* child);
};
#endif // !NODE_H
