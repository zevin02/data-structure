#include<iostream>
using namespace std;
#include<time.h>
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<set>
/*
    功能基本一样

    1. unorder_xxx遍历不按照keypaixv ,命名体现了
    2. unorder_xxx只有单向迭代器，没有提供反向迭代器
    3. unorder_xxx综合效率略胜map和set


*/


void test_set()
{

//set是排序+去重
//unordered_set：只能实现去重,没有实现排序


    unordered_set<int> us;
    us.insert(1);
    us.insert(3);
    us.insert(4);
    us.insert(1);
    us.insert(7);
    us.insert(6);
    us.insert(2);
    auto it=us.begin();
    while(it!=us.end())
    {
        cout<<*it<<" ";
        it++;
    }
    cout<<endl;
    //这个就不用去重,

    unordered_multiset<int> s;
    s.insert(12);
    s.insert(12);
    s.insert(12);
    auto dt=us.begin();

     while(dt!=s.end())
    {
        cout<<*dt<<" ";
        dt++;
    }

}


void test_op()
{
    //对比一下set和unordered_set的性能
    //unordered_map的性能比较强，强于map
    //set对红黑树，如果是顺序插入的话，效率就不会很高，随机数据插入的话就不会特别的有序
    //如果用unorder_map,

    int n=1000;
    vector<int> v;
    v.reserve(n);
    srand(time(0));
    size_t begin=clock();
    for(int i=0;i<n;i++)
    {
        v.push_back(rand());
    }
    size_t end1=clock();

    set<int> s;
    unordered_set<int> m;//这个随机插入的效率非常快

    for(auto e :v)
    {
        s.insert(e);
    }

    //

}

void test_map()
{
    unordered_map<int,string> l;
    l[1]="11";
    //和map学的都几乎是一样的

}
class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        //先进行排序
        multiset<int> s1;
        multiset<int> s2;
        for(auto e:nums1)
        {
            s1.insert(e);
        }
           for(auto e:nums2)
        {
            s2.insert(e);
        }
        auto it1=s1.begin();
        auto it2=s2.begin();
        //如果一个小于另一个，小的那个++
        //如果两个指向的值相同，那么就进入我们要的返回数组里面
        //两个同时++，直到一方走完
        
        vector<int> ret;
        while(it1!=s1.end()&&it2!=s2.end())
        {
            if(*it1==*it2)
            {
                ret.push_back(*it1);
                it1++;
                it2++;
            }
            else if(*it1<*it2)
            {
                it1++;
            }
            else
            {
                it2++;
            }
        }
        return ret;
    }
};

void test_leetcode()
{
    Solution s;
    vector<int> a={1,2,2,1};
    vector<int> b={2,2};

    vector<int> ret=s.intersect(a,b);
    for(auto e:ret)
    {
        cout<<e<<endl;
    }
}


int main()
{
    // test_set();
    // test_op();
    // test_map();
    test_leetcode();

    return 0;
}