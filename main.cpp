#include "Node.h"
#include <string.h>
#include <stdio.h>
#include <queue>
#include <vector>

int goal[3][3] = { {1,2,3}, {4,5,6}, {7,8,0} };

struct compareNodes {
	bool operator()(Node* n1, Node* n2) {
		return n1->getCost() < n2->getCost();
	}
};

bool Search(Node* root, int option) {
	bool solution = false;
	std::priority_queue<Node*, std::vector<Node*>, compareNodes> frontier;
	frontier.push(root);
	std::vector<Node*> explored;
	Node* curr;
	while (!frontier.empty()) {
		if (frontier.empty()) {
			return false;
		}
		curr = frontier.top();
		frontier.pop();
		if (memcmp(curr->state, goal, sizeof(goal) > 0)) {
			//std::cout << "too small" << std::endl;
		}
		else if (memcmp(curr->state, goal, sizeof(goal) < 0))
		{
			//std::cout << memcmp(curr->state, goal, sizeof(goal)) << std::endl;
			//std::cout << "too large" << std::endl;
		}
		else {
			//std::cout << "just right" << std::endl;
		}
		explored.push_back(curr);
		
	}
}

int main() {
	int puzzle[3][3] = { {1,2,3}, {4,5,6}, {7,8,0} };
	int input = 0;
	char inputString [10];
	std::cout << "Would you like to run the program with a test puzzle(1) or input your own(2)?" << std::endl;
	std::cin >> input;
	if (input == 1) {
		//do nothing and leave puzzle as easy puzzle
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
	Node* root = new Node(puzzle, NULL, 0);
	//root->printState();
	std::cout << "Which algorithm do you want: Uniform Cost Search (1), A* Misplaced Tile(2), A* Manhattan Distance(3)" << std::endl;
	std::cin >> input;
	if (input == 1) {
		Search(root, 1);
	}
	else if (input == 2) {
		//TODO
	}
	else if (input == 3)
	{
		//TODO
	}
	
}
