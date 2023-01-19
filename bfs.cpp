////////////////////////////////////////////////////////////////
//	Name:			Ryan Quinn
//	Major:			Computer Science
//	Creation Date: 	9/28/21
//	Due Date:		10/1/21
//	Course:			CSC447 010
//	Professor Name:	Dr. Schwesinger
//	Assignment: 	1
//	Filename: 		bfs.cpp
//	Purpose:		Create a breadth first search for the towers of 
//					hanoi problem given a starting and solution state. 
//					The algorithm is deterministic.
////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_set>
#include <math.h>
#include <iomanip>

using namespace std;

////////////////////////////////////////////////////////////////
//	Function name:	readFile
//	Description: 	Reads a tower of hanoi state from a file.
//	Parameters:		vector<char> *state: holds a single game state
//					fstream &ifs: file stream
//	Return Value:	vector<char> * - a game state containing the three towers
////////////////////////////////////////////////////////////////
vector<char>* readFile(vector<char> *state, fstream &ifs){
	string fileLine;
	
	for (int i=0; i<3; ++i){
		getline(ifs, fileLine);
		fileLine.append("\n");
		
		//pushes tower elements into a vector for each tower
		for (int j=1; fileLine[j] != '\n'; ++j){
			if (fileLine[j] != ' '){
				state[i].push_back(fileLine[j]);
			}
		}
	}
	return state;
}

////////////////////////////////////////////////////////////////
//	Function name:	printTowers
//	Description: 	Prints out the towers in an easy to read way.
//	Parameters:		vector<char> *state: holds a single game state
//	Return Value:	N/A
////////////////////////////////////////////////////////////////
void printTowers(vector<char> *state){
	for (int i=0; i<3; ++i){	//loops 3 times for each tower
		cout << i+1 << " ";
		
		if (state[i].empty()){	//is tower empty?
			cout << '\n';
			continue;
		}
		
		vector<char>::iterator itr = state[i].begin();
		
		//using an iterator, print out all vector elements for each tower
		while (itr != state[i].end()){
			cout << *itr << ' ';
			itr++;
		}
		cout << "\n";
	}
}

////////////////////////////////////////////////////////////////
//	Function name:	successors
//	Description: 	Determines the successors for a given state. It will
//					add them to a vector and return it.
//	Parameters:		vector<char> *state: state to find successors for
//	Return Value:	vector<vector<char> *> - list of successors
////////////////////////////////////////////////////////////////
vector<vector<char> *> successors(vector<char> *state){
	vector<vector<char> *> result;	//vector to hold successors
	vector<char> *newState = new vector<char>[3];	//holds successor state
	
	//Computing Successors \/ (up to 6 maximum)
	
	//is there an element on the third tower?
	if (state[2].size() != 0){
		//deep copy input state to newState
		for (int i=0; i<3; ++i)
			newState[i] = state[i];
		
		//compute new state for move from tower 3 to 2
		newState[1].push_back(newState[2].back());
		newState[2].pop_back();
		result.push_back(newState);
		
		//deep copy input state to newState
		newState = new vector<char>[3];
		for (int i=0; i<3; ++i)
			newState[i] = state[i];
		
		//compute new state for move from tower 3 to 1
		newState[0].push_back(newState[2].back());
		newState[2].pop_back();
		result.push_back(newState);
	}
	if (state[1].size() != 0){
		//deep copy input state to newState
		newState = new vector<char>[3];
		for (int i=0; i<3; ++i)
			newState[i] = state[i];
		
		//compute new state for move from tower 2 to 3
		newState[2].push_back(newState[1].back());
		newState[1].pop_back();
		result.push_back(newState);
		
		//deep copy input state to newState
		newState = new vector<char>[3];
		for (int i=0; i<3; ++i)
			newState[i] = state[i];
		
		//compute new state for move from tower 2 to 1
		newState[0].push_back(newState[1].back());
		newState[1].pop_back();
		result.push_back(newState);
	}
	if (state[0].size() != 0){
		//deep copy input state to newState
		newState = new vector<char>[3];
		for (int i=0; i<3; ++i)
			newState[i] = state[i];
		
		//compute new state for move from tower 1 to 3
		newState[2].push_back(newState[0].back());
		newState[0].pop_back();
		result.push_back(newState);
		
		//deep copy input state to newState
		newState = new vector<char>[3];
		
		for (int i=0; i<3; ++i)
			newState[i] = state[i];
		
		//compute new state for move from tower 1 to 2
		newState[1].push_back(newState[0].back());
		newState[0].pop_back();
		result.push_back(newState);
	}
	return result;
}

