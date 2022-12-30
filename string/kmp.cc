#include <iostream>
using namespace std;
#include <vector>
#include <string>
void getnext(string ch, int length, vector<int> &next)
{
    next[1] = 0;
    int i = 1;
    int j = 0;
    while (i <= length)
    {
        if (j == 0 || ch[i] == ch[j])
        {
            next[++i] = ++j;
        }
        else
        {
            j = next[j];
        }
    }
}

bool kmpindex(string& src, string& mstr,vector<int>& next)
{

    for (int i = 1, j = 1; i <= src.length() && mstr.length(); i++, j++)
    {
        if (mstr[j] != src[i])
        {
            // 这个时候的j就是我们需要的字符串
            int pos =next[j];
            int index=pos;
            // 从第pos个位置开始和主串的pos开始比
            for (; pos < mstr.size(); pos++, i++)
            {
                if (mstr[pos] != src[i])
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

int main()
{
    string src = "xabaabaabcabaabc";
    string mstr = "xabaabc";
    vector<int> next = {0, 0,1};
    src[0]=src.size()-1;
    mstr[0]=mstr.size()-1;
    
    next.resize(mstr.size());
    getnext(mstr, mstr.size(), next);
    if(kmpindex(src,mstr,next))
    {
        cout<<"find"<<endl;
    }
    else
    cout<<"not find"<<endl;
    return 0;
}