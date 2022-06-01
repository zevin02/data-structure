#pragma once
#include <iostream>
#include <cassert>
using namespace std;

// vector和list相比还是vector的使用次数更多，因为空间不够扩容，这样之后的开辟就不需要再去扩容了（空间的影响不大），时间效率影响最大
//如果频繁使用头插或者中间插入的话，就要使用list
// STL的所有容器都不保证线程安全

namespace xzw
{
    template <class T>
    struct ListNode //节点，
    {
        ListNode<T> *_prev;
        ListNode<T> *_next;
        T _data;
        ListNode(const T &data)                           //构造函数，对节点就行初始化，有参数
            : _data(data), _next(nullptr), _prev(nullptr) //其实他的头和尾指针也不用初始化，

        {
        }
        ListNode() //弄了一个函数重载，没传参数的

            : _next(nullptr), _prev(nullptr)
        {
        }
    };

    template <class T, class Ref, class Ptr> //我们这里多增加一个模板参数,处理const迭代器,第二个参数是处理*,第三个参数是处理->
                                             //迭代器模拟的是指针的行为，像指针一样去访问容器

    // string vector 原生指针（物理空间是连续的）
    //迭代器不一定是一个原生指针,也可能是一个类，去封装节点指针，通过运算符重载，实现.让他像指针一样，
    struct _list_iterator //迭代器这个类,直接就给你访问的，一般都是给你struct
    {
        typedef ListNode<T> Node; //把这个对象重命名一下,这样之后就不用再显示使用T
        typedef _list_iterator<T, Ref, Ptr> _iterator;
        Node *_node;            //节点的一个迭代器,因为本质上还是指针，这个迭代器他本来就是指向一个节点，所以他必须要有一个节点的指针
        _list_iterator(Node *x) //用一个节点的指针来构造一个迭代器
            : _node(x)
        {
        }
        _iterator &operator++() //++it;返回的是原来的类型
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

        _iterator operator++(int) // it++;返回的是原来的类型
        {
            //到下一个位置
            _iterator tmp(*this); //拷贝构造一个他这个位置的
            _node = _node->_next;
            return tmp; //返回原来的地址
        }

        _iterator operator--(int) // it--;
        {
            //到下一个位置
            _iterator tmp(*this);
            _node = _node->_prev;
            return tmp;
        }
        Ref operator*() //传引用返回，之后这个值还能被修改.这里使用一个模板参数
        {
            return _node->_data;
        }

        bool operator==(const _iterator &t) //这个对象不能被改变
        {
            return _node == t._node;
        }

        bool operator!=(const _iterator &t) //必须要使用const对象才可以，不然就会报错
        {
            return _node != t._node; //指向节点的指针不一样就不相等
        }

        //在这里重载一下指针的解引用
        T *operator->() //这里是普通迭代器，返回的是T*，
        {
            return &_node->_data; //前面加了一个&，
        }

        //同样，我们还要处理const迭代器的->

        //迭代器的拷贝构造和析构，都不需要我们自己实现，
        // it2(it),这里我们就是希望他们是指向同一个节点，这样才能对这个节点就行操作，深拷贝出来的，根本就不是我们需要的东西
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
        typedef _list_iterator<T, T &, T *> iterator;                   //把迭代器也重命名一下,普通的迭代器,因为*返回的就是这个类型所以用&引用，第三个是* 是指针，所以使用的是T*
        typedef _list_iterator<T, const T &, const T *> const_iterator; // const迭代器，第二个参数使用const对象，

        List() //构造函数，里面对头节点进行初始化，哨兵位的头节点，默认的构造函数
        {
            _head = new Node;     //无参构造
            _head->_next = _head; //因为是双向链表
            _head->_prev = _head;
        }
        // List l(t)

        // List(const List<T> &t) //要实现深拷贝
        // {
        //     _head = new Node;     //无参构造
        //     _head->_next = _head; //因为是双向链表
        //     _head->_prev = _head;

        //     iterator it = begin();
        //     //要有一个哨兵位的头节点
        //     while (it != end())
        //     {
        //         PushBack(it._node->_data);
        //         it++;
        //     }
        // }

        // //现代版本的拷贝构造
       

        // //赋值的传统写法l=lt
        // List<T> &operator=(const List<T> &lt)
        // {
        //     if (this != &lt)
        //     {
        //         //原来的节点都要清理掉
        //         clear();
        //         for (auto e : lt) // e就是这个容器里面的有效数据
        //         {
        //             PushBack(e);
        //         }
        //     }
        //     return *this;
        // }

        template <class InputIterator>
        //任意类型的迭代器
        List(InputIterator first, InputIterator last)//这里又添加了一个构造函数,使用迭代器来进行构造 
        {
            _head = new Node;     //无参构造
            _head->_next = _head; //因为是双向链表
            _head->_prev = _head;
            while (first != last)
            {
                PushBack(*first); //把这段迭代器区间里面的值插入到容器里面
                ++first;
            }
        }
        //L1(l2)
        List(const List<T>& lt)
        {
            
            _head = new Node;     //无参构造
            _head->_next = _head; //因为是双向链表
            _head->_prev = _head;
            List<T> tmp(lt.begin(),lt.end());
            std::swap(_head,tmp._head);//我们交换了两个节点的指针,这样this指向的就是tmp里面的东西,因为是通过指针进行访问的

        }

