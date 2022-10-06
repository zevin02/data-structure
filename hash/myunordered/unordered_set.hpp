#pragma once
#include "HashTable.hpp"

namespace xzw
{

    template <class K, class hash = Hash<K>> //因为这里我们已经引入了头文件了，所以Hash<K>也都被我们引入了

    class unordered_set
    {
        struct SetKeyOfT
        {
            const K &operator()(const K &key)
            {
                return key;
            }
        };

    private:
        HashTable<K, K, SetKeyOfT, Hash<K>> _ht; //第二个参数决定是什么模型

    public:
        typedef typename HashTable<K, K, SetKeyOfT, Hash<K>>::iterator Iterator;
        bool insert(const K &key)
        {
            return _ht.Insert(key);
        }
        Iterator begin()
        {
            return _ht.begin();
        }
        Iterator end()
        {
            return _ht.end();
        }
    };
};