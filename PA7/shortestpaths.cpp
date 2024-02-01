/*******************************************************************************
 * Name          : shortestpaths.cpp
 * Author        : Alisha Patel
 * Pledge        : I pledge my honor that I have abided by the Stevens Honor System.
 * Date          : 12/5/22
 * Description   : To solve all the pairs shortest paths problem with Floyd's algorithm.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <limits>
#include <climits>

using namespace std;


long INF = LONG_MAX/2;
int n = 0;

int len(long num) {
    int counter = 0;
    long longNum = num;
    if(num == 0){
        return 1;
    }
    while(longNum >= 1) {
        longNum = longNum / 10;
        counter++;
    }
    return counter;
}


/**
 * Display the matrix on the screen formatted as a table.
 */
void display_table(long** const matrix, const string &label, const bool use_letters = false){
	cout << label << endl;
	long max_val = 0;
	for (int i = 0; i < num_vertices; i++){
		for(int j = 0; j < num_vertices; j++){
			long cell = matrix[i][j];
			if (cell < INF && cell > max_val){
				max_val = matrix[i][j];
			}
		}
	}
	int max_cell_width = use_letters ? len(max_val) :
			len(max(static_cast<long>(num_vertices), max_val));
	cout << ' ';
	for (int j = 0; j < num_vertices; j++) {
		cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
	}
	cout << endl;
	for (int i = 0; i < num_vertices; i++) {
		cout << static_cast<char>(i + 'A');
		for (int j = 0; j < num_vertices; j++) {
			cout << " " << setw(max_cell_width);
			if (matrix[i][j] == INF) {
				cout << "-";
			}
			else if (use_letters) {
				cout << static_cast<char>(matrix[i][j] + 'A');
			}
			else {
				cout << matrix[i][j];
			}
	    }
	    cout << endl;
	}
	cout << endl;
}

//creates a matrix initialized with INF and 0s
long** create_matrix(int num_vertices){
	long** matrix = new long*[num_vertices];
	for(int i = 0; i < num_vertices; i++){
		matrix[i] = new long[num_vertices];
	}

	for(int j = 0; j < num_vertices; j++){
		for(int k = 0; k < num_vertices; k++){
			if(j == k){
				matrix[j][k] = 0;
			}
			else{
				matrix[j][k] = INF;
			}
		}
	}
	return matrix;
}

//creates the distance matrix with INFs
long** int_distance(int num_vertices){
	long** matrix = new long*[num_vertices];
	for (int i = 0; i < num_vertices; i++){
		matrix[i] = new long[num_vertices];
	}
	for(int j = 0; j < num_vertices; j++){
		for (int k = 0; k < num_vertices; k++){
			matrix[j][k] = INF;
		}
	}
	return matrix;
}



void floyds(long** &dist, long** & inter){
	//path length equals the distance matrix
	long** p_length = dist;

	for(int k = 0; k < num_vertices; k++){
		for(int i = 0; i < num_vertices; i++){
			for (int j = 0; j < num_vertices; j++){
				if((dist[i][j] > dist[i][k] + dist[k][j])){
					inter[i][j] = k;
				}
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[j][k]);
			}
		}
	}
}

//backtracks to see which paths were selected
string path_display_helper(long** &dist, long** &intermediate, int i, int j){
	string path = "";

	if (dist[i][j] == INF){
		return "none";
	}
	else if(intermediate[i][j] == INF){
		path.push_back(static_cast<char>('A' + i));
		if(static_cast<char>('A' + j) != static_cast<char>('A' + i)){
			path.push_back(static_cast<char>('A' + j));
		}
		return path;
	}
	else{
		path = path_display_helper(dist, intermediate, i, intermediate[i][j]);
		path.append(path_display_helper(dist, intermediate, intermediate[i][j], j));
		return path;
	}
}

void path_display(long** dist, long** intermediate){

	for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cout << static_cast<char>(i + 'A') << " -> " << static_cast<char>(j + 'A') << ", distance: " << flush;
				if (dist[i][j] == INF) {
					cout << "infinity, path: none" << endl;
					continue;
				} else {
					string path = path_display_helper(dist, intermediate, i, j);
					if (i == j) {
						cout << "0, path: " << static_cast<char>(i + 'A') << endl;
						continue;
					}
					cout << dist[i][j] << ", path: " << static_cast<char>(i + 'A') << " -> " << flush;
					for (unsigned int k = 1; k < path.length(); k=k+2) {
						if (k == path.length()-1) {
							cout << path[k] << flush;
						} else {
							cout << path[k] << " -> " << flush;
						}
					}
					cout << endl;
				}
			}
	}
}

