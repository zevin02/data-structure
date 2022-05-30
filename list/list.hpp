#pragma once
#include<iostream>
using namespace std;
namespace xzw
{
    template <class T>
    struct ListNode //节点
    {
        ListNode<T> *_prev;
        ListNode<T> *_next;
        T _data;
        ListNode(const T &data)
            : _data(data), _next(nullptr), _prev(nullptr)
        {
        }
        ListNode() //弄了一个函数重载，没传参数的

            : _next(nullptr), _prev(nullptr)
        {
        }
    };

    template <class T>
    struct _list_iterator
    {
        typedef ListNode<T> Node; //把这个对象重命名一下,这样之后就不用再显示使用T
        typedef _list_iterator<T> _iterator;
        Node *_node; //节点的一个迭代器,因为本质上还是指针

        _list_iterator(Node *x) //用一个节点的指针来构造一个迭代器
            : _node(x)
        {}
        _iterator &operator++()
        {
            //到下一个位置
            _node = _node->_next;
            return *this;
        }
        T &operator*()
        {
            return _node->_data;
        }
        bool operator==(_iterator& t)
        {
            return _node==t._node; 
        }
        bool operator!=(_iterator & t)
        {
            return _node!=t._node;//指向节点的指针不一样就不相等
        }

    };

    template <class T>
    class List
    {
        public:
        typedef ListNode<T> Node; //把这个对象重命名一下,这样之后就不用再显示使用T

    private:
        Node *_head; //头节点

    public:
        typedef _list_iterator<T> iterator;
        List() //构造函数，里面对头节点进行初始化
        {
            _head = new Node;
            _head->_next = _head; //因为是双向链表
            _head->_prev = _head;
        }
        void PushBack(const T &x)
        {
            Node *t = new Node(x); //开空间+初始化
            //这个就算链表是空也是可以使用的
            Node *last = _head->_prev;
            last->_next = t;
            t->_prev = last;
            t->_next = _head;
            _head->_prev = t;
        }
        iterator begin()
        {
            return iterator(_head->_next);//调用了一个构造函数，使用了一个匿名对象
        }
        iterator end()
        {
            return iterator(_head);//end是最后一个数据的下一个位置//使用了一个匿名对象
        }


        //弄出迭代器出来,迭代器还是一个节点的指针，只不过这个迭代器用一个自定义类型进行封装
        //原本在顺序表里面迭代器的++，可以到达下一个位置，但是list中的是随即迭代器，++it 不能到达下一个位置，所以我们需要对
        //他就行运算符重载，* 解引用
    };

    void test_list1()
    {
        List<int> lt;
        lt.PushBack(1);
        lt.PushBack(2);
        lt.PushBack(3);
        lt.PushBack(4);
        List<int>::iterator it=lt.begin();
        cout<<*it<<endl;
        ++it;
        cout<<*it<<endl;
        // while(it!=lt.end())
        // {
        //     cout<<(*it)<<" ";
        //     ++it;
        // }
        cout<<endl;
    }
}