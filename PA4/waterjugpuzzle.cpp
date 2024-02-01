/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Birva Patel, Alisha Patel
 * Date        : October 22, 2022
 * Description : Solves the water jug puzzle using breadth first search
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

// Struct to represent state of water in the jugs.
struct State {
    int a, b, c;
    string directions;                                                  // directions string stores the particular step which was carried out
    vector<string> path;                                                // vector path stores the order of string instructions from root to goal state.

    State(int _a, int _b, int _c, string _directions) :                 // constructor for structure
        a{_a}, b{_b}, c{_c}, directions{_directions} { }

    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};

int argument[6];                                                        // saves user input as an array

void bfs(int A, int B, int C) {

	// getting the capacities and goals from input array:
	int capA = argument[0], capB = argument[1], capC = argument[2];
	int goalA = argument[3], goalB = argument[4];

	queue<State> Q;                                                     // queue to use for bfs traversal

	ostringstream oss;
	State* root = new State(A, B, C, "Initial State. ");                // creating root state using dynamic allocation
	oss << "Initial state. " << root->to_string();
	root->directions = oss.str();                                       // creating direction string for the initial state
	oss.clear();
	root->path.push_back(root->directions);                             // pushing direction to path vector

	Q.push(*root);                                                      // pushing root to queue
	delete root;                                                        // deleting root

	// CREATING A 2D BOOLEAN ARRAY WITH DYNAMIC DIMENSIONS
	bool **array = new bool*[capA+1];
	for(int i = 0; i < (capA+1); i++) {
		array[i] = new bool[capB+1];
		fill(array[i], array[i] + (capB+1), false);
	}

	while(!Q.empty()) {                                                 // starting bfs traversal

		State current = Q.front();                                      // saving front element of queue to current

		A = current.a;                                                  // saving the current water in the jugs to A, B, C
		B = current.b;
		C = current.c;

		if ( (A == goalA) && (B == goalB) ) {                           // if this state is the goal state, print solution and return
			for(size_t i = 0; i < current.path.size(); i++) {
				cout << current.path[i] << endl;
			}
			// DELETING 2D ARRAY
			for(int i = 0; i < capA+1; i++) {
				delete [] array[i];
			}
			delete [] array;
			return;
		}

		Q.pop();                                                         // pop the front element of queue
		array[A][B] = true;                                             // marking the current states as "visited" in boolean array

		// STARTING THE WATER JUG STEPS IN SPECIFIED ORDER:
		int newA, newB, newC;

		//  1. POUR FROM C TO A:
		if (A < capA) {                                                  // only possible if A is not at full capacity
			newA = A + C;
			if (newA > capA) {                                           // set the newA and newC amounts
				newA = capA;
			}
			newC = C - (newA - A);
			if (array[newA][B] != true) {
				State* child = new State(newA, B, newC, "str");          // creating this new child state
				ostringstream oss;
				int poured = (newA - A);                                   // calculate gallons poured for output string
				if (poured == 1)                                         // adjusting formatted output string
					oss << "Pour 1 gallon from C to A. " << child->to_string();
				else
					oss << "Pour " << poured << " gallons from C to A. " << child->to_string();
				child->directions = oss.str();                           // saving the direction string for this step
				child->path = current.path;
				child->path.push_back(child->directions);                // appending it to its path vector
				Q.push(*child);                                          // pushing it to queue
				oss.clear();
				delete child;
			}
		}

		// 2. POUR FROM B TO A
		if (A < capA) {                                                  // only possible if A not at full capacity already
			newA = A + B;
			if (newA > capA) {
				newA = capA;
			}
			newB = B - (newA - A);
			if (array[newA][newB] != true) {
				State* child = new State(newA, newB, C, "str");
				ostringstream oss;
				int poured = (newA - A);
				if (poured == 1)
					oss << "Pour 1 gallon from B to A. " << child->to_string();
				else
					oss << "Pour " << poured << " gallons from B to A. " << child->to_string();
				child->directions = oss.str();
				child->path = current.path;
				child->path.push_back(child->directions);
				Q.push(*child);
				oss.clear();
				delete child;
			}
		}

		// 3. POUR FROM C TO B
		if (B < capB) {                               // only possible if B not at full capacity already
			newB = B + C;
			if (newB > capB) {
				newB = capB;
			}
			newC = C - (newB-B);
			if (array[A][newB] != true) {
				State* child = new State(A, newB, newC, "str");
				ostringstream oss;
				int poured = (newB-B);
				if(poured == 1)
					oss << "Pour 1 gallon from C to B. " << child->to_string();
				else
					oss << "Pour " << poured << " gallons from C to B. " << child->to_string();
				child->directions = oss.str();
				child->path = current.path;
				child->path.push_back(child->directions);
				Q.push(*child);
				oss.clear();
				delete child;
			}
		}

		// 4. POUR FROM A TO B
		if (B < capB) {                               // only possible if B not at full capacity already
			newB = B + A;
			if (newB > capB) {
				newB = capB;
			}
			newA = A - (newB-B);
			if (array[newA][newB] != true) {
				State* child = new State(newA, newB, C, "str");
				ostringstream oss;
				int poured = (newB-B);
				if(poured == 1)
					oss << "Pour 1 gallon from A to B. " << child->to_string();
				else
					oss << "Pour " << poured << " gallons from A to B. " << child->to_string();
				child->directions = oss.str();
				child->path = current.path;
				child->path.push_back(child->directions);
				Q.push(*child);
				oss.clear();
				delete child;
			}
		}


		// 5. POUR FROM B TO C
		if (C < capC) {                               // only possible if C not at full capacity already
			newC = C + B;
			if (newC > capC) {
				newC = capC;
			}
			newB = B - (newC-C);
			if (array[A][newB] != true) {
				State* child = new State(A, newB, C, "str");
				ostringstream oss;
				int poured = (newC-C);
				if(poured == 1)
					oss << "Pour 1 gallon from B to C. " << child->to_string();
				else
					oss << "Pour " << poured << " gallons from B to C. " << child->to_string();
				child->directions = oss.str();
				child->path = current.path;
				child->path.push_back(child->directions);
				Q.push(*child);
				oss.clear();
				delete child;
			}
		}

		// 6. POUR FROM A TO C
		if (C < capC) {                               // only possible if C not at full capacity already
			newC = C + A;
			if (newC > capC) {
				newC = capC;
			}
			newA = A - (newC-C);
			if (array[newA][B] != true) {
				State* child = new State(newA, B, newC, "str");
				ostringstream oss;
				int poured = (newC-C);
				if(poured == 1)
					oss << "Pour 1 gallon from A to C. " << child->to_string();
				else
					oss << "Pour " << poured << " gallons from A to C. " << child->to_string();
				child->directions = oss.str();
				child->path = current.path;
				child->path.push_back(child->directions);
				Q.push(*child);
				oss.clear();
				delete child;
			}
		}// END OF  TREE BRANCHING

	} // end of while loop

	// if loop reaches here, means goal state was never encountered
	cout << "No solution." << endl;

	// DELETING 2D ARRAY
	for(int i = 0; i < capA+1; i++) {
		delete [] array[i];
	}
	delete [] array;
}


