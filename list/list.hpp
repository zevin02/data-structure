#pragma once
#include <iostream>
using namespace std;
namespace xzw
{
    template <class T>
    struct ListNode //节点，
    {
        ListNode<T> *_prev;
        ListNode<T> *_next;
        T _data;
        ListNode(const T &data)//构造函数，对节点就行初始化，有参数
            : _data(data), _next(nullptr), _prev(nullptr)//其实他的头和尾指针也不用初始化，

        {
        }
        ListNode() //弄了一个函数重载，没传参数的

            : _next(nullptr), _prev(nullptr)
        {
        }
    };

    template <class T,class Ref>//我们这里多增加一个模板参数,处理const迭代器
    struct _list_iterator //迭代器这个类
    {
        typedef ListNode<T> Node; //把这个对象重命名一下,这样之后就不用再显示使用T
        typedef _list_iterator<T,Ref> _iterator;
        Node *_node;            //节点的一个迭代器,因为本质上还是指针，这个迭代器他本来就是指向一个节点，所以他必须要有一个节点的指针
        _list_iterator(Node *x) //用一个节点的指针来构造一个迭代器
            : _node(x)
        {}
        _iterator &operator++()//++it;
        {
            //到下一个位置
            _node = _node->_next;
            return *this;
        }
        _iterator &operator--()
        {
            //到下一个位置
            _node = _node->_prev;
            return *this;
        }
        Ref operator*() //传引用返回，之后这个值还能被修改.这里使用一个模板参数
        {
            return _node->_data;
        }
         
        bool operator==(const _iterator &t)//这个对象不能被改变
        {
            return _node == t._node;
        }
        bool operator!=(const _iterator &t) //必须要使用const对象才可以，不然就会报错
        {
            return _node != t._node; //指向节点的指针不一样就不相等
        }

        //迭代器的拷贝构造和析构，都不需要我们自己实现，
        //it2(it),这里我们就是希望他们是指向同一个节点，这样才能对这个节点就行操作，深拷贝出来的，根本就不是我们需要的东西
        //我们要的就是浅拷贝，要的就是浅拷贝，析构也不需要我们实现，这个节点迭代器不能对他处理，这个节点不是迭代器的，只是为了访问和修改容器的

    };

    //如果要实现const迭代器，我们不要写冗余的代码

    template <class T>
    class List
    {
    public:
        typedef ListNode<T> Node; //把这个对象重命名一下,这样之后就不用再显示使用T

    private:
        Node *_head; //头节点,哨兵位

    public:
        typedef _list_iterator<T,T&> iterator; //把迭代器也重命名一下,普通的迭代器
        typedef _list_iterator<T,const T&> const_iterator; //const迭代器，第二个参数使用const对象，

        List()                              //构造函数，里面对头节点进行初始化
        {
            _head = new Node;     //无参构造
            _head->_next = _head; //因为是双向链表
            _head->_prev = _head;
        }
        void PushBack(const T &x) //处理尾插
        {
            Node *t = new Node(x); //开空间+初始化，有参数的构造
            //这个就算链表是空也是可以使用的
            Node *last = _head->_prev;
            last->_next = t;
            t->_prev = last;
            t->_next = _head;
            _head->_prev = t;
        }
        iterator begin() //迭代器的头
        {
            return iterator(_head->_next); //调用了一个构造函数，使用了一个匿名对象，头节点的下一个就是第一个位置
            //返回一个迭代器的类
        }
        iterator end() //迭代器的尾，iterator是一个类型
        {
            return iterator(_head); // end是最后一个数据的下一个位置//使用了一个匿名对象
        }
        //上面是普通迭代器，下面是const迭代器
        
        const_iterator begin() const //迭代器的头
        {
            return const_iterator(_head->_next); //调用了一个构造函数，使用了一个匿名对象，头节点的下一个就是第一个位置
            //返回一个迭代器的类
        }
        const_iterator end() const//迭代器的尾，iterator是一个类型
        {
            return const_iterator(_head); // end是最后一个数据的下一个位置//使用了一个匿名对象
        }
        

        //弄出迭代器出来,迭代器还是一个节点的指针，只不过这个迭代器用一个自定义类型进行封装
        //原本在顺序表里面迭代器的++，可以到达下一个位置，但是list中的是随即迭代器，++it 不能到达下一个位置，所以我们需要对
        //他就行运算符重载，* 解引用


        void f()
        {
            //Node* 原生指针和一个迭代器对象，他们占用空间是一样的，存的东西也一样，但是对他们使用运算符的意义和结果都是不一样的
            Node* pnode=_head->_next;
            iterator it=_head->_next;
            pnode++;
            *pnode;
            it++;
            *it;
            //这就是类型的意义
            
        }
    
    
    };

    void print(const List<int>& lt)//const对象必须使用const迭代器,这里我们实现的就是使用const迭代器进行打印
    {
         List<int>::const_iterator it=lt.begin();//这里调用的就是const迭代器，实现了重载
         while(it!=lt.end())
         {
            //  *it+=2;//这个调用的是const迭代器
             cout<<*it<<" ";
             ++it;
         }
    }

    void test_list1()
    {
        List<int> lt;
        lt.PushBack(1);
        lt.PushBack(2);
        lt.PushBack(3);
        lt.PushBack(4);
        List<int>::iterator it = lt.begin();//因为迭代器就是每一个元素，begin（）本身就相当于一个节点
        print(lt);
        // while (it != lt.end())
        // {
        //     cout << (*it) << " "; //*解引用，这样就可读可写
        //     ++it;
        // }
        --it;
        cout<<*it<<endl;
        cout << endl;
    }
}