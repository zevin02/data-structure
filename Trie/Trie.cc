#include"Trie.hpp"

int main()
{
    Trie test;
    test.insert("appli");
    test.insert("ap");
    test.insert("reduse");
    test.insert("application");
    cout<<test.search("appli1")<<endl;
    cout<<test.search("appli")<<endl;
    cout<<test.search("reduse")<<endl;
    cout<<test.startWith("red")<<endl;
    cout<<test.startWith("ap")<<endl;
    return 0;
}