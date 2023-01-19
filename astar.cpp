////////////////////////////////////////////////////////////////
//	Name:			Ryan Quinn
//	Major:			Computer Science
//	Creation Date: 	9/29/21
//	Due Date:		10/1/21
//	Course:			CSC447 010
//	Professor Name:	Dr. Schwesinger
//	Assignment: 	1
//	Filename: 		astar.cpp
//	Purpose:		Create an A* search for the towers of hanoi
//					problem given a solution state.
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
//	Return Value:	vector<char> * - vector array holding the 3 tower levels
////////////////////////////////////////////////////////////////
vector<char>* readFile(vector<char> *state, fstream &ifs){
	string fileLine;
	
	for (int i=0; i<3; ++i){
		getline(ifs, fileLine);
		fileLine.append("\n");
		
		//pushes only tower elements into a vector for each level
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

////////////////////////////////////////////////////////////////
//	Function name:	heuristic
//	Description: 	Determines the heuristic value of a given state using
//					the number of matching elements between the given state
//					and the goal. 
//	Parameters:		vector<char> *state: state to determine heuristic for
//					vector<char> *goal: goal state
//	Return Value:	int - heuristic value
////////////////////////////////////////////////////////////////
int heuristic(vector<char> *state, vector<char> *goal){
	int h = (state[0].size() + state[1].size() + state[2].size());
	
	for (int i=0; i<3; ++i){
		for (int j=0; j<state[i].size() && j<goal[i].size(); ++j){
			if (state[i][j] == goal[i][j]){
				h -= 1;
			}
		}
	}
	if (h > 6 || h < 0) {
		cout << "inadmissible heuristic" << endl;
		printTowers(state);
		cout << '\n';
		printTowers(goal);
		cin.get();
	}
	return h;
}

class Node {
	public:
	Node(vector<char> *s, vector<char> *g, Node *p){	//constructor with state and parent as parameters
		state = s;
		parent = p;
		maxFront = 0;
		nodesExp = 0;
		
		
		if (p == NULL){	//first node?
			cost = heuristic(s, g);
		}
		else{	//inherits parent cost + state heuristic
			cost = parent->cost + heuristic(s, g);
			
		}
		if (p != NULL)
			depth = parent->depth + 1;
		else
			depth = 0;
	}
	
	//reverses priority_queue to have lowest cost on top
	bool operator<(const Node rhs) const{
		return cost > rhs.cost;
	}
	
	vector<char> *state; //state inside of node
	Node *parent;
	double depth;
	int maxFront; //maximum frontier
	double nodesExp; //nodes expanded
	int cost;
};

////////////////////////////////////////////////////////////////
//	Function name:	astar
//	Description: 	Runs the A* algorithm until a solution is found.
//	Parameters:		vector<char> *start: starting state
//					vector<char> *goal: goal state
//	Return Value:	Node - solution node with final values for depth, maxFront, etc.
////////////////////////////////////////////////////////////////
Node astar(vector<char> *start, vector<char> *goal){
	int maxF = 0;
	int nodesE = 0;
	
	//priority queue to keep track of lowest cost Node
	priority_queue<Node> path;
	Node node = Node(start, goal, NULL);
	
	path.push(node);
	
	unordered_set<string> reached;
	reached.insert(stateToString(start));	//insert start converted to a string
	
	while (!(path.empty())){
		Node current = path.top();  //lowest cost Node
		
		//keeps track of path max length
		if (path.size() > maxF){
			maxF = path.size();
			current.maxFront = maxF;
		}
		else{
			current.maxFront = maxF;
		}
		
		path.pop();
		
		//does current node contain the goal state?
		if (current.state[0] == goal[0] && current.state[1] == goal[1] &&
				current.state[2] == goal[2])
		{
			current.nodesExp = nodesE;
			return current;
		}
		
		auto result = successors(current.state);	//get successors
		
		while(!result.empty()){ //create node for each successor
			Node child = Node(result.back(), goal, &current);	
			result.pop_back();
			nodesE += 1;
			
			//add node to path if it doesn't exist in reached
			if (reached.count(stateToString(child.state)) == 0){
				reached.insert(stateToString(child.state));
				path.push(child);
			}
		}
	}
	return Node(NULL, NULL, NULL);
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
	
	vector<char> *start = new vector<char>[3];
	vector<char> *goal = new vector<char>[3];
	start = readFile(start, ifs);	//get start state
	goal = readFile(goal, ifs);		//get goal state
	
	ifs.close();
	
	cout << "\nSTART\n";
	printTowers(start);
	cout << "\nGOAL\n";
	printTowers(goal);
	
	cout << "\nCalculating..." << endl;
	Node finish = astar(start, goal);
	
	if (finish.state == NULL){
		cout << "Solution not found" << endl;
		return 0;
	}
	
	cout << "\nSolution found" << endl;
	cout << "\nLengh of the solution: " << finish.depth << endl;
	cout << "Max queue length: " << finish.maxFront << endl;
	cout << setprecision(10) << "Expansion count: " << finish.nodesExp << endl;
	cout << "Effective Branching Factor: " << endl;
	cout << setprecision(16) << fixed << pow((finish.nodesExp), (1/(finish.depth))) << "\n\n";
	
	delete[] start;
	delete[] goal;
	return 0;
}