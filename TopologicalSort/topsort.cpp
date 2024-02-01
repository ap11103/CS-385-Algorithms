#include <iostream>
#include <vector>
#include <algorithm> // For the fill function.
#include <queue>
using namespace std;

// The graph argument really is a pointer to the array, but we can use
// it as if it were the array itself.
vector<int> toposort(vector<int> graph[], int V) {
    int *indegree = new int[V];
    // Initialize indegree to all zeroes:
    fill(indegree, indegree + V, 0);
    // Compute the indegree of each vertex.
    // We loop over every vertex v1 in the graph:
    for(int v1 = 0; v1 < V; v1++) {
        // then loop over every edge <v1, v2>:
        for(int v2: graph[v1]) {
            // and increase the corresponding indegree of v2:
            indegree[v2]++;
        }
        // Another way to do the same thing (note the use of size_t
        // for the loop index because that's the return type of the
        // size() method for a vector):
        //for(size_t j = 0; j < graph[v1].size(); j++) {
        //    int v2 = graph[v1][j];
        //    indegree[v2]++;
        //}
    }

    // Initialize the set S (using a queue):
    queue<int> S;
    for(int vertex = 0; vertex < V; vertex++) {
        if(indegree[vertex] == 0) {
            S.push(vertex);
        }
    }

    // Initialize the list L to be empty:
    vector<int> L;
    // and run the topological sort algorithm:
    while(!S.empty()) {
        int vertex = S.front(); // For queue.
        //int vertex = S.top(); // For priority queue.
        S.pop();
        L.push_back(vertex);
        for(int adjacent: graph[vertex]) {
            indegree[adjacent]--;
            if(indegree[adjacent] == 0) {
                S.push(adjacent);
            }
        }
    }

    // Check for a cycle in the graph:
    for(int vertex = 0; vertex < V; vertex++) {
        if(indegree[vertex] > 0) {
            // There is a cycle so whatever is in L is useless, so
            // clear L.
            L.clear();
            break;
        }
    }

    delete[] indegree;
    return L;
}

/* The expected input format is:
 *
 * number_of_vertices number_of_edges
 *
 * on the first input line, followed by one line for each edge with
 * the following format for each line:
 *
 * source_vertex destination_vertex
 *
 * Vertex numbers start at zero (to keep things simple).
 */
int main() {
    int V; // Number of vertices.
    int E; // Number of edges.
    cin >> V >> E;

    // Make an adjacency list in a simple, straightforward manner: an
    // array of vectors of integers.
    vector<int> *graph = new vector<int>[V];
    // For each edge e = <v1, v2>, we insert v2 into graph[v1].
    while(E--) {
        int v1, v2;
        cin >> v1 >> v2;
        graph[v1].push_back(v2);
    }
	// To keep things simple we assume that the edges are listed in
	// the input by increasing v2 (for a given v1), otherwise we would
	// have to sort here each vector one by one after reading the whole
	// input and before calling the toposort function.
	// Whether we sort or not, we still get a correct topological
	// order as result, but if we want to get the same result as in the
	// lecture notes then we need to sort if we cannot guarantee
	// that the input is already sorted in the right way.

    vector<int> order = toposort(graph, V);

    if(order.size() > 0) {
        for(int vertex: order) {
            cout << vertex << " ";
        }
        cout << endl;
    } else {
        cout << "toposort failed" << endl;
    }

    delete[] graph;
    return 0;
}
