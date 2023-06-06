#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

class LFUCache
{
private:
    int capacity_;
    int minFrequency_;
    unordered_map<int, pair<int, int>> cache_; // key->{value,frequency},查询的时候直接查询这个cache
    unordered_map<int, list<int>::iterator> keyIter_;//每个key对应的元素在frequency中的list某个位置的迭代器
    unordered_map<int, list<int>> frequency_; //哈希表->key是出现的频次，value就是一个链表里面出现这个频次的元素
public:
    LFUCache(int capacity)
        : capacity_(capacity), minFrequency_(0)
    {
    }
    //访问某个key
    int get(int key)
    {
        //
        if (cache_.find(key) == cache_.end())
        {
            return -1; //没有找到就返回-1
        }
        //找到了
        int value = cache_[key].first;
        int& freq = cache_[key].second;
        frequency_[freq].erase(keyIter_[key]); //从原来的链表上删除这个元素
        //更新频率
        freq++;
        frequency_[freq].push_back(key); //插入到对应频次的链表上
        keyIter_[key] = --frequency_[freq].end();//更新迭代器对应的list迭代器节点
        if (frequency_[minFrequency_].empty())
        {
            minFrequency_++;
        }
        return value; //获得这个元素对应的value
    }
    void put(int key,int value)
    {
        //插入一个元素
        if (capacity_ <= 0)
            return;
        int existingValue = get(key);//根据这个key获得它对应的value
        if (existingValue != -1)
        {
            //这个元素存在，只需要更新他的值就行了
            cache_[key].first =value;
            return;
        }
        //当前的值不存在
        if (cache_.size() >= capacity_)
        {
            int evictkey = frequency_[minFrequency_].front(); //获得最小的元素
            cache_.erase(evictkey);
            keyIter_.erase(evictkey);
            frequency_[minFrequency_].pop_front(); //删除第一个元素
        }
        //添加进新的元素
        cache_[key] = {value, 1};
        frequency_[1].push_back(key);//往出现1次的频率链表中添加元素
        keyIter_[key] = --frequency_[1].end();
        minFrequency_ = 1;//因为是新插入进来的元素，所以就需要将最小频率修改为1
    }
};