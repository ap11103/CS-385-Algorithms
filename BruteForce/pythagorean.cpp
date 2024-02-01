#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

/**
 * Struct to represent a Pythagorean triple: three positive
 * integers a, b, and c such that a^2 + b^2 = c^2.
 */
struct Triple {
    int a, b, c;

    Triple(int a_, int b_, int c_) : a{a_}, b{b_}, c{c_} { }

    friend ostream& operator<<(ostream &os, const Triple &t) {
        os << t.a << "^2 + " << t.b << "^2 = " << t.c << "^2";
        return os;
    }
};

/**
 * Returns a vector of Pythagorean triples where all values (a, b, c) are
 * less than or equal to the limit.
 */
vector<Triple> find_pyth_triples(const int limit) {
    vector<Triple> result;
	
	for(int a = 1; a <= limit; a++) {
		for(int b = a + 1; b <= limit; b++) {
			for(int c = b + 1; c <= limit; c++) {
				if(a * a + b * b == c * c) {
					result.push_back(Triple(a, b, c));
				}
			}
		}
	}
	return result;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        cerr << "Usage: " << argv[0] << " <limit>" << endl;
        return 1;
    }
    int limit;
    istringstream iss;

    iss.str(argv[1]);
    if(!(iss >> limit) || limit < 3 ) {
        cerr << "Error: limit must be an integer >= 3." << endl;
        return 1;
    }
    vector<Triple> triples = find_pyth_triples(limit);
    for(const auto &triple : triples) {
        cout << triple << endl;
    }
    return 0;
}
