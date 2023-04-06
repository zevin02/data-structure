#include <iostream>
#include <vector>
#include <string>
#include "Trie.hpp" //包含前面添加的类定义

using namespace std;

int main()
{
    vector<string> words = {"apple", "banana", "cat", "dog", "egg", "fly", "apple", "cat", "dog", "dog", "egg", "egg", "egg"};
    Trie trie;
    for (auto word : words)
    {
        trie.insert(word); //插入字符串
    }

    //测试词频统计
    cout << "word 'egg' appears " << trie.countWord("egg") << " times." << endl;
    cout << "word 'cat' appears " << trie.countWord("cat") << " times." << endl;
    cout << "word 'xyz' appears " << trie.countWord("xyz") << " times." << endl;

    //测试出现频率最高的5个词
    cout << "Top 5 frequent words:" << endl;
    vector<string> topWords = trie.topKFrequent(5);
    for (string word : topWords)
    {
        cout << word << endl;
    }

    return 0;
}