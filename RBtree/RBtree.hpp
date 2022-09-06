#pragma once
#include <iostream>
using namespace std;

enum Color
{
    RED,
    BLACK
};
template <class K, class V>

struct RBTreeNode
{
    RBTreeNode *_left;
    RBTreeNode *_right;
    RBTreeNode *_parent;
    pair<K, V> _kv;
    Color _col; //控制颜色
    RBTreeNode(const pair<K, V> &kv)
        : _kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr), _col(RED)//一开始的颜色给红色
    {
    }
};

template <class K, class V>
class RBTree
{
    typedef RBTreeNode<K, V> Node;

private:
    Node *_root;

private:
    void _InOrder(Node *root)
    {
        if (root == nullptr)
            return;
        _InOrder(root->_left);
        cout << root->_kv.first << " ";
        _InOrder(root->_right);
    }
    void RotateR(Node *parent)
    {
        //右单旋转
        Node *subL = parent->_left;           //子节点
        Node *subLR = subL->_right;           //子节点的右节点
        Node *parentparent = parent->_parent; //出问题节点的父节点
        parent->_left = subLR;
        subL->_right = parent;
        parent->_parent = subL;
        if (subLR)
            subLR->_parent = parent;

        //和父节点的父节点连接
        if (parent == _root)
        {
            //要旋转的节点已经是根
            //更新根
            _root = subL;
            _root->_parent = nullptr; //更新顶部
        }
        else
        {
            //父节点上面还有节点
            if (parentparent->_left == parent)
            {
                //是左节点
                subL->_parent = parentparent;
                parentparent->_left = subL;
            }
            else
            {
                subL->_parent = parentparent;
                parentparent->_right = subL;
            }
        }
        
    }
    void RotateL(Node *parent) //左单旋
    {
        Node *subR = parent->_right;
        Node *subRL = subR->_left;
        Node *parentparent = parent->_parent;
        parent->_parent = subR;
        subR->_left = parent;
        parent->_right = subRL;
        if (subRL)
            subRL->_parent = parent;
        if (parent == _root)
        {
            _root = subR;
            _root->_parent = nullptr;
        }
        else
        {
            if (parentparent->_right == parent)
            {
                parentparent->_right = subR;
                subR->_parent = parentparent;
            }
            else
            {
                parentparent->_left = subR;
                subR->_parent = parentparent;
            }
        }
    }

public:
    RBTree()
        : _root(nullptr)
    {
    }
    bool Insert(const pair<K, V> &kv)
    {
        if (_root == nullptr)
        {
            _root = new Node(kv);
            _root->_col = BLACK;
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
        cur->_col = RED; //新增节点颜色给红色

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
        //插入之前一定是红黑树
        //这里就要控制平衡，通过颜色
        //插入黑节点（影响路径），还是红节点（只影响自己的，不影响其他区域）

        //往上调整
        //我为红，父亲如果为红，爷爷一定是黑色
        // 1.叔叔存在且为红
        while (parent && parent->_col == RED) //往上处理,父亲颜色为红色，就要处理，黑色就不要处理,为红，就不是根
        {
            Node *grandparent = parent->_parent;
            if (parent == grandparent->_left)
            {
                Node *uncle = grandparent->_right; //叔叔就是在右边
                //叔叔存在且为红
                if (uncle && uncle->_col == RED)
                {
                    //叔叔存在，且为红
                    //变色，继续向上处理
                    parent->_col = BLACK;
                    uncle->_col = BLACK;
                    grandparent->_col = RED;

                    cur = grandparent; //继续迭代往上操作
                    parent = cur->_parent;
                }

                //叔叔不存在
                else //不存在，或存在且为黑
                {
                    //不存在,进行旋转
                    //旋转+变色
                    if (cur == parent->_left)
                    {
                        RotateR(grandparent); //进行右单旋转
                        //变色
                        parent->_col = BLACK;
                        grandparent->_col = RED;
                    }
                    else
                    {
                        //双旋
                        //左右双旋
                        // p进行右单旋，g左单旋，g变红藕，cur变黑，
                        RotateL(parent);
                        RotateR(grandparent);
                        cur->_col = BLACK;
                        grandparent->_col = RED;
                    }
                    break;
                }
            }
            else //父亲是祖父的右边
            {
                Node *uncle = grandparent->_left; //叔叔就是在右边
                //叔叔存在且为红
                if (uncle && uncle->_col == RED)
                {
                    //叔叔存在，且为红
                    //变色，继续向上处理
                    parent->_col = BLACK;
                    uncle->_col = BLACK;
                    grandparent->_col = RED;

                    cur = grandparent; //继续迭代往上操作
                    parent = cur->_parent;
                }

                //叔叔不存在
                else //不存在，或存在且为黑
                {
                    //不存在,进行旋转
                    //旋转+变色
                    if (cur == parent->_right)
                    {
                        RotateL(grandparent); //进行右单旋转
                        //变色
                        parent->_col = BLACK;
                        grandparent->_col = RED;
                    }
                    else
                    {
                        RotateR(parent);
                        RotateL(grandparent);
                        cur->_col = BLACK;
                        grandparent->_col = RED;
                    }
                    break;
                }
            }
        }
        _root->_col = BLACK; //根的一定是黑色

        return true;
    }
    bool IsBalance()
    {
        if(_root&&_root->_col==RED)
        {
            cout<<"根节点是黑色"<<endl;
            return false;
        }
        int basevalue=0;//基准值
        Node* left=_root;
        while(left)
        {
            if(left->_col==BLACK)
            {
                basevalue++;
            }
            left=left->_left;
        }

        //用最左路径黑色节点的数量，做基准值
        int blacknum=0;//每个节点的黑色个数
        return _IsBalance(_root,basevalue,blacknum);
    }
    void InOrder() //不能验证是不是AVL树
    {
        _InOrder(_root);
    }

private:
    bool _IsBalance(Node* root,int basevalue,int blacknum)
    {
        //根节点是黑的
        //红色，孩子是黑色,没有连续的红节点
        //每个路径含有相同的黑色
        if(root==nullptr)
        {
            //一条路径走完了
            if(basevalue!=blacknum)
            {
                //每条路劲都有blacknum
                cout<<"存在黑色节点数量不相等"<<endl;
                return false;
            }
            return true;
        }
        if(root->_col==RED&&root->_parent->_col==RED)//红节点
        {
            //检查它的父亲,一定有  
            cout<<"出现连续的红节点"<<endl;
            return false;
        }
        if(root->_col==BLACK)
        {
            blacknum++;
        }
        return _IsBalance(root->_left,basevalue,blacknum)&&_IsBalance(root->_right,basevalue,blacknum);//没有用引用，下面的加加，不会影响下面
    }
};

void test()
{
    RBTree<int, int> rbt;
    int a[]={16,3,7,11,9,26,18,14,15};
    // int a[] = {4,2,6,1,3,5,15,7,16,14};
    for (auto e : a)
    {
        rbt.Insert(make_pair(e, e));
    }
    cout<<rbt.IsBalance()<<endl;
    rbt.InOrder();
}