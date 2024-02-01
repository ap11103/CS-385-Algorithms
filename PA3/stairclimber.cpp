/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Alisha Patel
 * Date        : 10/1/22
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;



vector< vector<int> > get_ways(int num_stairs) {
    // TODO: Return a vector of vectors of ints representing
    // the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.


	vector<vector<int>> ways;
	vector<int> none;

	//if the number of stairs is less than 0, input empty
	if(num_stairs <= 0){
		ways.push_back(none);
	}
	else{
		//Since there are 3 ways that num_stairs can be climbed
		//goes through each of the possibility
		for (int i = 1; i < 4; i++){
			vector<vector<int>> result;
			//as long as the number of stairs s greater than the index
			//it will keep calling the function recursively subtracting i from stairs
			if (num_stairs >= i){
				//recursive call for n - i
				result = get_ways(num_stairs - i);
				//sorting the result before pushing into a vector from the back
				for(size_t j = 0; j < result.size(); j++){
					result[j].insert(result[j].begin(), i);
					ways.push_back(result[j]);
				}
			}
		}
	}
	return ways;

}


void display_ways(const vector< vector<int> > &ways) {
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.


	//increment i for creating space between index numbers for the list
	//to increment from 1 to 9
	int space = 0;
	for(unsigned int i = 1; i<= ways.size(); i*=9){
		space++;
	}

	for(unsigned int j = 0; j < ways.size(); j++){
		//uses the amount of space to format the start of the list
		//based on the number of ways to climb stairs
		cout << setw(space) << (j+1) << ". [";
		//iterates through the size of the vector and keep iterating
		//to print out all the elements in the list
		for(unsigned int k = 0; k < ways[j].size(); k++){
			//to indicate if it is the last index of the vector list
			if (k == ways[j].size() - 1){
				cout << ways[j][k] << "]" << endl;
			}
			//else spacing between the elements in the list
			else{
				cout << ways[j][k] << ", ";
			}
		}
	}



}



int main(int argc, char * const argv[]) {

	//if there's not 2 arguments given -> error statement
	if (argc != 2){
		cerr << "Usage: " << argv[0] << " <number of stairs>" << endl;
		return 1;
	}

	//If stairs is not an integer -> error statement
	istringstream iss(argv[1]);
	int stairs = 0;
	if(!(iss >> stairs)){
		cerr << "Error: Number of stairs must be a positive integer." << endl;
		return 1;
	}

	//error when the argument given is less than or equal to 0
	if(stairs <= 0){
		cerr << "Error: Number of stairs must be a positive integer." << endl;
		return 1;
	}

	vector<vector<int>> allways;
	//test case when there is only 1 stair
	allways = get_ways(stairs);
	if (stairs == 1){
		cout << "1 way to climb 1 stair." << endl;
		cout << "1. [1]";
		return 0;
	}

	else{
		//runs the actual function/output when the arguments are favorable
		cout << allways.size() << " ways to climb " << argv[1] << " stairs." << endl;
		//calls the display function to print the output correctly
		display_ways(allways);
		return 0;
	}


}
