#include <vector>
#include "HashTable.hpp"
using namespace std;
#include<bitset>
template <size_t N> //设置开多大的空间,这里的空间，指的是开多少个bit位
class bit_set
{
private:
    // vector<int> _bits;  //一个字节8个bit位
    vector<char> _bits; //
public:
    bit_set()
    {
        _bits.resize(N / 8 + 1, 0); //我们就多浪费一个字节,都初始化成0
    }
    void set(size_t x) //把对应的x位设置成1
    {
        //先计算x在第几个char里面
        size_t i = x / 8;
        //再计算，它再这个的第几个bit位
        //直接%就行了
        int j = x % 8; //在第j个bit位里面
        //直接把它和那个数进行或一下
        _bits[i] |= (1 << j); //把这个1左移j位
    }
    void reset(size_t x) //把对应的x位设置成0
    {
        //进行按位与
        size_t i = x / 8;
        int j = x % 8;
        _bits[i] &= (~(1 << j)); // 把上面的取反即可
    }
    bool find(size_t x) //查找一下这个数是存在
    {
        size_t i = x / 8;
        int j = x % 8;
        return (bool)_bits[i] & (~(1 << j)); //这样不会改变上面的bits里面的值
    }
};


//位图的应用
//1. 给定100亿个数，设计算法，找到只出现一次的整数
//把里面的每个元素多添加一个值，每次来的时候，先判断这个值是否存在，如果存在就直接++int即可，如果不存在就设置其存在
/*
1. 出现0次
2. 出现1次
3. 出现多次

*/
template<size_t N>
class twobit
{
    //00代表没有
    //01代表1个
    //10代表2个
private:
    bit_set<N> bts1;//
    bit_set<N> bts2;

public:
    void Set(size_t x)//用两个bit位来表示
    {
        if(!bts1.find(x)&&!bts2.find(x))
        {
            //00
            bts2.set(x);     
        }
        else if (!bts1.find(x)&&bts2.find(x))
        {
            //01
            bts2.reset(x);
            bts1.set(x);
        }
        else
        {
            //这个地方就是已经出现了很多次
            bts2.set(x);
            bts1.reset(x);
        }
    }
    void PrintOnceNum()
    {
        for(size_t i=0;i<N;i++)
        {
            if(!bts1.find(i)&&bts2.find(i))
            {
                //这个就是只出现一次
                cout<<i<<endl;
            }
        }
    }

};


//2.给两个文件，分别由100亿个整数，我们只有1G内存，如何找到两个文件的交集
//思路1，一个文件中的整数，set到一个位图，读取第二个整数是否在一个位图，在就有，不在就没有交集,这个就要用O（N），交集中，会把重复值找出来，多次出现

//思路2，读取一个整数设计到位图bs1，再把另一个文件中的整数，set到位图bs2，
//a.把bs1中的值，一次和bs2中的值进行与一下，再去看，与完是1的位置的值，就是交集,全1是1





//3. 一个文件中100亿个int，1G内存，找到出现次数不超过2次的整数

int main()
{
    bit_set<100> bts; //开一个100个空间的大小
    bts.set(5);
    bts.set(4);
    bts.set(10);
    bts.set(22);
    bts.set(20);
    cout << bts.find(4) << endl;
    cout << bts.find(22) << endl;
    cout << bts.find(20) << endl;
    bts.reset(5);
    // bit_set<0xffffffff> bs;//这样就可以了,
    // bit_set<-1> bs;//这样也可以，因为它被认为是补码，最大值
    //库里面也是有位图的
    // bitset<-1> l;
    // l.set(21);
    // l.test(22);//测试你这个位置是否有数字
    twobit<100> bbs;
    int a[]={99,0,4,50,33,44,2,5,99,0,50,99,50,2};
    for(auto e: a)
    {
        bbs.Set(e);
    }
    bbs.PrintOnceNum();
    return 0;
}
