#include "map.hpp"
void test_map()
{
    xzw::map<int,int> s;
    s.insert(make_pair(3,3));
    s.insert(make_pair(1,1));
    s.insert(make_pair(2,2));
    s.insert(make_pair(4,4));
    s[1]=2;
    s[2];
    s[6]=1;
    xzw::map<int,int>::Iterator it=s.begin();
    while(it!=s.end())
    {
        cout<<it->first<<" "<<it->second<<endl;
        it++;
    }
    if(!s.empty())
    {
        cout<<"size="<<s.size()<<endl;
    }


}
int main()
{
    test_map();
    return 0;
}