int main(int argc, char* argv[]) {

	// ERROR CHECKING:
	if(argc != 7) {
		cerr << "Usage: " << argv[0] << " <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
		return 1;
	}
	char c = 'A';
	for (int i = 1; i <= 3; i++) {
		istringstream iss_i, iss_j;
		iss_i.str(argv[i]);
		iss_j.str(argv[i+3]);
		if(!(iss_i >> argument[i-1]) || argument[i-1] <= 0) {
				cerr << "Error: Invalid capacity '" << argv[i] << "' for jug " << c << "." << endl;
				return 1;
		}
		if(!(iss_j >> argument[i+2]) || argument[i+2] < 0) {
	            cerr << "Error: Invalid goal '" << argv[i+3] << "' for jug " << c << "." << endl;
				return 1;
		}
		c++;
		iss_i.clear();
		iss_j.clear();
	}
	c = 'A';
	for (int i = 0; i <= 2; i++) {
		if (argument[i+3] > argument[i]) {
			cerr << "Error: Goal cannot exceed capacity of jug " << c <<"." << endl;
			return 1;
		}
		c++;
	}
	int sum = argument[3]+argument[4]+argument[5];
	if (sum != argument[2]) {
		cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
		return 1;
	}

	bfs(0, 0, argument[2]);

	return 0;

}

/*
string bfs(int a, int b, int c)

add new State(a, b, c) to queue
while (queue is not empty)
current <- front of queue
pop from the queue
if current equals goal_state
return the backtracked solution
if current has already been seen
continue
mark current as having been visited
try the 6 ways to pour water, pushing new States to the queue
return "no solution"

print bfs(0, 0, 8)
 */
