#include <iostream>
#include <chrono>
#include <fstream>
using namespace std;
using namespace std::chrono;

static void swap(int array[], const int a, const int b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

void bubble_sort(int array[], const int length) {
    for(int i = 0; i <= length - 2; i++) {
        for(int j = 0; j <= length - 2 - i; j++) {
            if(array[j + 1] < array[j]) {
               swap(array, j, j + 1);
            }
        }
    }
}

void bubble_sort_opt(int array[], const int length) {
    int unsorted = length;
    while(unsorted > 0) {
        int s = 0;
        for(int j = 1; j < unsorted; j++) {
            if(array[j] < array[j - 1]) {
                swap(array, j - 1, j);
                s = j;
            }
        }
        unsorted = s;
    }
}

void selection_sort(int array[], const int length) {
    for(int i = 0; i < length - 1; i++) {
        int min_j = i;
        for(int j = i + 1; j < length; j++) {
            if(array[j] < array[min_j]) {
                min_j = j;
            }
        }
        if(min_j != i) {
            swap(array, i, min_j);
        }
    }
}

void insertion_sort(int array[], const int length) {
    for(int i = 1; i < length; i++) {
        int j, current = array[i];
        for(j = i - 1; j >= 0 && array[j] > current; j--) {
            array[j + 1] = array[j];
        }
        array[j + 1] = current;
    }
}

int main() {
    // Initialize the pseudo-random number generator.
    // Using rand/srand is not good in statistical or cryptographic
    // applications but good enough here to test sorting.
    srand(time(0));
    ofstream file;
    file.open("elementarysorting.csv");
    file << "n,BubbleSort,BubbleSortOpt,SelectionSort,InsertionSort" << endl;
    for(int n = 0; n <= 20000; n+= 1000) {
        file << n << ",";
        int *src = new int[n];
        int *copy = new int[n];
        for(int i = 0; i < n; i++) {
            copy[i] = src[i] = rand();
        }

        auto start = high_resolution_clock::now();
        bubble_sort(copy, n);
        auto stop = high_resolution_clock::now();
        duration<double, std::milli> duration = stop - start;
        file << duration.count() << ",";

        for(int i = 0; i< n; i++) {
            copy[i] = src[i];
        }
        start = high_resolution_clock::now();
        bubble_sort_opt(copy, n);
        stop = high_resolution_clock::now();
        duration = stop - start;
        file << duration.count() << ",";

        for(int i = 0; i< n; i++) {
            copy[i] = src[i];
        }
        start = high_resolution_clock::now();
        selection_sort(copy, n);
        stop = high_resolution_clock::now();
        duration = stop - start;
        file << duration.count() << ",";

        for(int i = 0; i< n; i++) {
            copy[i] = src[i];
        }
        start = high_resolution_clock::now();
        insertion_sort(copy, n);
        stop = high_resolution_clock::now();
        duration = stop - start;
        file << duration.count() << endl;
        
        delete [] src;
        delete [] copy;
    }
    file.close();
    return 0;
}
