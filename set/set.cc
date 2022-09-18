#include "set.hpp"
void test_set()
{
    xzw::set<int> s;
    s.insert(1);
    if(!s.insert(1).second)
    {
        cout<<"has 1"<<endl;
    }
    s.insert(3);
    s.insert(6);
    s.insert(2);
    xzw::set<int>::Iterator it=s.begin();
    
    // while(it!=s.end())
    // {
    //     cout<<(*it)<<" ";
    //     ++it;
    // }
    ++it;
    cout<<*it<<endl;
    ++it;
    cout<<*it<<endl;
    --it;
    cout<<*it<<endl;
    --it;
    cout<<*it<<endl;
    if(s.Find(4)!=nullptr)
    {
        cout<<"cunzai"<<endl;
    }
    xzw::set<int> p(s);//拷贝构造
    s.insert(9);
    s.insert(8);
    p=s;
    for(auto e: p)
    {
        cout<<e<<" ";
    }
}
int main()
{
    test_set();
    return 0;
}