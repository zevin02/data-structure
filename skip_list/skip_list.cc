#include <iostream>
#include "skip_list.hpp"

using namespace std;

int main() {
    // Create a skip list with some keys
    skipList sl;
    sl.insert(1);
    sl.insert(4);
    sl.insert(4);
    sl.insert(5);
    sl.insert(8);
    sl.insert(10);
    
    // Perform a range search
    vector<int> result = sl.rangeSearch(4, 8);
    
    // Print the result
    cout << "Range search result: ";
    for (int key : result) {
        cout << key << " ";
    }
    cout << endl;
    
    return 0;
}