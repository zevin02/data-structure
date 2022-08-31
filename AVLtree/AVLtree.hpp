#pragma once
#include <iostream>
using namespace std;
#include<cassert>
template <class K, class V>
struct AVLTreeNode
{
    AVLTreeNode<K, V> *_left;
    AVLTreeNode<K, V> *_right;
    AVLTreeNode<K, V> *_parent; //连接父亲
    pair<K, V> _kv;
    int _bf; // balance factor,这个不是必须的,平衡因子
    AVLTreeNode(const pair<K, V> &kv)
        : _kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr), _bf(0) //新增节点的平衡因子一定是0
    {
    }
};

template <class K, class V>
class AVLTree
{
    typedef AVLTreeNode<K, V> Node;

private:
    Node *_root;

public:
    AVLTree()
        : _root(nullptr)
    {
    }

    bool Insert(const pair<K, V> &kv)
    {
        if (_root == nullptr)
        {
            _root = new Node(kv);
            return true;
        }
        //
        Node *cur = _root;
        Node *parent = nullptr; //还要找到父亲
        while (cur)
        {
            if (cur->_kv.first > kv.first) //比它小，在左边
            {
                parent = cur;
                cur = cur->_left;
            }
            else if (cur->_kv.first < kv.first) //比他大，在右边
            {
                parent = cur;
                cur = cur->_right;
            }
            else
            {
                return false;
            }
        }
        //走到这里，就可以链接上
        cur = new Node(kv);
        cur->_parent = parent; //链接上父亲
        if (parent->_kv.first > kv.first)
        {
            //比他小，在左边
            parent->_left = cur;
        }
        else
        {
            parent->_right = cur;
        }
        //插入后要控制平衡
        //我们在插入之前一定是一个AVL树,新插入的节点平衡因子一定是0，
        //插入在右边，父亲的平衡因子要++，（bf=右-左）
        //插入在左边，父亲的平衡因子要--，（bf=右-左）
        
        //更新以后，如果parent->bf=0,就不用更新了，已近平衡，高度就没有变化了，不会继续影响祖宗
        //如果更新之后，parent->bf=1/-1，说明parent更新前parent是0，现在变成1/-1，有一边变高了,会对上一层有影响
        //如果更新后，parent->bf=2/-2，已经不平衡了,需要旋转处理
        //只有这5种情况
        
        //新增节点只会影响它的祖先，不会影响它的兄弟
        //先更新平衡因子，在如果出现异常的平衡因子，需要旋转平衡处理
        while (parent)//走到根
        {
            if (cur == parent->_left)
            {
                //在左边，parent--
                parent->_bf--;
            }
            else
            {
                parent->_bf++;
            }
            if (parent->_bf == 0)
            {
                //更新结束了，把矮的给填上去了
                break;
            }
            else if(parent->_bf==1||parent->_bf==-1)//继续往上走
            {
                cur = parent;
                parent=parent->_parent;
            }
            else if(parent->_bf==2||parent->_bf==-2)//找到问题的地方，要进行旋转
            {
                //旋转
                
            }
            else
            {
                //之前就有问题,树中的平衡因子就有问题
                assert(false);
            }
        }

        return true;
    }
};
void testavltree()
{
    AVLTree<int,int> t;
    int a[3]={1,2,3};
    for(auto e: a)
    {
        t.Insert(make_pair(e,e));

    }

}