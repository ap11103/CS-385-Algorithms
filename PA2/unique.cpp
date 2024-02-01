/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Alisha Patel
 * Date        : 9/24/22
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>

using namespace std;

bool is_all_lowercase(const string &s) {
    // TODO: returns true if all characters in string are lowercase
    // letters in the English alphabet; false otherwise.

	//the for loop goes through each element of the string
	//if it is uppercase or not alphabetic return false
	//else return true
	bool x = true;
	//casts unsigned int for advanced looping
	for(unsigned int i = 0; i < s.length(); i++){
		if ((!isalpha(s[i]) || isupper(s[i]))){
			x = false;
		}
	}
	return x;


}

bool all_unique_letters(const string &s) {
    // TODO: returns true if all letters in string are unique, that is
    // no duplicates are found; false otherwise.

    // You MUST use only a single int for storage and work with bitwise
    // and bitshifting operators.  Using any other kind of solution will
    // automatically result in a grade of ZERO for the whole assignment.

	//if the length of the string is 0, return true
	if(s.length() == 0){
		return true;
	}

	//creates an unsigned int value for storage of the bits
	unsigned int vector = 0;
	//goes through the all the elements of the string
	for(unsigned int i = 0; i < s.length(); i++){
		//gets the distance between the letter and a
		int ascii = s[i] - 97;
		//shifts left by the the ascii value and if it's not 0 -> false
		//repeated value -> uses the and operator
		//masks with 1 << ascii
		if ((vector & (1 << ascii)) > 0){
			return false;
		}
		//else it would use the or operator to preserve the value
		vector |= (1 << ascii);
	}
	return true;

}

int main(int argc, char * const argv[]) {
    // TODO: reads and parses command line arguments.
    // Calls other functions to produce correct output.

	//if there's not total of 2 arguments -> error message
	if (argc != 2){
		cerr << "Usage: " << argv[0] << " <string>" << endl;
		return 1;
	}

	//if lowercase method of the argument returns false -> error message
	if(is_all_lowercase(argv[1]) == false){
		cerr << "Error: String must contain only lowercase letters." << endl;
		return 1;
	}

	//if the unique method of the argument returns false -> error message
	if(all_unique_letters(argv[1]) == false){
		cerr << "Duplicate letters found." << endl;
		return 1;
	}

	//if the unique method of the argument returns true -> cout message
	if(all_unique_letters(argv[1]) == true){
		cout << "All letters are unique." << endl;
	}



}
