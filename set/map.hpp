#pragma once
#include "RBtree.hpp"

namespace xzw
{
    template <class K, class V>
    class map
    {

    public:
        struct MapKeyOfT //仿函数,泛型编程，复用同一棵树
        {
            const K &operator()(const pair<K, V> &kv)
            {
                return kv.first; //返回的是第一个参数
            }
        };

        typedef typename RBTree<K, pair<K, V>, MapKeyOfT>::Iterator Iterator;//对于一个类型的使用，我们就要声明，不然编译器以为是一个变量

        Iterator begin()
        {
            return _t.begin();
        }
        Iterator end()
        {
            return _t.end();
        }

        pair<Iterator,bool> insert(const pair<K, V> &kv)
        {
            return _t.Insert(kv);
        }


        V& operator[](const K&key)
        {   
            auto ret=_t.Insert(make_pair(key,V()));
            return ret.first->second;
        }
    private:
        RBTree<K, pair<K, V>, MapKeyOfT> _t; //用第二个模板参数来控制
    };

};
