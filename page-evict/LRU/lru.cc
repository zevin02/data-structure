#include"lru.hpp"
int main()
{
    LRU lru(3);
    lru.put(1,1);
    lru.put(2,2);
    lru.put(3,3);
    lru.put(4,4);
    return 0;
}