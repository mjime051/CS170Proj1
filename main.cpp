#include "Node.h"
#include <string.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <iterator>
#include <math.h>
#include <stack>
#include <map>

int goal[3][3] = { {1,2,3}, {4,5,6}, {7,8,0} };
Node* stackCurr;
std::vector<std::pair<int, std::pair<int,int>>> vec;
//user defined compare that will be used to create our priority queue and make it based off the costs of the nodes
struct compareNodes {
	bool operator()(Node* n1, Node* n2) {
		return (n1->getCost() + n1->getLevel()) > (n2->getCost() + n2->getLevel());
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
	//std::cout << "Checking if this state is in explored yet" << std::endl;
	//n1->printState();
	if (explored.size() != 0)
	{
		for (int exploredIndex = 0; exploredIndex < explored.size(); exploredIndex++)
		{
			//std::cout << "Comparing to " << std::endl;
			//explored.at(exploredIndex)->printState();
			if (compareStates(n1, explored.at(exploredIndex))) {
				//std::cout << "true my dude" << std::endl;
				return true;
			}
		}
		//std::cout << "false" << std::endl;
		return false;
	}
	else
	{
		//std::cout << "false cause empty" << std::endl;
		return false;
	}
}

int numMismatch(int arr[3][3]) {
	//go through the state of the node and see how many values at each index match the values in the goal state
	//increment for each value that is not == to goal state value
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if (arr[i][j] != goal[i][j]) {
				count++;
			}
		}
	}
	return count;
}

void initMap() {
	std::pair<int, int> first(0, 0);
	std::pair<int, std::pair<int, int>> index00(1, first);
	std::pair<int, int> second(0, 1);
	std::pair<int, std::pair<int, int>> index01(2, second);
	std::pair<int, int> third(0, 2);
	std::pair<int, std::pair<int, int>> index02(3, third);
	std::pair<int, int> fourth(1, 0);
	std::pair<int, std::pair<int, int>> index10(4, fourth);
	std::pair<int, int> fifth(1, 1);
	std::pair<int, std::pair<int, int>> index11(5, fifth);
	std::pair<int, int> sixth(1, 2);
	std::pair<int, std::pair<int, int>> index12(6, sixth);
	std::pair<int, int> seventh(2, 0);
	std::pair<int, std::pair<int, int>> index20(7, seventh);
	std::pair<int, int> eighth(2, 1);
	std::pair<int, std::pair<int, int>> index21(8, eighth);
	std::pair<int, int> ninth(2, 2);
	std::pair<int, std::pair<int, int>> index22(0, ninth);
	vec.push_back(index00);
	vec.push_back(index01);
	vec.push_back(index02);
	vec.push_back(index10);
	vec.push_back(index11);
	vec.push_back(index12);
	vec.push_back(index20);
	vec.push_back(index21);
	vec.push_back(index22);
	/*for (size_t i = 0; i < vec.size(); i++)
	{
		std::cout << "Index " << i << " is " << vec[i].second.first << vec[i].second.second << std::endl;
	}*/
}

int numManhattan(int arr[3][3]) {
	int count = 0;
	int it = 0;
	for (int k = 0; k < 3; k++)
	{
		for (int l = 0; l < 3; l++) {
			if (arr[k][l] != goal[k][l]) {
				//std::cout << it << std::endl;
				int val = arr[k][l] - 1;
				//std::cout << "value at " << val << " is " << vec[val].second.first << vec[val].second.second << std::endl;
				count += abs(vec[val].second.first - k) + abs(vec[val].second.second - l);
			}
			it++;
		}
	}
	//std::cout << count << std::endl;
	return count;
}

int chooseCost(int arr[3][3], int option) {
	//choose the corresponding cost based on what input was passed in by user
	if (option == 1)
	{
		return 0;
	}
	else if(option == 2)
	{
		return numMismatch(arr);
	}
	else if (option == 3) {
		return numManhattan(arr);
	}
}

void printSolution() {
	std::stack<Node*> stack;
	Node* goalNode = new Node(goal, NULL, 0, 0);
	stack.push(goalNode);
	while (stackCurr->getParent()) {
		stack.push(stackCurr->getParent());
		stackCurr = stackCurr->getParent();
	}

	std::cout << "Solution path is " << std::endl;

	while (!stack.empty())
	{
		stackCurr = stack.top();
		stackCurr->printState();
		std::cout << std::endl;
		stack.pop();
	}
}

