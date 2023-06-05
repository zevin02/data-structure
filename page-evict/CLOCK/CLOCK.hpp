#include <iostream>
using namespace std;
#include <vector>

// // hand是进行轮询的访问
// //当需要换出的时候，需要换出去的是hand指针遇到的第一个ref为false的page
// struct page
// {
//     bool is_exist_;
//     //ref用来标记当前页面是否被访问过了，如果当前节点被访问过了，REF属性就会设置成true
//     //当进行页面淘汰的时候，会扫描所有的节点，找到REF属性为false来进行删除
//     //如果当前被访问的REF是true，就需要标记成false，以便下次来扫描的时候能找到最久未被访问的节点
//     bool is_REF;
//     bool is_MODIFIED;

//     int pageid_;
//     page(int pageid)
//         : pageid_(pageid), is_exist_(true), is_REF(true),is_modify_(false)
//     {
//     }
// };

// //对page的删除是把is_exists设置为false,换入新的page的时候，就需要将新的page覆盖掉原来的page
// class CLOCK
// {
// private:
//     vector<page *> pages_;
//     int hand_; // hand指针来进行轮询,轮询pages
//     int size_;
//     int capacity_;

// public:

// //创建CLOCK的时候，让hand指向0索引位置，让pages的vector预留出capacity的大小
//     CLOCK(int capacity)
//     :hand_(0),size_(0),capacity_(capacity)
//     {
//         pages_.reserve(capacity_);
//     }
    
//     //当clock满了的时候，就需要调度页面的算法,当要插入新元素的时候，就插入到这里返回的下标的位置上
//     //
// int clock_dispatch()
// {
//     while (true)
//     {
//         page* cur_page = pages_[hand_]; // 获得当前hand指向的页面
//         if (cur_page->is_exist_)
//         {
//             // 如果当前的页面存在
//             if (cur_page->is_REF)
//             {
//                 // 这里把REF设置成false
//                 cur_page->is_REF = false; // 如果当前被访问的REF是true，就需要标记成false，以便下次来扫描的时候能找到最久未被访问的节点
//             }
//             else if (cur_page->is_MODIFIED)
//             {
//                 // 如果修改位为true，则将其设置为false，并继续下一个页面的扫描
//                 cur_page->is_MODIFIED = false;
//             }
//             else
//             {
//                 // 当前的REF和MODIFIED都是false，需要将当前元素设置为不存在
//                 cur_page->is_exist_ = false;
//                 return hand_;
//             }
//         }
//         hand_ = (hand_ + 1) % size_;
//     }
// }


//     void put(int pageid)
//     {
//         if(size_==capacity_)//如果已经满了
//         {
//             //进行页表值换算法
//             int index=clock_dispatch();
//             pages_[index]=new page(pageid);
//         }
//         //如果没有满
//         //就直接添加
//         pages_[hand_]=new page(pageid);
//         size_++;
//         hand_=(hand_+1)%size_;//将指针移动到下一个位置

//     }
//     //由于实际系统中，页面不仅会被修改，还会被访问
//     //1.最近没有被修改，也没有被访问
//     //2.最经没被访问，但是被修改了
//     //3.最近访问了，但是没有修改
//     //4.最近又访问又修改

//     //需要使用两个标记来依次判断有没有情况1的页面，如果有就换出，如果没有就判断情况2
//     //判断完后还是没有，需要将所有情况都设置为0,再依次判断1和2
    
// };



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
                    //该节点被访问过，设置成未被访问过
                    hand_->is_REF = false;
                } else if (hand_->is_MODIFIED) {
                    //设置成未被修改过
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