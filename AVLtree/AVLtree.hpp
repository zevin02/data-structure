#pragma once
#include <iostream>
using namespace std;
#include <cassert>
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
        while (parent) //走到根
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
                //更新结束了，把矮的给填上去了，就不需要处理了
                break;
            }
            else if (parent->_bf == 1 || parent->_bf == -1) //继续往上走
            {
                cur = parent;
                parent = parent->_parent;
            }
            else if (parent->_bf == 2 || parent->_bf == -2) //找到问题的地方，要进行旋转
            {
                //对在上面没有影响
                //旋转
                // 1.右单旋，左边高，往右边去旋转，根往下嗯下去
                //保持搜索树的规则，控制平衡，让-2的做它左节点的右节点，把它左节点原来的右节点，连接到-2的左节点上
                if (parent->_bf == -2 && cur->_bf == -1) //两个轴点都是左边高
                {
                    //意义：整棵树平衡了
                    //整体高度降低1
                    RotateR(parent);
                    break;
                }
                else if (parent->_bf == 2 && cur->_bf == 1) //两个轴点都是右边高
                {
                    //左单旋
                    //整体高度+1，单纯的一边高
                    RotateL(parent); //右边大
                    break;
                }
                else if (parent->_bf == -2 && cur->_bf == 1)
                {
                    //有的左边高，有的右边高,折现，
                    //使用两次单旋

                    // cur的右边高，parent的左边高
                    //先对cur进行左单旋
                    RotateLR(parent); //左右双旋转
                    break;
                }
                else if (parent->_bf == 2 && cur->_bf == -1)
                {
                    //有的左边高，有的右边高,折现，
                    //使用两次单旋

                    // cur的右边高，parent的左边高
                    //先对cur进行左单旋
                    RotateRL(parent); //左右双旋转
                    break;
                }
            }
            else
            {
                //之前就有问题,树中的平衡因子就有问题
                assert(false);
            }
        }

        return true;
    }
    void RotateLR(Node *parent) //左右双旋
    {
        //先对子进行一个左单旋
        Node *subL = parent->_left;
        Node *subLR = subL->_right;
        int bf = subLR->_bf; //三种情况

        RotateL(parent->_left);
        RotateR(parent);
        if(bf==0)
        {
            subL->_bf=0;
            subLR->_bf=0;
            parent->_bf=0;
        }
        else if(bf==1)
        {
            subL->_bf=-1;
            subLR->_bf=0;
            parent->_bf=0;
        }
                else if(bf==-1)
        {
            subL->_bf=0;
            subLR->_bf=0;
            parent->_bf=1;
        }
        else
        {
            assert(false);
        }

    }
    void RotateRL(Node *parent) //右左双旋
    {
        //先对子进行一个左单旋
        Node *subR = parent->_right;
        Node *subRL = subR->_left;
        int bf = subRL->_bf; //三种情况

        RotateR(parent->_right);
        RotateL(parent);
        if (bf == 1)
        {
            parent->_bf = -1;
            subR->_bf = 0;
            subRL->_bf = 0;
        }
        else if (bf == -1)
        {
            parent->_bf = 0;
            subR->_bf = 1;
            subRL->_bf = 0;
        }
        else if (bf == 0) //自己就是新增
        {
            parent->_bf = 0;
            subR->_bf = 0;
            subRL->_bf = 0;
        }
        else //一定不会出现的情况
        {
            assert(false);
        }
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
        subL->_bf = 0;
        parent->_bf = 0;
    }
    void _InOrder(Node *root)
    {
        if (root == nullptr)
            return;
        _InOrder(root->_left);
        cout << root->_kv.first << " ";
        _InOrder(root->_right);
    }
    void InOrder() //不能验证是不是AVL树
    {
        _InOrder(_root);
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
        subR->_bf = 0;
        parent->_bf = 0;
    }
    bool IsBalance()
    {
        _IsBalance(_root);
    }
    int Height(Node *root)
    {
        //计算高度
        if (root == nullptr)
            return 0;
        int lefth = Height(root->_left);
        int righth = Height(root->_right);
        return lefth > righth ? lefth + 1 : righth + 1; //高度就是左右子树里面大的那个加1
    }
    bool _IsBalance(Node *root)
    {
        if (root == nullptr)
        {
            return true;
        }
        int LeftHeight = Height(root->_left);
        int RightHeight = Height(root->_right);
        if (RightHeight - LeftHeight != root->_bf) //检查平衡因子是否正确
        {
            cout << root->_kv.first << "现在是:" << root->_bf << endl;
            cout << root->_kv.first << "应该是:" << RightHeight - LeftHeight << endl;
            return false;
        }
        return abs(RightHeight - LeftHeight) < 2 && _IsBalance(root->_left) && _IsBalance(root->_right); //再去检查左右子树
    }
};

void testavltree()
{
    AVLTree<int, int> t;
    // int a[] = {16,3,7,11,9,26,18,14,15};
    int a[] = {4, 2, 6, 1, 3, 5, 15, 7, 16, 14};
    for (auto e : a)
    {
        t.Insert(make_pair(e, e));
        cout << t.IsBalance();
    }
    // t.Insert(make_pair(18,18));
    // pair<int,int> s(18,18);

    // cout<<t.IsBalance();
    // t.InOrder();
}