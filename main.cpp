#include "Node.h"
#include <string.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <iterator>
#include <math.h>
#include <stack>

int goal[3][3] = { {1,2,3}, {4,5,6}, {7,8,0} };

//user defined compare that will be used to create our priority queue and make it based off the costs of the nodes
struct compareNodes {
	bool operator()(Node* n1, Node* n2) {
		return n1->getCost() > n2->getCost();
	}
};

bool compareStates(Node* n1, Node* n2) {
	//can also consolidate into one function that just take in two nodes, see how many things are mismatching
	//and if it is > 0 then they do not equal each other
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if (n1->state[i][j] != n2->state[i][j]) {
				return false;
			}
		}
	}
	return true;
}

bool compareGoal(Node* n1) {
	//now looking at this I can consolidate into one function with numMismatch and just check
	//if returned value == 0 then you are == to goal so you have a solution
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if (n1->state[i][j] != goal[i][j]) {
				return false;
			}
		}
	}
	return true;
}

bool checkExplored(Node* n1, std::vector<Node*> explored) {
	
	if (explored.size() != 0)
	{
		for (int exploredIndex = 0; exploredIndex < explored.size(); exploredIndex++)
		{
			if (compareStates(n1, explored.at(exploredIndex))) {
				return true;
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}

int numMismatch(Node* n1) {
	//go through the state of the node and see how many values at each index match the values in the goal state
	//increment for each value that is not == to goal state value
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if (n1->state[i][j] != goal[i][j]) {
				count++;
			}
		}
	}
	return count;
}

int numManhattan(Node* n1) {
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			count += abs((n1->state[i][j] - 1) % 3 - i % 3) + abs((n1->state[i][j] - 1) / 3 - i / 3);
		}
	}
	return count;
}

int chooseCost(Node* n1, int option) {
	//choose the corresponding cost based on what input was passed in by user
	if (option == 1)
	{
		return n1->getCost() + 1;
	}
	else if(option == 2)
	{
		return n1->getCost() + 1 + numMismatch(n1);
	}
	else if (option == 3) {
		return n1->getCost() + 1 + numManhattan(n1);
	}
}