        List<T> & operator=(const List<T> lt)
        {
            std::swap(_head,lt._head);
            return *this;
        }
        void clear()
        {
            //把所有数据都删除掉
            iterator it = begin();
            while (it != end())
            {
                Erase(it++);
            }
            //这里删除完之后,节点的链接关系还要改变一下,但是这里我们是使用erase所以连接关系都没有任何的改变,都是正常的
            _head->_next = _head;
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
        const_iterator end() const //迭代器的尾，iterator是一个类型
        {
            return const_iterator(_head); // end是最后一个数据的下一个位置//使用了一个匿名对象
        }

        //弄出迭代器出来,迭代器还是一个节点的指针，只不过这个迭代器用一个自定义类型进行封装
        //原本在顺序表里面迭代器的++，可以到达下一个位置，但是list中的是随即迭代器，++it 不能到达下一个位置，所以我们需要对
        //他就行运算符重载，* 解引用

        void f()
        {
            // Node* 原生指针和一个迭代器对象，他们占用空间是一样的，存的东西也一样，但是对他们使用运算符的意义和结果都是不一样的
            Node *pnode = _head->_next;
            iterator it = _head->_next;
            pnode++;
            *pnode;
            it++;
            *it;
            //这就是类型的意义
        }

        //这里insert以后pos不会失效,因为他指向的节点不会变,vector会失效的是因为他扩容,他指向的东西就变了,
        iterator Insert(iterator pos, const T &x)
        {
            //这里的迭代器是一个对象，不是指针，所以使用 .
            //在pos的前面插入一个数据
            Node *cur = pos._node; //我们这里有了迭代器，就能够获得他指向的对象，因为是使用struct进行封装的

            Node *prev = cur->_prev;
            Node *newnode = new Node(x);
            //在cur的前面插入一个节点
            newnode->_next = cur;
            cur->_prev = newnode;
            prev->_next = newnode;
            newnode->_prev = prev;
            //这样就连接起来了
            return iterator(newnode); //返回的是newnode的迭代器,
        }

        void push_front(const T &x)
        {
            Insert(begin(), x);
        }
        //这个地方pos就失效了
        iterator Erase(iterator pos)
        {
            assert(pos != end()); //不能把头给删除掉
            Node *t = pos._node;
            Node *prev = t->_prev;
            Node *next = t->_next;
            prev->_next = next;
            next->_prev = prev;
            delete t;              //删除一个节点
            return iterator(next); //指向刚刚那个位置的迭代器
        }
        void pop_back()
        {
            Erase(--end()); // end()是一个迭代器,再--,就到了最后一个元素了
        }
        void pop_front()
        {
            Erase(begin());
        }
        ~List()
        {
            //析构
            clear();
            delete _head; //要把头节点给干掉
            _head = nullptr;
        }
    };

    struct Date //这里是一个类型，放到链表里面作为节点数据
    {
        int _day;
        int _month;
        int _year;
        Date(int year = 2022, int month = 2, int day = 2)
            : _year(year), _month(month), _day(day)
        {
        }
    };
    void print(const List<int> &lt) // const对象必须使用const迭代器,这里我们实现的就是使用const迭代器进行打印
    {
        List<int>::const_iterator it = lt.begin(); //这里调用的就是const迭代器，实现了重载
        while (it != lt.end())
        {
            //  *it+=2;//这个调用的是const迭代器
            cout << *it << " ";
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
        List<int> l(lt); //这个肯定不能用浅拷贝的,会指向同一个地址肯定是不行的
        lt.PushBack(4);
        lt.push_front(5);
        lt.Erase(++lt.begin());
        lt.Insert(lt.begin(), 9);
        lt.pop_front();
        lt.clear();
        List<int> k = lt;
        List<int>::iterator it = lt.begin(); //因为迭代器就是每一个元素，begin（）本身就相当于一个节点
        it++;
        print(lt);
        // while (it != lt.end())
        // {
        //     cout << (*it) << " "; //*解引用，这样就可读可写
        //     ++it;
        // }
    }
    void test_list2()
    {
        List<Date> lt;                  //因为有个哨兵位的头节点
        lt.PushBack(Date(2022, 11, 9)); //这里我们直接给一个匿名对象
        lt.PushBack(Date(2022, 3, 9));  //这里我们直接给一个匿名对象
        lt.PushBack(Date(2022, 4, 9));  //这里我们直接给一个匿名对象
        lt.pop_back();

        List<Date>::iterator it = lt.begin();
        while (it != lt.end())
        {
            // cout<<(*it)._year<<"/"<<(*it)._month<<"/"<<(*it)._day<<endl;
            //这里返回的是Date,所以可以使用.
            cout << it->_year << "/" << it->_month << "/" << it->_day << endl; //但是it因为就是一个指针，所以还可以使用运算符
            cout << endl;
            //这里是去调用it.operator->(),返回的是Date* (T*)是一个指针
            //这里本来应该是it->->_year,为了解决代码的可读性，所以编译器优化了，省略了一个->，所以所有的类型想要重载->，都会被优惠，省略一个->
            ++it;
        }
    }
}