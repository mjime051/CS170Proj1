#include "Node.h"
#include <string.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <iterator>

int goal[3][3] = { {1,2,3}, {4,5,6}, {7,8,0} };

struct compareNodes {
	bool operator()(Node* n1, Node* n2) {
		return n1->getCost() > n2->getCost();
	}
};

bool compareStates(Node* n1, Node* n2) {
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
	//TODO
	return 0;
}

int chooseCost(Node* n1, int option) {
	if (option == 1)
	{
		return n1->getCost() + 1;
	}
	else if(option == 2)
	{
		return n1->getCost() + numMismatch(n1);
	}
	else if (option == 3) {
		return n1->getCost() + numManhattan(n1);
	}
}

bool Search(Node* root, int option) {
	bool solution = true;
	std::priority_queue<Node*, std::vector<Node*>, compareNodes> frontier;
	frontier.push(root);
	std::vector<Node*> explored;
	Node* curr;
	int numChildrenCreated = 0;
	while (!frontier.empty()) {
		if (frontier.empty()) {
			std::cout << "There was no solution" << std::endl;
			return false;
		}
		curr = frontier.top();
		frontier.pop();
		curr->printState();
		if (checkExplored(curr,explored))
		{
			continue;
		}
		solution = compareGoal(curr);
		if (solution)
		{
			std::cout << "There was a solution!" << std::endl;
			std::cout << "Amount of steps taken was " << curr->getLevel() << std::endl;
			return true;
		}
		explored.push_back(curr);
		//std::cout << "Number of nodes in explored is " << explored.size() << std::endl;
		//check which operations you can make
		int currBlankX = curr->getBlankX();
		int currBlankY = curr->getBlankY();
		//can move up! so do it
		if (currBlankX != 0)
		{
			int temp = curr->state[currBlankX - 1][currBlankY];
			int newState[3][3];
			memcpy(newState, curr->state, 3 * 3 * sizeof(int));
			newState[currBlankX-1][currBlankY] = 0;
			newState[currBlankX][currBlankY] = temp;
			//create new child
			//std::cout << "Curr cost is " << curr->getCost() << std::endl;
			//std::cout << "Updated up cost is now " << chooseCost(curr, option) << std::endl;
			Node* childUp = new Node(newState, curr, chooseCost(curr,option), curr->getLevel() + 1);
			bool addChildUp = checkExplored(childUp,explored);
			//check explored set for the newly created child
			
			//if it made it through the last two checks and it is still true then push it into frontier
			if (!addChildUp)
			{
				numChildrenCreated++;
				//std::cout << "number of children created so far is " << numChildrenCreated << std::endl;
				frontier.push(childUp);
			}
		}
		//can move down! so do it
		if (currBlankX != 2)
		{
			int temp = curr->state[currBlankX + 1][currBlankY];
			int newState[3][3];
			memcpy(newState, curr->state, 3 * 3 * sizeof(int));
			newState[currBlankX + 1][currBlankY] = 0;
			newState[currBlankX][currBlankY] = temp;
			//create new child
			//std::cout << "Curr cost is " << curr->getCost() << std::endl;
			//std::cout << "Updated down cost is now " << chooseCost(curr, option) << std::endl;
			Node* childDown = new Node(newState, curr, chooseCost(curr,option), curr->getLevel() + 1);
			bool addChildDown = checkExplored(childDown, explored);
			//check explored set for the newly created child

			//if it made it through the last two checks and it is still true then push it into frontier
			if (!addChildDown)
			{
				numChildrenCreated++;
				//std::cout << "number of children created so far is " << numChildrenCreated << std::endl;
				frontier.push(childDown);
			}
		}
		//can move left! so do it
		if (currBlankY != 0)
		{
			int temp = curr->state[currBlankX][currBlankY-1];
			int newState[3][3];
			memcpy(newState, curr->state, 3 * 3 * sizeof(int));
			newState[currBlankX][currBlankY-1] = 0;
			newState[currBlankX][currBlankY] = temp;
			//create new child
			//std::cout << "Curr cost is " << curr->getCost() << std::endl;
			//std::cout << "Updated left cost is now " << chooseCost(curr, option) << std::endl;
			Node* childLeft = new Node(newState, curr, chooseCost(curr, option), curr->getLevel() + 1);
			bool addChildLeft = checkExplored(childLeft, explored);
			//check explored set for the newly created child

			//if it made it through the last two checks and it is still true then push it into frontier
			if (!addChildLeft)
			{
				numChildrenCreated++;
				//std::cout << "number of children created so far is " << numChildrenCreated << std::endl;
				frontier.push(childLeft);
			}
		}
		//can move right! so do it
		if (currBlankY != 2)
		{
			int temp = curr->state[currBlankX][currBlankY + 1];
			int newState[3][3];
			memcpy(newState, curr->state, 3 * 3 * sizeof(int));
			newState[currBlankX][currBlankY + 1] = 0;
			newState[currBlankX][currBlankY] = temp;
			//create new child
			//std::cout << "Curr cost is " << curr->getCost() << std::endl;
			//std::cout << "Updated right cost is now " << chooseCost(curr, option) << std::endl;
			Node* childRight = new Node(newState, curr, chooseCost(curr, option), curr->getLevel() + 1);
			bool addChildRight = checkExplored(childRight, explored);
			//check explored set for the newly created child

			//if it made it through the last two checks and it is still true then push it into frontier
			if (!addChildRight)
			{
				numChildrenCreated++;
				//std::cout << "number of children created so far is " << numChildrenCreated << std::endl;
				frontier.push(childRight);
			}
		}
	}
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
