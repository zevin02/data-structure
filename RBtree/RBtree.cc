#include"RBtree.hpp"
/*
1. 一个节点不是红的就是黑的
2. 根节点是黑的
3. 一个节点是红的，孩子是黑的（没有连续的红色节点）****
4. 对于每个节点，从该节点到后代节点的简单路径，都包含相同的黑色（黑色节点的数量相等）****
5. 每个叶子节点都是黑色的（null节点）


最长路径不超过最短路劲的2倍
    最短路径：全黑
    最长路径: 一黑一红
    假设每条路径黑节点是N，
    n<=random path<=2n
    路径要数到空位置


    左右两边没那么均衡:整体的高度
    假设红黑树中一中路径黑色节点=x
    高度2x>=h>=x

    全黑        一黑一红
    2^x-1<=N<=2^2x-1:N为节点个数
    
    X>=logx(N),x<=log(N)/2
*/
#include"RBtree.hpp"

int main()
{
    
    return 0;
}