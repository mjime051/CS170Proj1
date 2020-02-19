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

public:
	int state[3][3];
	Node(int state[3][3],Node* par, int cost, int level);
	Node* getParent();
	int getCost();
	int getBlankX();
	int getBlankY();
	int getLevel();
	void printState();
};
#endif // !NODE_H
