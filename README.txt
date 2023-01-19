
bfs.cpp

For my breadth first function, I ended up using arrays of vectors to
represent a state of the game. By doing this, I could easily deep copy them to
to create multiple successors with ease. I could then add them to 
another array, which stores the states, and return it. To create my reached
list, I decided to use an unordered_set of strings. By creating a string from
each state, each one could be unique. This is what I used to check for 
reached states in constant time.


astar.cpp

The A* algorithm I decided to implement was a slight change off what I did 
for the breadth first search. Some differences include adding a cost variable
to the Node class, overloaded the < operator, and creating a priority queue 
that contained Nodes with the lowest cost on top. In this queue, by overloading 
the < operator to return the result of a > operation, it would put the lowest 
cost value on top. I would use the lowest cost value in order to determine which 
state I would check.
The way I made my heuristics function is by comparing an inputted state with the
goal state and determining how many elements matched between the two. The  
heuristic started with 2 times the total number of elements on the towers. For 
every element in the inputted state that was in the same position as the goal 
state, it would be decreased by 2. One thing I noticed is that if there 
are only one or two nodes on a tower, the heuristic may be one (or more) moves 
less. This increased the efficiency of my program by slightly more than double 
(in terms of speed). It may be deterministic based on the number of towers and
elements, but the test cases do not expand enough for the need to compute it.
