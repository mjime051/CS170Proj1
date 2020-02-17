#include "Node.h"

Node::Node(int input[3][3],Node* par, int cost) {
	this->parent = par;
	memcpy(this->state, input, 3 * 3 * sizeof(int));
	this->left = NULL;
	this->down = NULL;
	this->right = NULL;
	this->up = NULL;
	this->cost = cost;
}

Node* Node::getParent() {
	return this->parent;
}

int Node::getCost() {
	return this->cost;
}

int Node::getBlankX() {
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if (this->state[i][j] == 0) {
				return i;
			}
		}
	}
}

int Node::getBlankY() {
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if (this->state[i][j] == 0) {
				return j;
			}
		}
	}
}

void Node::printState() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << this->state[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Node::setLeft(Node* child) {
	this->left = child;
}

void Node::setDown(Node* child) {
	this->down = child;
}

void Node::setRight(Node* child) {
	this->right = child;
}

void Node::setUp(Node* child) {
	this->up = child;
}
