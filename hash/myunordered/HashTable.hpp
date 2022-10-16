#pragma once
#include <iostream>
#include <vector>
#include <list>
using namespace std;

/*
map的key只要能比较大小就行了，map的支持比较大小是用仿函数写的，如果要发过来，只要仿函数里面的参数反过来即可
unordered_map的key，要求，1.能支持取模，或者能转化成取模的的整形
2. 要支持key能不能比较大小，用来find，或相等的仿函数,我们写的类型，就要自己写一个仿函数来支持即可，或者可以支持比较大小，但是不是我们想要的，比如类型是指针，就要我们自己写



*/

template <class K>
struct Hash //把哈希函数放到前面来
{
    size_t operator()(const K &key)
    {
        return key; //负数也给他转化成非负数
    }
};
template <>
struct Hash<string> //特化，全特化
{
    size_t operator()(const string &key)
    {
        size_t ret = 0;
        for (auto e : key)
        {
            ret += e;
        }
        return ret;
    }
};

//这个地方我们也要支持一个迭代器,我们要支持一个桶一个桶进行查找就行了
template <class K, class T, class KeyOfT, class HashFunc>
class HashTable;   //解决互相引用的问题
template <class T> //因为我们也要给map和set使用

struct HashNode
{
    T _data;
    HashNode<T> *_next; //我们用一个单链表,我们自己弄的
    HashNode(const T &data)
        : _data(data), _next(nullptr)
    {
    }
};

template <class K, class T, class Ref, class Ptr, class KeyofT, class HashFunc>
struct HTIterator
{
    typedef HashNode<T> Node; //如果我把一个桶走完了怎么办
    typedef HTIterator<K, T, Ref, Ptr, KeyofT, HashFunc> Self;

    Node *_node;
    HashTable<K, T, KeyofT, HashFunc> *_pht; //指向hash表对象的指针

    HTIterator(Node *node, HashTable<K, T, KeyofT, HashFunc> *pht)
        : _node(node), _pht(pht)
    {
    }

    Ref operator*() //返回引用
    {
        return _node->_data;
    }
    Ptr operator->() //返回地址
    {
        return &_node->_data;
    }
    Self &operator++()
    {
        vector<Node *> table = _pht->GetVectorNode();//这里我们写一个函数获得它的table
        if (_node->_next)
        {
            _node = _node->_next; //这个桶的下一个节点存在，往后走就行了
        }
        else
        {
            bool flag = false;
            //这个节点的下一个不存在，要走到下一个桶
            KeyofT kot;
            HashFunc hf;
            size_t index = hf(kot(_node->_data)) % table.size(); //自己不为空，所以，可以计算它再哪个桶里面,这样就能知道它在几号桶里面
            ++index;
            //从下一个桶，下一个不为空的桶
            while (index <table.size())
            {
                if (table[index])
                {
                    flag = true;
                    break;
                }
                else
                {
                    //没找到就++
                    index++;
                }
            }

            if (flag)
            {
                //找到了
                _node = table[index];
            }
            else
            {
                //表走完了，都没有
                _node = nullptr;
            }
        }
        return *this;
    }

    bool operator!=(const Self &s) const
    {
        return _node != s._node;
    }
};