bool Search(Node* root, int option) {
	//our flag to be returned
	bool solution = true;
	//a priority queue holding Node*, with underlying container vector holding Node* and my user defined compare
	std::priority_queue<Node*, std::vector<Node*>, compareNodes> frontier;
	//push root into frontier
	frontier.push(root);
	//explored data structure can be of any type so I chose vector so it can dynamically change size
	std::vector<Node*> explored;
	//Our iterator node, where we are currently at in our tree
	Node* curr;
	//a counter for how many children we create and add to the frontier
	int numChildrenCreated = 0;
	//Loop until our frontier is empty or we find a solution
	while (!frontier.empty()) {
		if (frontier.empty()) {
			std::cout << "There was no solution" << std::endl;
			return false;
		}
		curr = frontier.top();
		frontier.pop();
		//this is equivalent to checking the frontier when expanding our nodes, we pretty much add duplicates into frontier
		//but this will catch the duplicates and skip them
		if (checkExplored(curr,explored))
		{
			continue;
		}
		solution = compareGoal(curr);
		if (solution)
		{
			std::cout << "There was a solution!" << std::endl;
			std::cout << "Amount of steps taken was " << curr->getLevel() << std::endl;
			std::cout << "number of children created and added to frontier is " << numChildrenCreated << std::endl;
			std::cout << "Number of nodes explored is " << explored.size() << std::endl;
			return true;
		}
		explored.push_back(curr);
		//check which operations you can make, use the postion of the blank to see chich operations
		//are valid
		int currBlankX = curr->getBlankX();
		int currBlankY = curr->getBlankY();
		//if the blank is in the top row x==0 then it cannot move up, so check x != 0 is true to move blank up
		if (currBlankX != 0)
		{
			//need to save the value in the space where the blank is going to move to
			int temp = curr->state[currBlankX - 1][currBlankY];
			//create a copy of the state we are currently at
			int newState[3][3];
			memcpy(newState, curr->state, 3 * 3 * sizeof(int));
			//swap the blank and the value
			newState[currBlankX-1][currBlankY] = 0;
			newState[currBlankX][currBlankY] = temp;
			//create new child
			Node* childUp = new Node(newState, curr, chooseCost(curr,option), curr->getLevel() + 1);
			bool addChildUp = checkExplored(childUp,explored);
			//check explored set for the newly created child, if not in explored add to frontier
			if (!addChildUp)
			{
				numChildrenCreated++;
				frontier.push(childUp);
			}
		}
		//if the blank is in the bottom row x==2 then it cannot move down, so check x != 2 is true to move blank down
		if (currBlankX != 2)
		{
			//need to save the value in the space where the blank is going to move to
			int temp = curr->state[currBlankX + 1][currBlankY];
			//create a copy of the state we are currently at
			int newState[3][3];
			memcpy(newState, curr->state, 3 * 3 * sizeof(int));
			//swap the blank and the value
			newState[currBlankX + 1][currBlankY] = 0;
			newState[currBlankX][currBlankY] = temp;
			//create new child
			Node* childDown = new Node(newState, curr, chooseCost(curr,option), curr->getLevel() + 1);
			bool addChildDown = checkExplored(childDown, explored);
			//check explored set for the newly created child, if not in explored add to frontier
			if (!addChildDown)
			{
				numChildrenCreated++;
				frontier.push(childDown);
			}
		}
		//if the blank is in the left col y==0 then it cannot move up, so check y != 0 is true to move blank left
		if (currBlankY != 0)
		{
			//need to save the value in the space where the blank is going to move to
			int temp = curr->state[currBlankX][currBlankY-1];
			//create a copy of the state we are currently at
			int newState[3][3];
			memcpy(newState, curr->state, 3 * 3 * sizeof(int));
			//swap the blank and the value
			newState[currBlankX][currBlankY-1] = 0;
			newState[currBlankX][currBlankY] = temp;
			//create new child
			Node* childLeft = new Node(newState, curr, chooseCost(curr, option), curr->getLevel() + 1);
			bool addChildLeft = checkExplored(childLeft, explored);
			//check explored set for the newly created child, if not in explored add to frontier
			if (!addChildLeft)
			{
				numChildrenCreated++;
				frontier.push(childLeft);
			}
		}
		//if the blank is in the right col y==0 then it cannot move up, so check y != 0 is true to move blank right
		if (currBlankY != 2)
		{
			//need to save the value in the space where the blank is going to move to
			int temp = curr->state[currBlankX][currBlankY + 1];
			//create a copy of the state we are currently at
			int newState[3][3];
			memcpy(newState, curr->state, 3 * 3 * sizeof(int));
			//swap the blank and the value
			newState[currBlankX][currBlankY + 1] = 0;
			newState[currBlankX][currBlankY] = temp;
			//create new child
			Node* childRight = new Node(newState, curr, chooseCost(curr, option), curr->getLevel() + 1);
			bool addChildRight = checkExplored(childRight, explored);
			//check explored set for the newly created child, if not in explored add to frontier
			if (!addChildRight)
			{
				numChildrenCreated++;
				frontier.push(childRight);
			}
		}
	}
	/*std::stack<Node*> stack;
	while (curr->getParent()) {
		stack.push(curr->getParent());
	}
	std::cout << "Solution path is " << std::endl;
	*/
}

int main() {
	int puzzle[3][3] = { {1,2,3}, {0,4,6}, {7,5,8} };
	int input = 0;
	char inputString [10];
	std::cout << "Would you like to run the program with a test puzzle(1) or input your own(2)?" << std::endl;
	std::cin >> input;
	if (input == 1) {
		//do nothing and leave puzzle default
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
			{
				std::cout << puzzle[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
	else if(input == 2){
		std::cout << "Enter the puzzle as a string of 9 numbers with your blank being 0(e.g. 123456780)" << std::endl;
		std::cin >> inputString;
		int index = 0;
		//take an input and format it into a 2d array
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
			{
				puzzle[i][j] = inputString[index] - 48;
				std::cout << puzzle[i][j] << " ";
				index++;
			}
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
	Node* root = new Node(puzzle, NULL, 0, 0);
	//root->printState();
	std::cout << "Which algorithm do you want: Uniform Cost Search (1), A* Misplaced Tile(2), A* Manhattan Distance(3)" << std::endl;
	std::cin >> input;
	Search(root, input);
	
}
