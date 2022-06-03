#pragma once
#include"list.hpp"
namespace s
{
    template <class Iterator>

    class reverse_iterator //可以是任意类型的反向迭代器
    {
        typedef reverse_iterator<Iterator> self;
    protected:
        Iterator current; //使用的是一个迭代器对象,const或者非const
    public:
        reverse_iterator(Iterator it)//用一个正向迭代器去构造就行了
        :current(it)
        {}
        self& operator ++()//++就是--
        {
            --current;
            return *this;
        }

        self& operator --()//--就是++
        {
            ++current;
            return *this;
        }
        bool operator!=(const self& rit) const
        {
            return current!=rit.current;
        }
        template<class T>
        T &operator*()
        {
            //按照理论来说返回的是当前位置的数据
            //正向迭代器的结束就是反向迭代器的开始，正向迭代器的开始就是反向迭代器的结束，对称关系
            //所以就要先--
            Iterator tmp=current;
            return *--tmp;//解引用取前一个位置，主要就是为了让反向迭代器的开始和结束和正向迭代器对称，没有其他意义
        }

    };
}