void deletematrices(long** m) {
	// deleting distance matrix
	for(int i = 0; i < n; i++){
        delete[] m[i];
	}
    delete[] m;

}


int n = 0;




int main(int argc, const char *argv[]) {
	// checking parameters
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    // opening file for reading
    ifstream input_file(argv[1]);
    // if error
    if (!input_file) {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }

    string line;
    unsigned int line_number = 1;
    // getting the number of vertices
    getline(input_file, line);
    stringstream ss(line);
    if((!(ss >> n)) || (n < 1 || n > 26)) {
        cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
        return 1;
    }
    //initializing distance matrix
    long **matrix = new long*[n];
    for(int i = 0; i < n; i++) {
        matrix[i] = new long[n];
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matrix[i][j] = INF;
        }
    }
    // setting diagonal entries to 0 becuase distance from x to x is 0
    for(int i = 0; i < n; i++){
        matrix[i][i] = 0;
    }
    //initializing intermediate matrix
    long **inter = new long*[n];
    for(int i = 0; i < n; i++) {
        inter[i] = new long[n];
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            inter[i][j] = INF;
        }
    }
    line_number = 2;
    while (getline(input_file, line)) {
    	// checking for invalid data
    	if(line.size() < 5) {
    		cerr << "Error: Invalid edge data '" << line << "' on line " << line_number << "." << endl;
    		return 1;
    	}
    	// checking string contents by using substrings
    	char upper = (n+64);
    	size_t l = 0;
    	// vertex 1
    	string v1 = line.substr(0, line.find(" ", 0));
    	if((v1.length() != 1) || ((v1[0]-65) < 0 || (v1[0]-65) > n)) {
    		cerr << "Error: Starting vertex '" << v1 << "' on line " << line_number <<" is not among valid values A-" << upper << "." << endl;
    		return 1;
    	}
    	l = line.find(" ", l) + 1;
    	//vertex 2
    	string v2 = line.substr(l, line.find(" ", l) - l);
    	if((v2.length() != 1) || ((v2[0]-65) < 0 || (v2[0]-65) > n)) {
    	    cerr << "Error: Ending vertex '" << v2 << "' on line " << line_number <<" is not among valid values A-" << upper << "." << endl;
    		return 1;
       	}
    	l = line.find(" ", l) + 1;
    	// distance between vertices
    	int d;
    	string distance = line.substr(l, line.find(" ", l) - l);
    	stringstream isss(distance);
    	if((!(isss >> d)) || (d <= 0)) {
    		cerr << "Error: Invalid edge weight '" << distance << "' on line " << line_number << "." << endl;
    		return 1;
    	}
    	isss.clear();

        size_t last = 0;
        int i, j, k;
        string vertex1 = line.substr(0, line.find(" ", 0));
        i = vertex1[0] - 65;
        last = line.find(" ", last) + 1;
        string vertex2 = line.substr(last, line.find(" ", last) - last);
        j = vertex2[0] - 65;
        last = line.find(" ", last) + 1;
        string dist = line.substr(last, line.find(" ", last) - last);
        stringstream iss(dist);
        iss >> k;
        // setting the value in distance matrix for i,j
        matrix[i][j] = k;
        iss.clear();
        line_number++;
    }

    display_table(matrix, "Distance matrix:");
    floyds(matrix, inter);
    display_table(matrix, "Path lengths:");
    display_table(inter, "Intermediate vertices:", true);
    path_display(matrix, inter);



    deletematrices(matrix);

    deletematrices(inter);


    // deleting distance matrix
    for(int i = 0; i < n; i++){
        delete[] matrix[i];
    }
    delete[] matrix;
    // deleting intermediate matrix
    for(int i = 0; i < n; i++){
        delete[] inter[i];
    }
    delete[] inter;

    // closing input file
    input_file.close();

    return 0;
}
