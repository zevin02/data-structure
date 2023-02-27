#include<iostream>
#include<vector>
using namespace std;

//这个是迭代器不会失效的
void test1()
{
    vector<int> vi{1,-9,3,-2,0,7,-5};
    //expect {1,1,3,3,0,0,7,7};
    auto it=vi.begin();
    while(it!=vi.end())
    {
        if(*it<0)
        {
            it=vi.erase(it);//erase 删除了之前it位置的节点，放回的是删除之后刚才哪个位置的节点，现在往后变了
            //这个迭代器不会失效
        }
        else
        {
            it=vi.insert(it,*it);//在it这个迭代器位置，插入之前it指向的值
            //it指向的是3的位置
            //假设之前的是{1,3,-2,0,-7,5}
            //插入之后{1,3,3,-2,0,-7,5}
            //insert返回的是插入位置的，所以就是第1个3,所以需要向后移动两个位置
            it+=2;//
        }
    }
}


//这个会出现迭代器失效
//1.end提前保存的，在erase之后，这个end就失效了
//2.erase删除了it位置的节点，但是又++it，此时这个it已经失效了，这个就是错的
//it=vi.erase(it);

void test2()
{
    vector<int> vi{1,-9,3,-2,0,7,-5};
    auto it=vi.begin();
    auto end=vi.end();
    while(it!=end)
    {
        if(*it<0)
        {
            vi.erase(it);
            ++it;
        }
    }

}


int main()
{
    test1();
    return 0;
}