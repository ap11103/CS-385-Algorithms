/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Alisha Patel
 * Date        : September 14, 2022
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    int num_primes() const {
        return num_primes_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;

    // Method declarations
    int count_num_primes() const;
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    sieve();
}

void PrimesSieve::display_primes() const {
    // TODO: write code to display the primes in the format specified in the
    // requirements document.
	const int max_prime_width = num_digits(max_prime_),
	          primes_per_row = 80 / (max_prime_width + 1);

	//iterate through primes while also keeping track of how many primes can be in a row
	int counter = 0;

	//If the number of primes are greater than primes_per_row,
	//it would be displayed through more than one row
	if(num_primes_ > primes_per_row){
		for(int i = 2; i <= limit_; i++){
			if(counter < primes_per_row){
				//If the number is prime and less than the max prime
				//it should reach to the last value of the row and print
				if((is_prime_[i] == true) && (i < max_prime_)){
					if(counter == (primes_per_row - 1)){
						cout << setw(max_prime_width) << i;
						counter++;
					}
					//not the max prime, then print with the space
					else{
						cout << setw(max_prime_width) << i << " ";
						counter++;
					}
				}
				else{
					if(is_prime_[i] == true){
						cout << setw(max_prime_width) << i;
						counter++;
					}
				}
			}
			//reset the counter after all the prime numbers are printed
			else if (counter == primes_per_row){
				counter = 0;
				cout << endl;
			}
		}
	}
	else{
		// if the num_primes is less than primes_per_row
		//all the primes should be printed on the same line
		for(int i = 2; i <= limit_; i++){
			if(is_prime_[i] == true){
				if(i == max_prime_){
					//since it's the last prime #, no space needed
					cout << i;
				}
				else{
					cout << i << " ";
				}
			}
		}
	}
}



int PrimesSieve::count_num_primes() const {
    // TODO: write code to count the number of primes found

	int counter = 0;
	for (int i = 2; i <= limit_; i++){
		// if the array contains the value T, it increments the counter
		if(is_prime_[i] == true){
			counter++;
		}
	}
	return counter;


}

void PrimesSieve::sieve() {
    // TODO: write sieve algorithm

	//To set all the values in is_prime_ array true
	//to be iterated later
	for(int i = 2; i <= limit_; i++){
		is_prime_[i] = true;
	}

	//implements the pseudocode provided
	for(int i = 2; i <= sqrt(limit_); i++){
		if(is_prime_[i]){
			for(int k = i*i; k <= limit_; k+=i){
				is_prime_[k] = false;
			}
		}
	}

	//iterates through the limit, and checks if it is a prime number
	//if it is, then updates max_prime_
	for(int i = 2; i <= limit_; i++){
		if (is_prime_[i] == true){
			max_prime_ = i;
		}
	}
	//calls the count_num_primes() to assign the counter value to num_primes
	num_primes_ = count_num_primes();


}

int PrimesSieve::num_digits(int num) {
    // TODO: write code to determine how many digits are in an integer
    // Hint: No strings are needed. Keep dividing by 10.

	//initializes the counter, if it's not 0, it keeps dividing by 10
	int counter = 0;
	while(!(num == 0)){
		num = num/10;
		counter++;
	}
	return counter;

}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

    cout << endl;


    // TODO: write code that uses your class to produce the desired output.
    PrimesSieve test(limit);
    cout<< "Number of primes found: " << test.num_primes() << endl;
    cout << "Primes up to " << limit << ":" << endl;
    test.display_primes();

    return 0;
}
