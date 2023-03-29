#pragma once
#include <iostream>
#include<string>
using namespace std;


class Trie
{
private:
    struct TrieNode
    {
        char l;//表示当前字母是否
        bool wordFinish=false;    //判断当前这个节点是否是这个字符床的最后一个节点
        //如果没有就指向NULL
        TrieNode *next[26]; //因为字母a-z就只有26个,当前节点可以指向的下一个节点位置
    };

public:
    Trie()
    :root(new TrieNode)
    {
        //初始化根节点
    }
    void insert(string word)//插入一个字符串
    {
        TrieNode* cur=root;
        for(int i=0;i<word.size();i++)
        {
            char ch=word[i];
            //插入节点
            if(cur->next[ch-'a']==nullptr)
            {
                cur->next[ch-'a']=new TrieNode();//开辟一个节点
            }
            cur=cur->next[ch-'a'];//继续往下面遍历
            cur->l=ch;//把下一个节点的值进行设置
            if(i==word.size()-1)
            {
                //说明遍历到了最后一个元素，我们就要设置他的wordfinish标志位是true
                cur->wordFinish=true;
            }
        }
    }
    bool search(string word)
    {
        //在前缀树中查找对应的word
        TrieNode* cur=root;
        for(int i=0;i<word.size();i++)
        {
            char ch=word[i];
            if(cur->next[ch-'a']==nullptr)
            {
                //没找到，直接返回失败
                return false;
            }
            else
            {
                //找到了
                cur=cur->next[ch-'a'];
                if(i==word.size()-1&&cur->wordFinish)
                {
                    //遍历到了最后一个元素，同时还有标志位，说明找到了
                    return true;
                }
                else if(i==word.size()-1)
                {
                    return false;//没找到，查询失败
                }
            }
        } 
return false;
    }
    //查询prefix前缀的串是否存在
    bool startWith(string prefix)
    {
        TrieNode* cur=root;
        for(int i=0;i<prefix.size();i++)
        {
            char ch=prefix[i];
            if(cur->next[ch-'a']==nullptr)
            {
                return false;
            }
            else
            {
                cur=cur->next[ch-'a'];
                if(i==prefix.size()-1)
                {
                    return true;//查找到了
                }
            }
        }
        return true;
    }
private:
    TrieNode* root;//根节点
};