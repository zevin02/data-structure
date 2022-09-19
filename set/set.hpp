#pragma once
#include "RBtree.hpp"

namespace xzw
{
    template <class K>
    class set
    {

    public:
        struct SetKeyOfT //仿函数
        {
            const K &operator()(const K &k)
            {
                return k; //返回的是第一个参数
            }
        };
        typedef typename RBTree<K, K, SetKeyOfT>::Iterator Iterator; //取出它的内嵌类型，再进行重命名

        Iterator begin()
        {
            return _t.begin();
        }
        Iterator end()
        {
            return _t.end();
        }
        Iterator Find(const K& key)
        {
            return _t.Find(key);
        }
        pair<Iterator,bool> insert(const K &key)
        {
            return _t.Insert(key);
        }


    private:
        RBTree<K, K, SetKeyOfT> _t; //用第二个模板参数来控制，第一个模板参数是用来知道V里面键的类型
    };

};