////////////////////////////////////////////////////////////////
//	Function name:	stateToString
//	Description: 	Creates unique string for a given state.
//	Parameters:		vector<char> *state: state to create string for
//	Return Value:	string - string form of given state
////////////////////////////////////////////////////////////////
string stateToString(vector<char> *state){
	string out = "1";
	out += string(state[0].begin(), state[0].end());
	out += "2";
	out += string(state[1].begin(), state[1].end());
	out += "3";
	out += string(state[2].begin(), state[2].end());
	return out;
}

class Node {
	public:
	Node(vector<char> *s, Node *p){	//constructor with state and parent as parameters
		state = s;
		parent = p;
		maxFront = 0;
		nodesExp = 0;
		
		if (p != NULL){
			depth = parent->depth + 1;
		}
		else{
			depth = 0;
		}
	}
	
	vector<char> *state; //state inside of node
	Node *parent;
	double depth;
	int maxFront; //maximum frontier
	double nodesExp; //nodes expanded
};

////////////////////////////////////////////////////////////////
//	Function name:	bfs
//	Description: 	Runs the bfs algorithm until a solution is found.
//	Parameters:		vector<char> *start: starting state
//					vector<char> *goal: goal state
//	Return Value:	Node - solution node with final values for depth, maxFront, etc.
////////////////////////////////////////////////////////////////
Node bfs(vector<char> *start, vector<char> *goal){
	int maxF = 0;
	int nodesE = 0;
	
	//create frontier and add the start node
	queue<Node> frontier;
	Node begin = Node(start, NULL);
	
	frontier.push(begin);
	
	unordered_set<string> reached;
	reached.insert(stateToString(start));	//insert start converted to a string
	
	while (!(frontier.empty())){
		Node current = frontier.front();  
		
		//keeps track of frontier max length
		if (frontier.size() > maxF){
			maxF = frontier.size();
			current.maxFront = maxF;
		}
		else{
			current.maxFront = maxF;
		}
		
		frontier.pop();
		
		//does current node contain the goal state?
		if (current.state[0] == goal[0] && current.state[1] == goal[1] &&
				current.state[2] == goal[2])
		{
			current.nodesExp = nodesE;
			return current;
		}

		auto result = successors(current.state);	//get successors
		
		while(!result.empty()){ //create node for each successor
			Node child = Node(result.back(), &current);	
			
			result.pop_back();
			nodesE += 1;
			
			//add node to frontier if it doesn't exist in reached
			if (reached.count(stateToString(child.state)) == 0){
				reached.insert(stateToString(child.state));
				frontier.push(child);
			}
		}
	}
	return Node(NULL, NULL);
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " [file_name.dat]\n";
		exit(EXIT_FAILURE);
	}
	
	fstream ifs;
	ifs.open(argv[1]);
	
	if (!ifs) { 
		cout << "There was an error reading the file: " << argv[1] << endl;
		ifs.close();
		return 0;
	}
	
	vector<char> *start = new vector<char>[3];	//holds the start game state
	vector<char> *goal = new vector<char>[3];	//holds the goal game state
	start = readFile(start, ifs);	//get start state
	goal = readFile(goal, ifs);		//get goal state
	
	ifs.close();
	
	cout << "\nSTART\n";
	printTowers(start);
	cout << "\nGOAL\n";
	printTowers(goal);
	
	Node finish = bfs(start, goal);
	
	cout << "\nSolution found" << endl;
	cout << "\nLengh of the solution: " << finish.depth << endl;
	cout << "Max queue length: " << finish.maxFront << endl;
	cout << "Expansion count: "  << setprecision(16) << finish.nodesExp << endl;
	cout << "Effective Branching Factor: " << endl;
	cout << setprecision(16) << fixed << pow((finish.nodesExp), (1/(finish.depth))) << "\n\n";
	
	delete[] start;
	delete[] goal;
	return 0;
}