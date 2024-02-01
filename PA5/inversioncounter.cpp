/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Alisha Patel
 * Version     : 1.0
 * Date        : 10/27/22
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/*
 * Counts the number of inversions in an array in Theta(n^2) time.
 */
long count_inversions_slow(int array[], int length) {
    // TODO
	//two nested loops to loop over i and j and compare array[i] and array[j]
	//if array[i] > array[j]; increment the number of inversions
	long inversions = 0;
	for (int i = 0; i < length - 1; i++){
		for (int j = i + 1; j < length; j++){
			if (array[i] > array[j]){
				inversions ++;
			}
		}
	}
	return inversions;
}

/**
 * Counts the number of inversions in an array in Theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
    // TODO
    // Hint: Use mergesort!

	//creates a scratch array based on the length of the array
	int *scratch = new int[length];
	long inversions = 0;
	//calls on the mergesort to count the inversions
	int high = length - 1;
	inversions = mergesort(array, scratch, 0, high);
	//delete the scratch pointer array created at the end of ounting inversions
	delete [] scratch;
	return inversions;
}




long merge(int array[], int scratch[], int low, int mid, int high){
	long inversions = 0;
	int i1 = low;
	int i2 = mid;
	int i = low;

	//Merge two sorted sub-arrays array[low...mid] and array[mid + 1, high]
	//uses the extra array scratch with the same size as array initialized count_inversion_fastttt

	while ((i1 <= (mid - 1)) && (i2 <= high)){
		if (array[i1] <= array[i2]){
			scratch[i++] = array[i1++];
		}
		else{
			scratch[i++] = array[i2++];
			//increment the inversions only when the first element is larger than the second
			inversions += (mid - i1);
		}
	}

	//from low to mid - 1; copy into scratch array
	for (int j = i1; j <= (mid - 1); j++){
		scratch[i++] = array[i1++];
	}

	//from mid to high; copy into scratch array
	for (int k = i2; k <= high; k++ ){
		scratch[i++] = array[i2++];
	}

	//copy the entire scratch array back into the original array
	for (int n = low; n <= high; n++){
		array[n] = scratch[n];
	}
	return inversions;
}

static long mergesort(int array[], int scratch[], int low, int high) {
    // TODO
	//initializing the values
	int mid;
	long inversions = 0;

	//
	if (low < high){
		mid = low + (high - low)/2;
		//splitting the arrays to [low, mid] and [mid+1, high]
		inversions += mergesort(array, scratch, low, mid);
		inversions += mergesort(array, scratch, (mid + 1), high);
		//call to the helper function
		inversions += merge(array, scratch, low, (mid + 1), high);

	}
	return inversions;
}

int main(int argc, char *argv[]) {
    // TODO: parse command-line argument

	bool slow = false;
	if (argc > 2){
		cerr << "Usage: ./inversioncounter [slow]" << endl;
		return 1;
	}

	//if there are 2 arguments; check if the first one is "slow"
	//if it is; it sets the slow boolean to true
	//else gives the error for unrecognized option
	if (argc == 2){
		istringstream oss;
		string arg;
		oss.str(argv[1]);
		oss >> arg;
		if (arg == "slow"){
			slow = true;
		}
		else{
			cerr << "Error: Unrecognized option '" << argv[1] << "'." << endl;
			return 1;
		}
		oss.clear();
	}


    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    // TODO: produce output

    long result;
    //if num of values is > 0; call the functions based on the boolean values set
    if (values.size() > 0){
    	if (slow == true){
    		//if slow is true; calls the slow count inversions
    		result = count_inversions_slow(&values[0], values.size());
    		cout << "Number of inversions: " << result << endl;
    	}
    	else{
    		//else calls the merge sort version of count inversions
    		result = count_inversions_fast(&values[0], values.size());
    		cout << "Number of inversions: " << result << endl;
    	}
    }
    else{
    	//if the num values is <= 0; the sequence is not given
    	cerr << "Error: Sequence of integers not received." << endl;
    	return 1;
    }


    return 0;
}
