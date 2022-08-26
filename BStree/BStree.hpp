#pragma once
#include <iostream>
using namespace std;
template <class K>

struct BSTNode
{
    K _val;
    BSTNode<K> *_left;
    BSTNode<K> *_right;
    BSTNode(const K &val)
        : _val(val), _right(nullptr), _left(nullptr)
    {
    }
};

template <class K>
struct BSTree
{
    typedef BSTNode<K> Node;

private:
    Node *_root;

public:
    BSTree()
        : _root(nullptr)
    {
    }

    bool Insert(const K &key) //非线性叫做insert
    {
        //插入重复的值，就返回失败
        if (_root == nullptr)
        {
            _root = new Node(key);
            return true;
        }
        else
        {
            Node *cur = _root;
            Node *prev;
            while (cur)
            {
                if (cur->_val > key)
                {
                    prev = cur;
                    cur = cur->_left;
                }
                else if (cur->_val < key)
                {
                    prev = cur;
                    cur = cur->_right;
                }
                else
                {
                    return false;
                }
            }
            cur = new Node(key);
            if (prev->_val < key)
            {
                prev->_right = cur;
            }
            else
            {
                prev->_left = cur;
            }
            return true;
        }
    }

    bool _InsertR(Node*& root,const K& key)//这里要多一个引用,指针的引用，进行修改才使用引用
    {
        if(root==nullptr)//走到空就插入,引用，root是上面的root->right别名
        {
            root=new Node(key);//直接链接上了 ，都是前面的别名，都连接起来了
            return true;
        }
        if(root->_val>key)
        {
            return _InsertR(root->_left,key);
        }
        else if(root->_val<key)
        {
            return _InsertR(root->_right,key);
        }
        else
        {
            return false;
        }
    }

    bool InsertR(const K& key)//有序的方式就会变成单边树，栈爆了
    {
        //递归有根，都要套一个子函数
        return _InsertR(_root,key);

    }


    Node* FindR(const K& key)
    {
        return _Find(_root,key);
    }
    Node* _FindR(Node* root,const K& key)
    {
        if(root==nullptr)
        {
            return nullptr;
        }
        if(root->_val==key)
        {
            return root;
        }
        else if(root->_val>key)
        {
            return _FindR(root->_left,key);
        }
        else
        {
            return _FindR(root->_right,key);
        }
    }

    bool Find(const K &key) //查找
    {
        Node *cur = _root;
        while (cur)
        {
            if (cur->_val > key)
            {
                cur = cur->_left;
            }
            else if (cur->_val < key)
            {
                cur = cur->_right;
            }
            else
            {
                return true;
            }
        }
        return false;
    }
    void InOrder()
    {
        //套一个子函数就可以了
        _InOrder(_root);
    }

    void _InOrder(Node *root) //在外面或得不到根
    {
        if (root == nullptr)
            return;
        _InOrder(root->_left);
        cout << root->_val << " ";
        _InOrder(root->_right);
    }

    bool Erase(const K &key)
    {
        //先查找,删除之后要保持搜索二叉树的状态
        //有两个儿子，用左子树的最右节点，或者右子树的最左节点来进行替换删除
        //先找到要删除的值，还要找到它的父亲
        Node *cur = _root;
        Node *parent = nullptr;
        while (cur)
        {
            if (cur->_val < key)
            {
                parent = cur;
                cur = cur->_right;
            }
            else if (cur->_val > key)
            {
                parent = cur;
                cur = cur->_left;
            }
            else
            {
                //这里面就找到了
                //如果删除的是根节点，左为空，或右为空，没有父亲，我们还要单独处理

                if (cur->_left == nullptr) //左为空
                {
                    if (parent == nullptr)
                    {
                        _root = _root->_right;
                    }
                    else
                    {
                        if (parent->_left == cur)
                        {
                            //我是父亲的左
                            //父亲的左，指向我的右
                            parent->_left = cur->_right;
                        }
                        else if (parent->_right == cur)
                        {
                            parent->_right = cur->_right;
                        }
                    }
                    delete cur;
                    cur = nullptr;
                }
                else if (cur->_right == nullptr) //右为空
                {
                    if (parent == nullptr)
                    {
                        _root = _root->_left;
                    }
                    else
                    {
                        if (parent->_left == cur)
                        {
                            //我是父亲的左
                            //父亲的左，指向我的右
                            parent->_left = cur->_left;
                        }
                        else if (parent->_right == cur)
                        {
                            parent->_right = cur->_left;
                        }
                    }
                    delete cur;
                    cur = nullptr;
                }
                else
                {
                    //都不为空
                    //我们用右子树的最左节点,替换法删除
                    Node *node = cur->_right;
                    Node *parent = cur;
                    if (node->_left != nullptr)
                    {

                        while (node->_left)
                        {
                            parent = node;
                            node = node->_left;
                        }
                        swap(cur->_val, node->_val);
                        parent->_left = node->_right;
                        delete node;
                        node=nullptr;

                    }
                    else
                    {
                        swap(cur->_val,node->_val);
                        parent->_right=node->_right;
                        delete node;
                        node=nullptr;
                    }
                }

                return true;
            }
        }
        return false;
    }

    Node* EraseR(const K& key)
    {
        _Erase(root,key);
    }
    Node* _EraseR(const K& key,Node*& root)
    {
        
    }
};

void test()
{
    //搜索二叉树：排序+去重
    BSTree<int> s;
    s.InsertR(5);
    s.InsertR(3);
    s.InsertR(7);
    s.InsertR(1);
    s.InsertR(4);
    s.InsertR(6);
    s.InsertR(8);
    s.InsertR(0);
    s.InsertR(2);
    s.InsertR(9);
    s.Erase(3);
    s.Erase(3);
    s.Erase(1);
    s.InOrder();
    if (s.Find(1))
    {
        cout << "找到了" << endl;
    }
}