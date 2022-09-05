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
        : _kv(kv)
    {
    }
};

template <class K, class V>
class RBTree
{
    typedef RBTreeNode<K, V> Node;

private:
    Node *_root;

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
                        //p进行右单旋，g左单旋，g变红藕，cur变黑，
                        RotateL(parent);
                        RotateR(grandparent);
                        cur->_col=BLACK;
                        grandparent->_col=RED;
                        parent->_col=RED;
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
                        cur->_col=BLACK;
                        grandparent->_col=RED;
                        parent->_col=RED;
                    }
                    break;
                }
            }
        }
        _root->_col = BLACK; //根的一定是黑色

        return false;
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
        //现在根的bf=0
        //右边的是bf=0
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
};