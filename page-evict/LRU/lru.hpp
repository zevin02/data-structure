#include <iostream>
using namespace std;
#include <unordered_map>
// LRU的节点
struct DListNode
{
    int key_;
    int value_;
    DListNode *prev_;
    DListNode *next_;
    DListNode()
        : key_(0), value_(0), prev_(nullptr), next_(nullptr) {}
    DListNode(int key, int value)
        : key_(key), value_(value), prev_(nullptr), next_(nullptr)
    {
    }
};

//我们需要一个哈希表（判断当前是否有该节点）
//需要链表来存储数据
// lru容量
class LRU
{
public:
    LRU(int capacity)
        : capcity_(capacity), size_(0)
    {
        head_=new DListNode();
        tail_=new DListNode();
        
        head_->next_=tail_;
        tail_->prev_=head_;
    }
    //访问某一个元素
    int get(int key)
    {
        if(cache_.find(key)==cache_.end())
        {
            //没找到,直接返回
            return -1;
        }
        //找到了，就拿到哈希表中对应的数据，就先删除后插入，把该节点放入到头部
        DListNode* node=cache_[key];
        move_to_head(node);
        return node->value_;
    }

    //插入某个元素
    //先查看当前插入的这个节点是否存在，如果存在，就需要拿到node，并且更新value，并且放到头部
    //如果不存在，就插入到头部，如果size等于capacity，就需要删除尾部节点，再插入新的节点
    void put(int key,int value)
    {
        //先查看当前节点是否存在
        if(cache_.find(key)!=cache_.end())
        {
            //当前节点存在
            //获得该节点，并且更新他的数据，在插入到头部
            DListNode* node=cache_[key];
            node->value_=value;//修改他的数据
            move_to_head(node);//移动到头部
        }
        else{
            //这里就是没有找到该元素
            DListNode* node=new DListNode(key,value);
            cache_[key]=node;
            add_to_head(node);//插入到头部
            size_++;
            //超出了容量大小，就需要执行删除操作
            if(size_>capcity_)
            {
                DListNode* target=remove_tail();
                cache_.erase(target->key_);//删除该元素在cache中
                delete(target);
                size_--;
            }
        }
    }

private:
    unordered_map<int, DListNode *> cache_; //；用来判断某个节点是否存在
    //我们每次就处理两个节点，一个头部插入，和尾巴删除
    DListNode *head_;                       //存储链表的头节点，哨兵位
    DListNode *tail_;                       //存储链表的尾节点的下一个节点,tail->prev代表的就是链表的尾节点
    int size_;                              //当前LRU中的元素数量
    int capcity_;                           //能够存储的最大容量

    void remove_node(DListNode* node)//移除某个元素
    {
        //删除节点
        node->prev_->next_=node->next_;
        node->next_->prev_=node->prev_;
    }
    void add_to_head(DListNode* node)//把该数据放到头部
    {

        node->next_=head_->next_;
        head_->next_->prev_=node;
        head_->next_=node;
        node->prev_=head_;

    }
    //把某一个刚访问过的节点，放到头部
    void move_to_head(DListNode* node)
    {
        remove_node(node);//删除该节点
        add_to_head(node);//添加到头部
    }
    //删除尾部元素
    DListNode* remove_tail()
    {
        DListNode* target=tail_->prev_;//最后一个有效节点
        remove_node(target);//移除该节点
        return target;
    }
};