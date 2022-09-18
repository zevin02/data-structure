#pragma once
#include <iostream>
using namespace std;

enum Color
{
    RED,
    BLACK
};
template <class T>

struct RBTreeNode
{
    RBTreeNode<T> *_left;
    RBTreeNode<T> *_right;
    RBTreeNode<T> *_parent;
    T _data;
    Color _col; //控制颜色
    RBTreeNode(const T &data)
        : _data(data), _left(nullptr), _right(nullptr), _parent(nullptr), _col(RED) //一开始的颜色给红色
    {
    }
};

template <class T, class Ref, class Ptr> // Ref返回的是里面的值，Ptr返回的是里面的地址
struct RBTreeIterator
{
    typedef RBTreeNode<T> Node;
    typedef RBTreeIterator<T,Ref, Ptr> Self;
    Node *_node;
    RBTreeIterator(Node *node)
        : _node(node)
    {
    }

    Ref operator*() //这个返回的是T&
    {
        return _node->_data;
    }
    Ptr operator->()
    {
        return &_node->_data;
    }
    Self& operator++()
    {
        // 1. 左边都访问过了，假如右树存在，下一个就要访问右树的最左节点
        // 2. 如果右为空，我所在的子树完了，我是父亲的左边，下一个访问父亲
        // 2.1我是父亲的右边，访问的父亲的父亲

        if (_node->_right) //右不为空
        {
            Node *min = _node->_right; //找右的最左节点
            while (min->_left)
            {
                min = min->_left;
            }
            _node = min;
        }
        else
        {
            //右为空，找孩子是父亲左的祖先
            Node *cur = _node;
            Node* parent=cur->_parent;
            //如果cur是父亲的右
            while(parent&&cur==parent->_right)
            {
                cur=parent;
                parent=parent->_parent;
            }
            //cur是父亲的左
            _node=parent;
        }
        return  *this;
    }
    Self& operator--()
    {
        //右根左，和++反过来的
        //左不为空，到左节点的最右边
        //左为空，是父亲的右节点，到父亲
        //是父亲的左节点，到孩子是父亲的右节点
        if(_node->_left)
        {
            //左不为空，
            Node* max=_node->_left;
            while(max->_right)
            {
                max=max->_right;
            }
            _node=max;
        }
        else
        {
            Node* parent=_node->_parent;
            while(parent&&parent->_left==_node)
            {
                
                _node=parent;
                parent=parent->_parent;
            }
            _node=_node->_parent;

        }

        return *this;
    }

    bool operator!=(const Self& s)
    {
        return _node!=s._node; 
    }

};

// set传的T是key RBTree<K,K,SetKeyOfT>
// map传的T是pair  RBTree<K,pair<K,pair<K,V>,MapKeyOfT>

template <class K, class T, class KeyOfT> // KeyOfT是一个仿函数，我们要取出T的first
class RBTree
{
    typedef RBTreeNode<T> Node;

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
    typedef RBTreeIterator<T, T &, T *> Iterator;
    typedef RBTreeIterator<T,const T&,const T*> const_Iterator;

    //中序遍历就是从小到大的
    Iterator begin() //因为它是走中序，所以第一个是最左节点
    {
        Node *min = _root; //最小值，就是左节点
        while (min && min->_left)
        {
            min = min->_left;
        }

        return Iterator(min);
    }
    Iterator end()
    {
        //最大值的下一个节点
        //最右边的下一个是null

        return Iterator(nullptr);
    }

    Iterator Find(const K& key)//K是为了取出V里面的第一个参数
    {
        Node* cur=_root;
        KeyOfT kot;
        while(cur)
        {
            if(kot(cur->_data)<key)
            {
                cur=cur->_right;
            }
            else if(kot(cur->_data)>key)
            {
                cur=cur->_left;
            }
            else
            {
                return Iterator(cur);
            }
        }
        return end();//没有找到，就返回end
    }

    RBTree()
        : _root(nullptr)
    {
    }
    ~RBTree()
    {
        Destroy(_root);
        _root=nullptr;
    }
    RBTree(const RBTree<K,T,KeyOfT>&t)
    {
        _root=Copy(t._root);
    }
    Node* Copy(Node* root)
    {
        if(root==nullptr)
        {
            return nullptr;
        }
        //前序遍历
        Node* newRoot=new Node(root->_data);
        newRoot->_col=root->_col;

        newRoot->_left=Copy(root->_left);
        newRoot->_right=Copy(root->_right);
        if(newRoot->_left)
        {
            newRoot->_left->_parent=newRoot;
        }
        else if(newRoot->_right)
        {
            newRoot->_right->_parent=newRoot;
        }


        return newRoot;
    }

    RBTree<K,T,KeyOfT>& operator=(RBTree<K,T,KeyOfT> t)
    {
        swap(_root,t._root);//这里的t是拷贝构造
        return *this;

    }
    void Destroy(Node* root)
    {
        if(root==nullptr)
        {
            return;
        }
        Destroy(root->_left);
        Destroy(root->_right);
        delete root;//后序遍历

    }
    pair<Iterator,bool> Insert(const T &data)
    {
        if (_root == nullptr)
        {
            _root = new Node(data);
            _root->_col = BLACK;
            return make_pair(Iterator(_root),true);
        }
        //

        //因为我们需要知道它的T的参数是什么
        //这里我们添加一个
        KeyOfT kot; //使用这个来进行比较,仿函数

        Node *cur = _root;
        Node *parent = nullptr; //还要找到父亲
        while (cur)
        {
            if (kot(cur->_data) > kot(data)) //这里如果是pair，我们就得按照k去比较大小,所以这里我们需要知道它是key还是pair
            {
                //取出它的有效比较对象来进行比较
                parent = cur;
                cur = cur->_left;
            }
            else if (kot(cur->_data) < kot(data)) //比他大，在右边
            {
                //只要把key取出来就都可以使用这个东西
                parent = cur;
                cur = cur->_right;
            }
            else
            {
                return make_pair(Iterator(cur),false);//这个值存在，就返回这个已近有的节点
            }
        }
        //走到这里，就可以链接上
        cur = new Node(data);

        Node* newnode=cur;//把这个新节点给保持起来
        cur->_col = RED; //新增节点颜色给红色

        cur->_parent = parent;              //链接上父亲
        if (kot(parent->_data) > kot(data)) //父亲的值也能取出来
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

        return make_pair(Iterator(newnode),true);
    }
};
