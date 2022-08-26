#include <iostream>
using namespace std;
#include <vector>
namespace xzw
{

    template <class T>
    struct Less
    {
        bool operator()(const T &x, const T &y)const 
        {
            return x < y;
        }
    };
    template <class T, class Container = vector<int>, class Compare = Less<T>> //默认是less就是建一个大堆

    //完全二叉树
    //父=（子-1）/2；
    //左子树=父*2+1，右子树=父*2+2；
    //默认实现大堆
    class priority_queue
    {
    public:
        priority_queue()
        {
        }

        void push(const T &x)
        {
            //入堆
            _con.push_back(x);
            //向上调整
            //比它小，交换
            adjust_up(_con.size() - 1);
        }
        void adjust_up(size_t child)
        {
            Compare com;
            size_t parent = (child - 1) / 2;
            while (child > 0)
            {
                if (com(_con[parent], _con[child])) //相当于parent<child==》大堆
                {
                    swap(_con[parent], _con[child]);
                    child = parent;
                    parent = (parent - 1) / 2;
                }
                else
                {
                    break;
                }
            }
        }

        size_t size()
        {
            return _con.size();
        }
        void pop()
        {
            //删除堆顶的数据
            //先和尾巴进行交换以下，然后再向下调整
            swap(_con[0], _con[_con.size() - 1]);
            _con.pop_back();
            adjust_down(0);
        }
        void adjust_down(size_t parent)
        {
            //向下调，比它大，就交换
            parent = 0;
            size_t child = (parent * 2) + 1;
            //我现在不知道谁比较大
            Compare com;
            while (child < _con.size()) //排到最后一个就结束了
            {
                if (child + 1 < _con.size() && com(_con[child], _con[child + 1])) //避免段错误,child<child+1
                    child++;
                if (com(_con[parent], _con[child]))//parent<child
                {
                    swap(_con[child], _con[parent]);
                    parent = child;
                    child = parent * 2 + 1;
                }
                else
                {
                    break;
                }
            }
        }
        const T &top()
        {
            return _con[0];
        }
        bool empty()
        {
            return !_con.size();
        }
        template <class InputIterator>

        priority_queue(InputIterator first, InputIterator last)
            : _con(first, last) // vector里面也可用迭代器区间初始化，所以这里直接就可以了
        {
            // while(first!=last)
            // {
            //     push(*first);
            //     first++;
            // }

            //现在这个是一个随机数，要构建成大堆
            //保证下面的每一个儿子都是一个大堆,
            for (int i = (_con.size() - 1 - 1) / 2; i >= 0; i--)
            {
                //从最后一个节点的父亲
                adjust_down(i);
            }
        }

    private:
        Container _con;
    };
    template<class T>
    //如果数据类型不支持比较，就要自己写一个仿函数,控制比较，
    struct greater
    {
        bool operator()(const T &x, const T &y)const 
        {
            return x > y;
        }
    };
    void test()
    {
        priority_queue<int,vector<int>,greater<int>> pq;//省略要从右完做缺省
        pq.push(1);
        pq.push(3);
        pq.push(2);
        pq.push(4);
        pq.push(5);
        while (!pq.empty())
        {
            cout << pq.top() << endl;
            pq.pop();
        }
    }
};