template <class K, class T, class KeyOfT, class HashFunc>
class HashTable
{
public:
    typedef HashNode<T> Node;
    typedef HTIterator<K, T, T &, T *, KeyOfT, HashFunc> iterator;
    // template <class K, class T, class Ref, class Ptr, class KeyOfT, class HashFunc>
    // friend struct HTIterator; //因为在迭代器里面要使用hashtable里面的东西，所以这里我们把它定义成友元对象即可

private:
    vector<Node *> _table; //链表里面的元素挂起来,指针数组
    size_t _n = 0;         //有效数据，计算负载因子
public:
    HashTable()=default;//构造函数,编译器显示生成默认构造函数
    HashTable(const HashTable<K,T,KeyOfT,HashFunc>& ht)//拷贝构造
    {
        //指针所以要完成一个深拷贝
        _table.resize(ht._table.size());//直接搞到和他一样大
        for(size_t i=0;i<ht._table.size();i++)
        {
            Node* cur=ht._table[i];
            while(cur)
            {
                //没有很强的关联性
                Node* copy=new Node(cur->_data);
                copy->_next=_table[i];
                _table[i]=copy;
                //进行头插
                cur=cur->_next;
            }
        }
    }
    HashTable<K,T,KeyOfT,HashFunc>& operator=(HashTable<K,T,KeyOfT,HashFunc> ht)
    //深拷贝都用现代写法
    {
        //ht1=ht2,进行赋值，这里的ht2是一个拷贝，所以不会关联到原来的东西
        ::swap(_n,ht._n);
        _table.swap(ht._table);//使用现代写法，进行交换
        return *this;
    }
    ~HashTable()
    {
        for(size_t i=0;i<_table.size();i++)
        {
            Node* cur=_table[i];
            while(cur)
            {
                Node* next=cur->_next;
                delete cur;
                cur=next;

            }
            _table[i]=nullptr;//把桶释放掉
        }
    }
    iterator begin()
    {
        for (size_t i = 0; i < _table.size(); i++)
        {
            if (_table[i])
            {
                return iterator(_table[i], this); //这样传进去就是哈希表的指针
            }
        }
        return end();
    }
    iterator end()
    {
        return iterator(nullptr, this); //给控就行了
    }
    iterator Find(const K &key)
    {
        if (_table.empty())
        {
            return iterator(nullptr,this);
        }
        KeyOfT kot;
        HashFunc hc;
        size_t index = hc(key) % _table.size();
        if (_table[index] == nullptr)
        {
            //没找到，
            return iterator(nullptr,this);
        }
        else
        {
            //在它对应的链表下面找，看看能不能找的到
            Node *cur = _table[index]; //_table[index]就是对应链表的头节点
            while (cur)
            {
                if (kot(cur->_data) == key)
                    return iterator(cur,this);
                cur = cur->_next;
            }
            return iterator(nullptr,this);
        }
    }
    bool Erase(const K &key)
    {
        if (_table.empty())
        {
            return false;
        }
        //找到了，就把它给删除掉
        HashFunc hc;
        KeyOfT kot;
        size_t index = hc(key) % _table.size();
        Node *cur = _table[index];
        Node *prev = nullptr;
        while (cur)
        {
            if (kot(cur->_data) == key)
            {
                if (prev == nullptr)
                {
                    //头删除
                    _table[index] = cur->_next;
                }
                else
                    prev->_next = cur->_next;
                delete cur;
                --_n;
                return true;
            }
            else
            {
                prev = cur;
                cur = cur->_next;
            }
        }
        return false;
    }
    vector<Node *> GetVectorNode()
    {
        return _table;
    }
    pair<iterator,bool> Insert(const T &data)
    {

        KeyOfT kot;
        iterator ret = Find(kot(data));
        if (ret!=end())
        {
            return make_pair(ret,false);
        }

        HashFunc hc;

        if (_n == _table.size()) //这里我们可以让负载因子变得大一点,负载因子=1的时候扩容`
        {
            size_t newcp = _table.size() == 0 ? 10 : _table.size() * 2;
            //我们会发现，当表的容量是一个素数的时候，它就会尽可能的冲突小

            //这里就自己来挪动，使用和闭散列一样的方法，不好，因为会一直要new节点，
            vector<Node *> newTable;
            newTable.resize(newcp); //扩容后要进行重新映射
            for (size_t i = 0; i < _table.size(); i++)
            {
                if (_table[i])
                {
                    //有数据
                    Node *cur = _table[i];
                    while (cur)
                    {
                        //把这个数据弄下来，直接
                        Node *next = cur->_next;
                        size_t index = hc(kot(cur->_data)) % newTable.size();
                        //然后进行头插
                        cur->_next = newTable[index];
                        newTable[index] = cur;
                        cur = next;
                    }
                }
                _table[i] = nullptr; //弄完了就把对应的数据给清空即可
            }
            // newcp里面的数据都是我们想要的
            _table.swap(newTable);
        }
        size_t index = hc(kot(data)) % _table.size();
        //没有数据
        Node *newnode = new Node(data); //新的头节点
        newnode->_next = _table[index];
        _table[index] = newnode; //
        //用头插法把数据插进去，因为重新映射代价比较大

        _n++;
        return make_pair(iterator(newnode,this),true);
    }
};
