#include <deque>

namespace xzw
{
    template <class T, class Container = deque<T>>
    class queue
    {
    public:
        void push(const T &x) //入栈
        {
            _con.push_back(x); //尾插
        }
        void pop()
        {
            _con.pop_front();
        }
        const T &front() const
        {
            return _con.front(); // vector和list都是支持这个
        }
        size_t size() const
        {
            return _con.size();
        }
        const T &back() const
        {
            return _con.back();
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
        queue<int> s;
        queue<int,list<int>> sl;
        s.push(9);
        s.push(8);
        s.push(0);
        s.push(1);
        s.push(3);
        s.push(2);
        while (!s.empty())
        {
            cout << s.front() << endl;
            s.pop();
        }
    }

};
