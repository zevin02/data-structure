#include"skip_list.hpp"

int main()
{
    skipList skip_list(6);
    srand(time(0));
    
    skip_list.insert(13);
    skip_list.insert(1);
    skip_list.insert(4);
    skip_list.insert(16);
    skip_list.insert(33);
    skip_list.printSkipList();
    cout<<skip_list.search(33)<<endl;
    cout<<skip_list.search(313)<<endl;
    skip_list.earse(13);
    // skip_list.printSkipList();
    return 0;
}