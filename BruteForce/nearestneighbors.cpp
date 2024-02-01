/**
 * What is a brute force algorithm?
 *
 * A brute force algorithm is one that tests all possible candidates
 * for meeting a certain criterion.
 *
 * It is not necessarily the best algorithm but is often the simplest one
 * and often works well enough for small input sizes.
 * It's tied closely to the concept of exhaustive search.
 *
 * Nearest-neighbor problem:
 *
 * x
 *                     x
 *                             x
 *                         x
 *                                                x
 *                  x
 *     x
 *                                         x
 *
 * Imagine each x represents the GPS coordinates of a person. We want to
 * find the 2 people who are closest to one another.
 */
#include <cmath>    // For sqrt.
#include <iostream>
#include <limits>   // For numeric_limits<float>::max().
#include <string>
#include <utility>  // For pairs.
using namespace std;

// Using a structure, which is very much like a class except that members
// are public by default while in a class they are private by default.
struct Neighbor {
    float x, y;
    string name;

    Neighbor(float x_, float y_, string name_) : x{x_}, y{y_}, name{name_} { }
};

float distance(float x1, float y1, float x2, float y2) {
    float diff_x = x2 - x1, diff_y = y2 - y1;
    return diff_x * diff_x + diff_y * diff_y;
}

pair<Neighbor, Neighbor> nearest_neighbors(Neighbor neighbors[], const int length) {
    float min_distance = numeric_limits<float>::max();
    int min_i, min_j;
	// Selection Sort: for every position i in the array, find the smallest
	// element in the rest of the array and move it to position i.
	// Here: for every position i in the array, find the nearest
	// element in the rest of the array and remember that pair of elements
	// if they have the smallest current distance.
	// Selection Sort and this algorithm are therefore very similar.
    for(int i = 0; i < length - 1; i++) {
        for(int j = i + 1; j < length; j++) {
            float cur_distance = distance(neighbors[i].x, neighbors[i].y,
                                          neighbors[j].x, neighbors[j].y);
            if(cur_distance < min_distance) {
                min_distance = cur_distance;
                min_i = i;
                min_j = j;
            }
        }
    }
    return make_pair(neighbors[min_i], neighbors[min_j]);
}

int main() {
	//create an array of neighbors
    Neighbor neighbors[4] = {Neighbor(1, 1, "A"),
                             Neighbor(10.5, 111, "B"),
                             Neighbor(121, 111.1, "C"),
                             Neighbor(121.5, 111.2, "D")
                            };
    pair<Neighbor, Neighbor> result = nearest_neighbors(neighbors, 4);
    cout << result.first.name << ", " << result.second.name << endl;
	// We need to remember to take the square root here since the distance
	// function in fact returns the square of the distance.
    cout << "Distance: " << sqrt(distance(result.first.x, result.first.y,
                                          result.second.x, result.second.y))
         << endl;
    return 0;
}
