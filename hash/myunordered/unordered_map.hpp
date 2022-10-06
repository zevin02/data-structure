#pragma once
#include "HashTable.hpp"

namespace xzw
{

    template <class K, class V>
    class unordered_map
    {
        struct MapKeyOfT
    {
        const K& operator()(const pair<K, V> &key)
        {
            return key.first;
        }
    };
    private:
        HashTable<K, pair<K, V>, MapKeyOfT> _ht;

    public:
        typedef typename HashTable<K, pair<K,V>, MapKeyOfT, Hash<K>>::iterator Iterator;
    
        bool insert(const pair<K, V> &key)
        {
            return _ht.Insert(key);
        }
    };
};