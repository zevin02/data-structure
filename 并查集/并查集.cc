#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

namespace ufs1
{

    template <class T>

    class UnionFindSet
    {
    public:
        UnionFindSet(vector<T>& a, size_t n)
            : _ufs(n, -1)
        {
            for (int i = 0; i < n; i++)
            {
                _a.push_back(a[i]);
                _indextmap[a[i]] = i;
            }
        }
        T FindRoot(T x) // 找到根
        {
            int root = _indextmap[x];
            while (_ufs[root] >= 0)
            {
                root = _ufs[root];
            }
            return _a[root];
        }
        void Union(T x1, T x2) // 合并两个树,两个树分别有两个节点叫x1,x2
        {
            // 1. 合并之前先找到他们的根,检查在不在一个集合中

            T root1 = FindRoot(x1);
            T root2 = FindRoot(x2);

            if (root1 == root2) // 本身就在一个集合，没必要合并
                return;

            // 我们把root2合并到root1
            _ufs[_indextmap[root1]] += _ufs[_indextmap[root2]];
            _ufs[_indextmap[root2]] = _indextmap[root1];
        }
        bool Inset(T x1, T x2)
        {
            T root1 = FindRoot(x1);
            T root2 = FindRoot(x2);

            return root1 == root2; // 相等就在一个集合
        }

    private:
        vector<T> _a;           // 编号可以找人
        map<T, int> _indextmap; // 人找编号
        vector<int> _ufs;       // 计算
    };
    class Solution
    {
    public:
        bool equationsPossible(vector<string> &equations)
        {
            vector<char> ch;
            char a = 'a';
            for (; a <= 'z'; a++)
            {
                ch.push_back(a);
            }
            UnionFindSet<char> ufs(ch, 26);
            for (auto &str : equations)
            {
                // 如果第二个元素=
                if (str[1] == '=')
                {
                    ufs.Union(str[0], str[3]);
                }
            }
            for (auto &str : equations)
            {
                if (str[1] == '!')
                {
                    if (ufs.Inset(str[0], str[3]))
                    {
                        return false;
                    }
                }
            }
            return true;
        }
    };

    void test1()
    {
        // string a[] = {"张三", "李四", "王五", "赵六"};
        // UnionFindSet<string> ufs(a, 4);
        // Solution st;
        // st.equationsPossible()
    }
};

namespace ufs2
{
    class UnionFindSet
    {
    public:
        UnionFindSet(size_t n)
            : _ufs(n, -1)
        {
        }
        void Union(int x1, int x2) // 合并两个树,两个树分别有两个节点叫x1,x2
        {
            // 1. 合并之前先找到他们的根,检查在不在一个集合中

            int root1 = FindRoot(x1);
            int root2 = FindRoot(x2);

            if (root1 == root2) // 本身就在一个集合，没必要合并
                return;

            // 我们把root2合并到root1
            _ufs[root1] += _ufs[root2];
            _ufs[root2] = root1;
        }
        int FindRoot(int x) // 找到根
        {
            int root = x;
            while (_ufs[root] >= 0)
            {
                root = _ufs[root];
            }
            return root;
        }
        bool Inset(int x1, int x2)
        {
            int root1 = FindRoot(x1);
            int root2 = FindRoot(x2);

            return root1 == root2; // 相等就在一个集合
        }
        size_t size() // 计算有几个元素
        {
            int cnt = 0;
            for (int i = 0; i < _ufs.size(); i++)
            {
                if (_ufs[i] < 0)
                    cnt += _ufs[i];
            }
            return (-cnt);
        }
        size_t SetSize() // 计算有几科树
        {
            size_t size = 0;
            for (int i = 0; i < _ufs.size(); i++)
            {
                if (_ufs[i] < 0)
                    size++;
            }
            return size;
        }

    private:
        vector<int> _ufs;
    };
    class Solution
    {
    public:
        int findCircleNum(vector<vector<int>> &isConnected)
        {
            // 计算森林有多少个
            UnionFindSet ufs(isConnected.size());
            for (size_t i = 0; i < isConnected.size(); i++)
            {
                for (int j = 0; j < isConnected[i].size(); j++)
                {
                    if (isConnected[i][j] == 1)
                    {
                        //=1说明两个城市之间有连接
                        ufs.Union(i, j);
                    }
                }
            }
            return ufs.SetSize();
        }
    };
    void demo()
    {
        Solution st;
        vector<vector<int>> vt = {{1, 0, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1}, {1, 0, 1, 1}};
        int ret = st.findCircleNum(vt);
    }
    void test()
    {
        vector<int> s = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        UnionFindSet ufs(10);
    }
};

int main()
{
    ufs1::test1();
    ufs2::demo();
    return 0;
}