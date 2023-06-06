#include "LFU.hpp"
int main()
{
    // Example usage of LFUCache
    LFUCache cache(2);

    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << endl; // Output: 1
    cache.put(3, 3);
    cout << cache.get(2) << endl; // Output: -1 (not found)
    cout << cache.get(3) << endl; // Output: 3
    cache.put(4, 4);
    cout << cache.get(1) << endl; // Output: -1 (not found)
    cout << cache.get(3) << endl; // Output: 3
    cout << cache.get(4) << endl; // Output: 4

    return 0;
}