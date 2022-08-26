#pragma once
#include <vector>
#include <list>
#include<queue>
namespace xzw
{
    template <class T, class Container=deque<T>>
    class stack
    {
    public:
        void push(const T &x) //入栈
        {
            _con.push_back(x); //尾插
        }
        void pop()
        {
            _con.pop_back();
        }
        const T &top()
        {
            return _con.back(); // vector和list都是支持这个
        }
        size_t size() const
        {
            return _con.size();
        }

        bool empty() const
        {
            return _con.empty();
        }

    private:
        Container _con; //适配器，栈既可以用stack也可以用list
    };

    void test()
    {
        stack<int, vector<int>> sv; //数组栈
        stack<int/*, list<int>*/> sl;   //链表栈
        sv.push(1);
        sv.push(2);
        sv.push(1);
        sv.pop();

        sl.push(2);
        sl.push(3);
        sl.push(1);
        sl.push(4);

        while (!sl.empty())
        {
            cout << sl.top() << endl;
            sl.pop();
        }

        while (!sv.empty())
        {
            cout << sv.top() << endl;
            sv.pop();
        }
    }
};