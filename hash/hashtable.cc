
#include <iostream>
using namespace std;
#include <vector>

enum Status //状态标记
{
    EXISTS,
    EMPTY,
    DELETE
};

template <class K, class V>

struct HashData
{
    pair<K, V> _kv;
    Status _status=EMPTY;
};

//这个hash表即可给map也可给set
//找一个值，遇到空才停，遇到删除还要继续找
//出现的问题
// 1.删除一个值，这个值应该设置成多少
// 2.删除完毕之后，后面冲突的值怎么办,现在，把一个值删除之后，就要把他的状态标记成删除，不清理数据

template <class K, class V>

class HashTable
{
private:
    vector<HashData<K, V>> _table; //我们数据不是都是按顺序的存储
    size_t _n = 0;                 //有效数据的个数
public:
    /*
    散列表的载荷因子a=表中元素的个数/散列表的长度,a越大，冲突的概率就越大，a越小，冲突的概率就越小，如果超过了一定值，就要扩容


    */
    HashData<K, V> *Find(const K &key)
    {
        if(_table.size()==0)
        {
            return nullptr;//没找到
        }
        size_t start = key % _table.size(); //模上它的vector的元素大小,如果模上它的容量的话，可能回超出size的范围，无法访问_table[i],只能访问size以内的
        size_t i = 0;
        size_t index = start;

        while (_table[index]._status != EMPTY)//在这里死循环了
        {
            //数据存在，就继续往下探测
            if (_table[index]._kv.first == key&&_table[index]._status==EXISTS)
            {
                return &_table[index];
            }
            i++;
            index = start + i * i; //这个是依次探测
            //如果到最后还有数据的话，那就要绕回来
            index %= _table.size();
        }

        return nullptr;
    }

    bool
    Erase(const K &key)
    {
        HashData<K,V> *ret = Find(key);
        if (ret == nullptr)
        {
            //找不到要删除的值
            return false;
        }
        else
        {
            //找到了那个值
            ret->_status = DELETE;
            return true;
        }
    }

    bool Insert(const pair<K, V> kv)
    {
        
        HashData<K,V> *ret = Find(kv.first);
        if (ret != nullptr)
        {
            return false; //有一个重复的值
        }

        if (_table.size() == 0 || _n * 10 / _table.size() >= 7)
        {
            //越小，冲突的概率就越低，效率越高，但是空间浪费越高
            //繁殖，冲突的概率越高，效率越低，浪费越小
            //载荷因子大于0.7就要进行扩容
            size_t newsize = _table.size() == 0 ? 10 : _table.size() * 2;
            // vector<HashData<K,V>>  newtable;

            // newtable.resize(newsize);//扩完容，就会把没有元素的地方初始化成0
            // //扩容完，要重新改变值的位置，原来冲突的可能不冲突了，不冲突的可能就冲突了
            // //遍历原表，把原表的数据重新映射到新表
            // for(size_t i=0;i<newtable.size();i++)
            // {
            //     //
            // }
            HashTable<K, V> newHT;
            newHT._table.resize(newsize);
            for (size_t i = 0; i < _table.size(); i++)
            {
                if (_table[i]._status == EXISTS)
                {
                    //数据存在,就往我们新的ht里面插入
                    // newHT._table.insert(_table[i]._kv);
                    newHT.Insert(_table[i]._kv);
                }
            }
            _table.swap(newHT._table); //把两个进行交换即可
        }
        size_t start = kv.first % _table.size(); //模上它的vector的元素大小,如果模上它的容量的话，可能回超出size的范围，无法访问_table[i],只能访问size以内的
        size_t i = 0;
        size_t index = start; //探测旁边
                              //现在我们可以用探测i平方
                              /*
                                  vector<int> v;
                                  v.reserve(10);
                                  for(int i=0;i<10;i++)
                                  {cin>>v[i]}//这样是不行的，因为[]必须要保证这个地址上是有数据的
                              */
        //如果这个位置没数据，就直接放进去，如果有数据，就要开始线性探测
        //空或者删除都可以放

        //线性探测的大逻辑,很容易拥堵在一起，尤其是相连的一块，你占我，我占你
        //二次探测

        while (_table[index]._status == EXISTS)
        {
            //数据存在，就继续往下探测
            i++;
            index = start + i * i; //这个是依次探测
            //如果到最后还有数据的话，那就要绕回来
            index %= _table.size();
        }
        //一定是在一个可以放数据的地方
        _table[index]._kv = kv;
        _table[index]._status = EXISTS; //插入设置存在
        _n++;

        //找空位置的代价会很大
        return true;
    }
};

void test()
{
    HashTable<int, int> ht;
    int a[8] = {2, 12, 22, 32, 42, 52, 62, 72};
    for (auto e : a)
    {
        ht.Insert(make_pair(e, e));
    }
    ht.Insert(make_pair(72, 72));
    ht.Find(2);
    ht.Erase(22);
}

int main()
{
    test();
    return 0;
}