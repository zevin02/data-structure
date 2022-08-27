#pragma once
#include <iostream>
using namespace std;
namespace K
{
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

        bool _InsertR(Node *&root, const K &key) //这里要多一个引用,指针的引用，进行修改才使用引用
        {
            if (root == nullptr) //走到空就插入,引用，root是上面的root->right别名
            {
                root = new Node(key); //直接链接上了 ，都是前面的别名，都连接起来了
                return true;
            }
            if (root->_val > key)
            {
                return _InsertR(root->_left, key);
            }
            else if (root->_val < key)
            {
                return _InsertR(root->_right, key);
            }
            else
            {
                return false;
            }
        }

        bool InsertR(const K &key) //有序的方式就会变成单边树，栈爆了
        {
            //递归有根，都要套一个子函数
            return _InsertR(_root, key);
        }

        Node *FindR(const K &key)
        {
            return _Find(_root, key);
        }
        Node *_FindR(Node *root, const K &key)
        {
            if (root == nullptr)
            {
                return nullptr;
            }
            if (root->_val == key)
            {
                return root;
            }
            else if (root->_val > key)
            {
                return _FindR(root->_left, key);
            }
            else
            {
                return _FindR(root->_right, key);
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
                            node = nullptr;
                        }
                        else
                        {
                            swap(cur->_val, node->_val);
                            parent->_right = node->_right;
                            delete node;
                            node = nullptr;
                        }
                    }

                    return true;
                }
            }
            return false;
        }

        Node *EraseR(const K &key)
        {
            _EraseR(key, _root);
        }
        bool _EraseR(const K &key, Node *&root)
        {
            if (root == nullptr)
                return false;
            if (root->_val > key)
            {
                return _EraseR(key, root->_left);
            }
            else if (root->_val < key)
            {
                return _EraseR(key, root->_right);
            }
            else
            {
                Node *del = root;
                if (root->_left == nullptr)
                {

                    root = root->_right;
                }

                else if (root->_right == nullptr)
                {
                    root = root->_left;
                }
                else
                {
                    //左右都不为空
                    //找到右子树的最左边，找替代节点

                    Node *min = root->_right;
                    while (min->_left)
                    {
                        min = min->_left;
                    }
                    swap(min->_val, root->_val); //把替代节点替换上去，后面只需要再递归的去删除交换的节点就可以了
                    //递归到右子树的最左节点去删除
                    return _EraseR(key, root->_right);
                }
                delete del;
                return true;
            }
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
        s.EraseR(7);
        // s.Erase(3);
        // s.Erase(1);
        s.InOrder();
        if (s.Find(1))
        {
            cout << "找到了" << endl;
        }
    }
};

/*

    搜索树的应用
    1. 搜索，key搜索模型，key/value模型
    2.排序+去重

    key搜索模型
    判断在不在：
   1.  搜索树存储小区业主的车牌号，扫以下车，
   2.  搜索树存储同学的学号
   3.  给你一篇英文的作文，写程序检查一下作文中单词拼写是否正确(把词库中的单词都放进搜索树里面，在就拼写正确，不在就拼写错误

   key/value模型
   通过一个值找另外一个值
   高铁站刷身份证进站
*/

namespace KV
{
    template <class K,class V>

    struct BSTNode
    {
        K _key;
        V _val;
        BSTNode<K,V> *_left;
        BSTNode<K,V> *_right;
        BSTNode(const K &key,const V& val)
            : _val(val), _right(nullptr), _left(nullptr),_key(key)
        {
        }
    };

    //比如：key是身份证，value就是票的信息，vector<info> _vinfo
    template <class K,class V>
    struct BSTree
    {
        typedef BSTNode<K,V> Node;

    private:
        Node *_root;

    public:
        BSTree()
            : _root(nullptr)
        {
        }

        bool Insert(const K &key,const V& val) //非线性叫做insert
        {
            //插入重复的值，就返回失败
            if (_root == nullptr)
            {
                _root = new Node(key,val);
                return true;
            }
            else
            {
                Node *cur = _root;
                Node *prev;
                while (cur)
                {
                    if (cur->_key > key)
                    {
                        prev = cur;
                        cur = cur->_left;
                    }
                    else if (cur->_key < key)
                    {
                        prev = cur;
                        cur = cur->_right;
                    }
                    else
                    {
                        return false;
                    }
                }
                cur = new Node(key,val);
                if (prev->_key < key)
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

        

        Node* Find(const K &key) //查找,返回对应的节点
        {
            Node *cur = _root;
            while (cur)
            {
                if (cur->_key > key)
                {
                    cur = cur->_left;
                }
                else if (cur->_key < key)
                {
                    cur = cur->_right;
                }
                else
                {
                    return cur;//找到就返回对应的节点
                }
            }
            return nullptr;//没有找到就返回空
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
            cout << root->_key << " ";
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
                if (cur->_key < key)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else if (cur->_key > key)
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
                            swap(cur->_key, node->_key);
                            parent->_left = node->_right;
                            delete node;
                            node = nullptr;
                        }
                        else
                        {
                            swap(cur->_val, node->_val);
                            swap(cur->_key, node->_key);
                            parent->_right = node->_right;
                            delete node;
                            node = nullptr;
                        }
                    }

                    return true;
                }
            }
            return false;
        }

        
    };
#include<string>
    void testdict()
    {
        BSTree<string,string> dict;
        dict.Insert("sort","排序");
        dict.Insert("up","上升");
        dict.Insert("left","左");
        dict.Insert("right","右边");
        string str;
        while(cin>>str)
        {
            BSTNode<string,string> * ret=dict.Find(str);
            if(ret!=nullptr)
            {
                cout<<ret->_val<<endl;
            }
            else
            {
                cout<<"nobody"<<endl;
            }
        }
    }

    
};