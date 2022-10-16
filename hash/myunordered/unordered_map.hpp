#pragma once
#include "HashTable.hpp"

namespace xzw
{

    template <class K, class V,class hash=Hash<K>>
    class unordered_map
    {
        struct MapKeyOfT
        {
            const K &operator()(const pair<K, V> &key)
            {
                return key.first;
            }
        };

    private:
        HashTable<K, pair<K, V>, MapKeyOfT,Hash<K>> _ht;

    public:
        typedef typename HashTable<K, pair<K, V>, MapKeyOfT, Hash<K>>::iterator Iterator;
        Iterator begin()
        {
            return _ht.begin();
        }
        Iterator end()
        {
            return _ht.end();
        }
        pair<Iterator, bool> insert(const pair<K, V> &key)
        {
            return _ht.Insert(key);
        }
        V &operator[](const K &key)
        {
            auto ret = _ht.Insert(make_pair(key, V())); // V给缺省值
            return ret.first->second;
        }
    };
};