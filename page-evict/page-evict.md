操作系统中数据库中，会使用驱逐算法，来实现内存和磁盘之间的交互，在内存空间满的时候，如果需要把磁盘的页面添加到内存中，就需要进行将内存的一个页面换出，保证内存空间不会溢出，我们希望在内存里面的页面能够尽可能多的被访问

# LRU
将缓存中最久没有被使用的页面给换出去，即先进入缓存的数据先离开,LRU中将先进入缓存的数据看作最久未被访问的数据



![](https://i.imgur.com/7iKfmtG.png)

1. 每次插入数据都是头部添加，当缓存满了删除数据的时候都是删除尾部
2. 双端队列中不能出现重复的数据，当插入数据已经存在的时候，就需要先将其该数据删除掉

# 时钟置换算法(CLOCK Algorithm)
时钟置换算法使用一个指针周期性地指向缓冲区中的下一个页面。

当页面被访问时:

* 如果页面在缓冲区中,将其标记为最近使用
* 否则,发生缺页异常,替换指针指向的页面

替换选择规则:

* 选择尚未使用的页面作为替换受害者
* 优先选择未修改过的页面

对新页面:

* 标记为最近使用
* 设置为修改状态

将页指针移动到下一个页面。

设置页面为未使用状态的目的是:

* 持续选出未使用的页面替换
* 使页面进入活跃和不活跃周期

优点:

>算法简单,需要很少的状态信息
通过周期性地访问每个页面,实现了类似时间分片的公平性

缺点:

>无法考虑页面使用频率的不同
整体上,时钟算法充分利用了页面使用序列中的局部性,但不能很好地处理全局性。

这样删除了REF和m标记,直接描述主要流程。页指针hand泛指缓冲区中的下一个页面,默认未使用。

当页面最近使用时便标记为最近使用,其他未使用的页面仍然可以被替换。


由于在实际系统使用中页面不仅会被访问还会被修改，所以改进型Clock算法就是把页面分成四种情况：
1. 最近既没修改也没访问
2. 最近没访问但修改了
3. 最近访问了但没修改
4. 最近即访问又修改了

按照类似简单Clock算法（使用两个标号）来依次判断有没有情况1的页面，如果有就换出，如果没有就判断有没有情况2的页面，注意，当判断完后还是没有并不是去判断情况3和4，而是把所有访问标号都重置为0，再依次判断1和2，这样就能找到最适合换出的页面。




~~~cpp

struct Page {
    bool is_exist_;//该页面是否存在
    bool is_REF;//该页面是否被访问过
    bool is_MODIFIED;//该页面是否被修改过
    int pageid_;

    Page(int pageid)
        : pageid_(pageid), is_exist_(true), is_REF(true), is_MODIFIED(true) {}
};

class CLOCK {
private:
    list<Page> pages_;
    list<Page>::iterator hand_;//页面值换的时候，通过迭代器的方式来进行页面的插入和删除
    int size_;
    int capacity_;

public:
    CLOCK(int capacity)
        : size_(0), capacity_(capacity) {
        pages_.reserve(capacity_);
        hand_ = pages_.begin();//指向头节点的指针
    }

    void clock_dispatch() {
        while (true) {
            if (hand_->is_exist_) {
                if (hand_->is_REF) {
                    hand_->is_REF = false;
                } else if (hand_->is_MODIFIED) {
                    hand_->is_MODIFIED = false;
                } else {
                    //既不存在也没有修改过，这个就是置换的高优先级别
                    hand_->is_exist_ = false;
                    return;
                }
            }
            //节点往后移动
            advance(hand_, 1);
            if (hand_ == pages_.end())//走到尾巴，
                hand_ = pages_.begin();
        }
    }

    void put(int pageid) {
        if (size_ == capacity_) {//满了，就需要进行值换
            clock_dispatch();//进行页面置换
            //hand现在指向的位置就是需要插入置换的节点
            *hand_ = Page(pageid);
        } else {
            pages_.emplace_back(pageid);//页面中插入一个元素
            size_++;//添加一个元素
        }
        advance(hand_, 1);//hand往后走1位
        if (hand_ == pages_.end())//如果走到最后，就需要转到头节点位置上
            hand_ = pages_.begin();
    }
};

~~~


# LFU
Least Frequently Used:最近最少使用算法
一个数据在一段事件很少被访问到，那么可以认为他将来被访问的可能性很小，因此当空间满了，最小频率访问的数据应该最先被淘汰

![](https://mjj.today/i/I6gLXN)