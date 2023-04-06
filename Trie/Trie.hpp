#pragma once
#include <iostream>
#include<string>
using namespace std;
//用途
//1.前缀匹配
//2.词频统计


#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <vector>
using namespace std;

struct TrieNode {
    char l;//对应的字符
    bool wordFinish = false;//是否是一个字符串
    int count = 0;
    int num = 0;//字符串出现次数
    TrieNode *next[26];
};

class Trie {
public:
    Trie() : root(new TrieNode()) {}

    void insert(string word)//插入函数
    {
        TrieNode *cur = root;
        for (char ch : word)
        {
            if (!cur->next[ch - 'a'])//如果当前字符不存在就需要进行新建
            {
                cur->next[ch - 'a'] = new TrieNode();
            }
            cur->next[ch - 'a']->l = ch;
            cur = cur->next[ch - 'a'];
        }
        cur->wordFinish = true;//cur走到了最后一个位置,这个位置说明是一个字符串，我们需要进行标记
        cur->num++;//出现了我们就需要把他的出现次数自增
    }

    bool search(string word)//搜索，查看某个字符串是否存在
    {
        TrieNode *cur = root;
        for (char ch : word)
        {
            if (!cur->next[ch - 'a'])//如果当前元素往下走无法找到，说明就失败了
            {
                return false;
            }
            cur = cur->next[ch - 'a'];
        }
        return cur->wordFinish;//走到最后，再判断是否标志位被设置了
    }

    bool startsWith(string prefix)//判断前缀
    {
        TrieNode *cur = root;
        for (char ch : prefix)
        {
            if (!cur->next[ch - 'a'])
            {
                return false;
            }
            cur = cur->next[ch - 'a'];
        }
        return true;//如果能走到最后，说明找到了
    }

    int countWord(string word)//统计一个词出现了几次
    {
        TrieNode *cur = root;
        for (char ch : word)
        {
            if (!cur->next[ch - 'a'])
            {
                return 0;
            }
            cur = cur->next[ch - 'a'];
        }
        return cur->num;
    }

    struct cmp
    {
        bool operator()(const pair<string, int>& a, const pair<string, int>& b)//用来进行词频统计的仿函数
        {
            return a.second > b.second; //维护一个小根堆
        }
    };

    vector<string> topKFrequent(int k) //返回出现频率最高的k个字符串
    {
        //pair的key是字符串，value就是字符串出现的次数
        priority_queue<pair<string, int>, vector<pair<string, int>>, cmp> pq;//用一个优先级队列来维护

        string word;
        dfs(root, word, pq); //深度优先搜索全部字符串，把所有的字符串都放在了pq里面了
        //这个时候pq中保存的就是出现次数最少的、k个节点，
        vector<string> res(k);//把那些元素放到一个数组中
        for (int i = 0; i < k && !pq.empty(); ++i, pq.pop())
        {
            res[i] = pq.top().first; //取出出现频率最高的k个字符串
        }

        return res;
    }

private:
    TrieNode *root;

    void dfs(TrieNode *node, string& word, priority_queue<pair<string, int>, vector<pair<string, int>>, cmp>& pq)//深度优先搜索
    {
        if (node->wordFinish) //遍历到了一个字符串
        {
            pq.emplace(word, node->num);//就把他对应的字符串和出现的次数作为一个pair添加到pq中
            if (pq.size() > 5) //保持队列中只有5个元素，如果个数大于5
            {
                pq.pop();//就需要把pq的顶部去除
            }
        }

        for (int i = 0; i < 26; ++i)//进行深搜
        {
            if (node->next[i])//如果下面的子结点存在
            {
                word.push_back(node->next[i]->l);//就往word里面添加这个字符
                dfs(node->next[i], word, pq);//继续往下面进行遍历
                word.pop_back();//搜索回来，就需要进行一个回溯，所以word就需要把词是的字符给去掉
            }
        }
    }
};