void Search(Node* root, int option) {
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
		//std::cout << frontier.size() << std::endl;
		curr = frontier.top();
		frontier.pop();
		//this is equivalent to checking the frontier when expanding our nodes, we pretty much add duplicates into frontier
		//but this will catch the duplicates and skip them
		solution = compareGoal(curr);
		if (solution)
		{
			stackCurr = curr;
			std::cout << "There was a solution!" << std::endl;
			std::cout << "Amount of steps taken was " << curr->getLevel() << std::endl;
			std::cout << "number of children created and added to frontier is " << numChildrenCreated << std::endl;
			std::cout << "Number of nodes explored is " << explored.size() << std::endl;
			/*for (size_t s = 0; s < explored.size(); s++)
			{
				explored.at(s)->printState();
				std::cout << "Heurtistic is " << explored.at(s)->getCost() << " path cost is " << explored.at(s)->getLevel() << std::endl;
				std::cout << std::endl;
			}*/
			printSolution();
			break;
		}
		//std::cout << "checking frontier" << std::endl;
		if (!checkExplored(curr,explored))
		{
			//curr->printState();
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
				newState[currBlankX - 1][currBlankY] = 0;
				newState[currBlankX][currBlankY] = temp;
				//create new child
				Node* childUp = new Node(newState, curr, chooseCost(newState, option), curr->getLevel() + 1);
				//std::cout << "checking at creation" << std::endl;
				bool addChildUp = !checkExplored(childUp, explored);
				numChildrenCreated++;
				//check explored set for the newly created child, if not in explored add to frontier
				if (addChildUp)
				{
					//std::cout << "Added up to frontier" << std::endl;
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
				Node* childDown = new Node(newState, curr, chooseCost(newState, option), curr->getLevel() + 1);
				//std::cout << "checking at creation" << std::endl;
				bool addChildDown = !checkExplored(childDown, explored);
				numChildrenCreated++;
				//check explored set for the newly created child, if not in explored add to frontier
				if (addChildDown)
				{
					//std::cout << "Added down to frontier" << std::endl;
					frontier.push(childDown);
				}
			}
			//if the blank is in the left col y==0 then it cannot move up, so check y != 0 is true to move blank left
			if (currBlankY != 0)
			{
				//need to save the value in the space where the blank is going to move to
				int temp = curr->state[currBlankX][currBlankY - 1];
				//create a copy of the state we are currently at
				int newState[3][3];
				memcpy(newState, curr->state, 3 * 3 * sizeof(int));
				//swap the blank and the value
				newState[currBlankX][currBlankY - 1] = 0;
				newState[currBlankX][currBlankY] = temp;
				//create new child
				Node* childLeft = new Node(newState, curr, chooseCost(newState, option), curr->getLevel() + 1);
				//std::cout << "checking at creation" << std::endl;
				bool addChildLeft = !checkExplored(childLeft, explored);
				numChildrenCreated++;
				//check explored set for the newly created child, if not in explored add to frontier
				if (addChildLeft)
				{
					//std::cout << "Added left to frontier" << std::endl;
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
				Node* childRight = new Node(newState, curr, chooseCost(newState, option), curr->getLevel() + 1);
				//std::cout << "checking at creation" << std::endl;
				bool addChildRight = !checkExplored(childRight, explored);
				numChildrenCreated++;
				//check explored set for the newly created child, if not in explored add to frontier
				if (addChildRight)
				{
					//std::cout << "Added right to frontier" << std::endl;
					frontier.push(childRight);
				}
			}
		}
		
	}
	if (frontier.empty()) {
		std::cout << "There was no solution" << std::endl;
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
	initMap();
	Search(root, input);
	/*Node* goalNode = new Node(goal, NULL, 0, 0);
	std::cout << compareStates(root, goalNode);
	std::vector<Node*> explored;
	explored.push_back(goalNode);
	explored.push_back(root);
	std::cout << checkExplored(root, explored);
	*/
	return 0;
}
