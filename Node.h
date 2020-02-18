#pragma once
#ifndef NODE_H
#define NODE_H
#include<string>
#include<iostream>

class Node {
private:
	
	int cost;
	int level;
	Node* parent;
	Node* left;
	Node* down;
	Node* right;
	Node* up;

public:
	int state[3][3];
	Node(int state[3][3],Node* par, int cost, int level);
	Node* getParent();
	int getCost();
	int getBlankX();
	int getBlankY();
	int getLevel();
	void printState();
	void setLeft(Node* child);
	void setDown(Node* child);
	void setRight(Node* child);
	void setUp(Node* child);
};
#endif // !NODE_H
