#include "unordered_set.hpp"
#include "unordered_map.hpp"

int main()
{
    xzw::unordered_set<int> kt;
    kt.insert(1);
    kt.insert(3);
    kt.insert(2);
    kt.insert(5);
    kt.insert(11);
    kt.insert(22);
    kt.insert(23);
    xzw::unordered_set<int>::Iterator it = kt.begin();
    while (it != kt.end())
    {
        cout << *it << " ";
        ++it;
    }
    xzw::unordered_map<int, int> kl;
    kl.insert(make_pair(1, 1));
    kl.insert(make_pair(2, 3));
    kl.insert(make_pair(12, 4));
    kl[2] = 5;
    xzw::unordered_map<int,int>::Iterator dit=kl.begin();
    while (dit != kl.end())
    {
        cout << dit->first<<"   "<<dit->second << " "<<endl;
        ++dit;
    }
    cout<<kl[12]<<endl;
    xzw::unordered_map<int,int> copy(kl);
    for(auto e:copy)
    {
        cout<<e.first<<"  "<<e.second<<endl;
    }

    return 0;
}