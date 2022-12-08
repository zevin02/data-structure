#include <iostream>
#include <vector>
#include <list>
using namespace std;

/*
map的key只要能比较大小就行了，map的支持比较大小是用仿函数写的，如果要发过来，只要仿函数里面的参数反过来即可
unordered_map的key，要求，1.能支持取模，或者能转化成取模的的整形
2. 要支持key能不能比较大小，用来find，或相等的仿函数,我们写的类型，就要自己写一个仿函数来支持即可，或者可以支持比较大小，但是不是我们想要的，比如类型是指针，就要我们自己写

*/
template <class K, class V>

struct HashNode
{
    pair<K, V> _kv;
    HashNode<K, V> *_next; //我们用一个单链表,我们自己弄的
    HashNode(const pair<K, V> &kv)
        : _kv(kv), _next(nullptr)
    {
    }
};

template <class K>
struct Hash
{
    size_t operator()(const K &key)
    {
        return key; //负数也给他转化成非负数
    }
};

template <class K, class V, class HashFunc = Hash<K>>
class HashTable
{
    typedef HashNode<K, V> Node;

private:
    vector<Node *> _table; //链表里面的元素挂起来,指针数组
    size_t _n = 0;         //有效数据，计算负载因子
public:
    Node *Find(const K &key)
    {
        if (_table.empty())
        {
            return nullptr;
        }
        HashFunc hc;
        size_t index = hc(key) % _table.size();
        if (_table[index] == nullptr)
        {
            //没找到，
            return nullptr;
        }
        else
        {
            //在它对应的链表下面找，看看能不能找的到
            Node *cur = _table[index]; //_table[index]就是对应链表的头节点
            while (cur)
            {
                if (cur->_kv.first == key)
                    return cur;
                cur = cur->_next;
            }
            return nullptr;
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
        size_t index = hc(key) % _table.size();
        Node *cur = _table[index];
        Node *prev = nullptr;
        while (cur)
        {
            if (cur->_kv.first == key)
            {
                if(prev==nullptr)
                {
                    //头删除
                    _table[index]=cur->_next;
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
    bool Insert(const pair<K, V> &kv)
    {

        Node *ret = Find(kv.first);
        if (ret)
        {
            return false;
        }

        HashFunc hc;

        if (_n == _table.size()) //这里我们可以让负载因子变得大一点,负载因子=1的时候扩容`
        {
            size_t newcp = _table.size() == 0 ? 10 : _table.size() * 2;
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
                        size_t index = hc(cur->_kv.first) % newTable.size();
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
        size_t index = hc(kv.first) % _table.size();
        //没有数据
        Node *newnode = new Node(kv); //新的头节点
        newnode->_next = _table[index];
        _table[index] = newnode; //
        //用头插法把数据插进去，因为重新映射代价比较大

        _table[index] = newnode;
        _n++;
        return true;
    }
};

void testhashtable()
{
    vector<int> r = {4, 24, 14, 7, 37, 27, 57, 67, 34, 14, 54};
    HashTable<int, int> ht;
    for (auto e : r)
    {
        ht.Insert(make_pair(e, e));
    }
    if (ht.Find(37))
    {
        cout << "find" << endl;
    }
    if(ht.Erase(4))
    {
        cout<<"erase"<<endl;
    }
     if (ht.Find(4))
    {
        cout << "find" << endl;
    }
}

