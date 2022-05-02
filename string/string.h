#pragma once
#include <string.h>
#include <iostream>
#include <cassert>
#include <cstdio>
using namespace std;
namespace xzw
{
    class string
    {
    private:
        char *_str;
        size_t _size;
        size_t _capacity;              //能存有效字符的空间个数，不算上\0
        static const size_t npos = -1; //静态成员变量属于整个类域
    public:
        // string()//构造一个空字符串
        //     :_str(new char[1])
        //     ,_size(0)
        //     ,_capacity(0)//存0个有效，
        // {
        //     _str[0]='\0';
        // }
        //string::npos=-1;//整形的最大值
        typedef char *iterator;             //迭代器，指针类型，内嵌类型，就是在类里面定义的类型,普通迭代器
        typedef const char *const_iterator; //只可以读，不可以写

        const_iterator begin() const //解引用之后那个内容是不可以修改的，const对象
        {
            return _str;
        }
        const_iterator end() const
        {
            return _str + _size;
        }
        iterator begin()
        {
            return _str; //返回第一个位置的地址
        }
        iterator end()
        {
            return _str + _size; //最后一个地址的下一个位置
        }

        string(const char *str = "") //构造函数，我们可以给一个全缺省
            //: _str(str)//不能这样写，因为我们要能够动态的扩容
            : _size(strlen(str)), _capacity(_size)
        {
            _str = new char[_capacity + 1]; //增容就是相等的时候就扩容,+1是为了使字符串保护\0
            strcpy(_str, str);
        }
        const char *c_str()
        {
            return _str;
        }
        //传统写法--本本分分的去开空间拷贝数据
        // string(const string &s) //传引用拷贝
        //     : _size(s._size), _capacity(s._capacity)
        // {
        //     _str = new char[_capacity + 1];
        //     strcpy(_str, s._str);
        // }
        // //assign，赋值运算符
        // string &operator=(const string &s) //我们也要自己写，不然就是浅拷贝，还要考虑自己给自己赋值
        // {
        //     //如果自己给自己赋值，要拷贝的东西被我们毁掉，就没办法操作了
        //     // if (this!=&s)
        //     // {
        //     //     delete[] _str; //直接简单粗暴的把原来的空间给释放调，,我们不能把s给释放调，说不定以后还会用到s的数据，有可能new失败
        //     //     //先把原来的空间给释放调，再重新开辟一块空间
        //     //     _str = new char[strlen(s._str) + 1];//开一个和s一样大的空间，
        //     //     strcpy(_str, s._str);
        //     // }
        //     if (this != &s)
        //     {
        //         char *tmp = new char[s._capacity+1];
        //         strcpy(tmp, s._str); //先拷贝再释放掉
        //         delete[] _str;       //

        //         _str = tmp;
        //         _size = s._size;
        //         _capacity = s._capacity;
        //     }
        //     return *this; //出了作用于*this还在，所以传引用返回
        // }

        //现代写法---投机取巧的方式去实现深拷贝
        string(const string &s)
            : _str(nullptr) //tmp交换后指向空，delete 空是不会报错的
              ,
              _size(0), _capacity(0)
        {
            string tmp(s._str); //是一种代码复用行为
            swap(_str, tmp._str);
            swap(_size, tmp._size);
            swap(_capacity, tmp._capacity);
        }

        // string operator=(const string& s)
        // {
        //     if(this!=&s)
        //     {
        //         string tmp(s);//调用一个拷贝构造
        //         swap(_str,tmp._str);//出了作用域，tmp现在是s的地址，出了作用域还帮助s把空间给释放掉

        //     }
        //     return *this;
        // }
        // void Swap(string &s)
        // {
        //     swap(_str, s._str); //所有类的赋值都可以这样做
        //     string tmp(s._str); //是一种代码复用行为
        //     swap(_size, s._size);
        //     swap(_capacity, s._capacity);
        // }

        //提倡
        string operator=(string s) //使用拷贝构造，没有用引用传参,s就是原来的拷贝
        {
            swap(_str, s._str); //所有类的赋值都可以这样做
            //string tmp(s._str); //是一种代码复用行为
            swap(_size, s._size);
            swap(_capacity, s._capacity);
            return *this;
        }
        string operator=(const char*s)
        
        {
            _str=nullptr;
            string tmp(s);
            swap(tmp._str,_str);
            swap(_size,tmp._size);
            swap(_capacity,tmp._capacity);
            return *this;
        }

        ~string() //析构函数
        {
            delete[] _str; //new完之后就要delete
            _str = nullptr;
            _size = _capacity = 0;
        }

        size_t size() const //遍历string,const在后面，则this的东西都不能够修改
        {
            return _size;
        }
        char &operator[](size_t pos) //重载，返回其引用
        {
            assert(pos < _size);
            return _str[pos]; //出了作用域，还存在,并且我们能够对其进行修改，this的东西发生了修改
        }
        const char &operator[](size_t pos) const //重载,const变量，不可以改变
        {
            //判断是否出了范围
            assert(pos < _size);
            return _str[pos]; //const修饰都不能改变
        }

        void reserve(size_t n) //对容量进行一个改变,给一个小于capacity的数值，不会对容量有影响
        {
            //n代表实际的空间
            if (n > _capacity)
            {
                //空间不够
                char *tmp = new char[n + 1]; //容量到n要对\0多开一个
                strcpy(tmp, _str);
                delete[] _str; //把原来的空间销毁掉
                _str = tmp;
                _capacity = n;
            }
        }

        void push_back(char ch) //尾插
        {
            if (_size == _capacity) //满了就要增容
            {
                //增容
                reserve(_capacity == 0 ? 4 : _capacity * 2);
            }
            _str[_size] = ch;
            _size++;
            _str[_size] = '\0'; //
        }
        void resize(size_t n, char ch = '\0') //改变容量的同时对未有的东西进行初始化，已经有东西就不用初始化
        {
            //如果n小于size的话,容量不改变，,但是字符串也要减小
            if (n < _size) //只保留前n个字符
            {
                _str[n] = '\0'; //在n的位置设置为\0
                _size = n;      //同时将
                //在
            }
            else if (_size < n && _capacity > n) //size小,容量大，就不用扩容
            {
                int rest = n - _capacity;

                while (rest--)
                {
                    push_back(ch);
                }
                //memset(_str+_size,ch,n-_size);//这样也可以
            }
            else if (_size < n && _capacity < n) //size小，容量小，也需要扩容
            {
                int rest = n - _capacity;
                reserve(n);
                while (rest--)
                {
                    push_back(ch);
                }
            }
            //reserve(n);
            //dui
        }

        void append(const char *str) //不能直接常数个的去扩容，扩容可能还是不太够用
        {
            //计算实际的空间
            size_t len = strlen(str);

            if (_size + len > _capacity) //天然考虑一个\0
            {
                //就要扩容
                reserve(_size + len);
            }
            strcpy(_str + _size, str); //从原来\0位置开始往后拷贝，\0也会自动拷贝进去
            _size += len;
        }
        string &operator+=(const char *str)
        {
            //this->append(str);
            append(str);
            return *this;
        }
        string &operator+=(const char ch)
        {
            this->push_back(ch);
            return *this;
        }
        size_t find(char ch) //返回第一次出现时候的位置，不存在就返回一个npos，
        {
            for (size_t i = 0; i < _size; i++)
            {
                if (_str[i] == ch)
                {
                    return i;
                }
            }
            return npos;
        }
        size_t find(const char *s, size_t pos = 0) //查找一个字符串,从pos位置开始找
        {
            char *ptr = strstr(_str + pos, s); //找不到就返回一个nillptr，找到了就返回第一次在s里面出现的位置的指针
            if (ptr == nullptr)
            {
                return npos;
            }
            else
                return ptr - _str; //和第一个位置相减就可以得到他的下标
        }
        string &insert(size_t pos, char ch)
        {
            assert(pos <= _size); //必须三小于size
            if (_size == _capacity)
            {
                reserve(_capacity == 0 ? 4 : _capacity * 2);
            }
            //从后往前挪动
            if (pos == 0) //头插
            {
                //用指针

                string s;
                s += ch;
                s += _str;
                swap(s._str, _str); //交换之后s直接去调用他的稀构函数，就会把s原来的空间给释放掉
                _size++;
                return *this;
            }
            else
            {
                size_t end = _size + 1; //为了解决那个问题，我们可以让end=size+1
                while (end > pos)       //end会越界，一个超大的数值，但是因为pos还是一个无符号数的比较，只要有一方是无符号数，就会按照无符号数进行比较,通常往范围大的提升
                {
                    _str[end] = _str[end - 1];
                    end--;
                }
                //插入
                _str[pos] = ch;
                _size++;
                _str[_size] = '\0';
                return *this;
            }
        }

        string &insert(size_t pos, const char *s)

        {
            size_t len = strlen(s);
            if (len + _size > _capacity)
            {
                reserve(len + _capacity);
            }
            //数据全部都往后挪动
            size_t end = _size + len; //为了解决那个问题，我们可以让end=size+1
            while (end >= pos+len)         //end会越界，一个超大的数值，但是因为pos还是一个无符号数的比较，只要有一方是无符号数，就会按照无符号数进行比较,通常往范围大的提升
            {
                //>pos会出现越界的情况
                _str[end] = _str[end - len];
                end--;
            }
            //插入
            strncpy(_str+pos,s,len);//拷贝len个字符过去,不能带\0,从_str+pos的位置从s里面拷贝len个字符
            _size+=len;
            return *this;
        }

        string& erase(size_t pos,size_t len=npos)
        {
            assert(pos<_size);
            size_t rest=_size-pos;
            if(len==npos||len>=rest)
            {
                //有多少删除多少
                _str[pos]='\0';
                _size=pos;
            }
            else
            {
                strncpy(_str+pos,_str+pos+len,_size-pos-len);
                _str[pos+len]='\0';
                _size=pos+len;
            }
            return *this;
        }       
    };
    ostream& operator<<(ostream&out,const string& s)//带返回值，为了连续的访问
    {   
        //ostream//可以支持输出库的内置类型，可以也可以自动识别类型
        for(auto ch : s)//没有\0
        {
            out<<ch;//把每个字符输出出来，
        }
        return out;
        //out<<s.c_str();不能这样输出，这个是有\0
        

    }
    istream& operator>>(istream&in,string &s)
    {
        //获取每个字符
        char ch=in.get();
        while(ch !=' '||ch!='\n')
        {
            s+=ch;
            ch=in.get();
        }
        return in;
    }
    void test_string1()
    {
        string s("hello ");
        //string s2(s);//拷贝构造,对默认类型会完成数值拷贝，浅拷贝，指向同一块空间，出了作用域，调用释放函数，自定义类型，调用他的拷贝构造
        //我们不能在同一块空间
        string s2(s);
        string s3("hello ");
        s = s3; //如果我们不写就是浅拷贝，连着地址一起拷贝过去了，s内存还没有释放，内存泄漏
        //我们不能直接去拷贝，因为可能会出现空间不够，导致还要去进一步的扩容，小拷贝到大，会导致空间用不完，
        s = "alcd";
        printf("sdahello,world a\n");
    }
    void test_string2()
    {
        string s("hello world");

        string s1;                  //不带参数
        cout << s.c_str() << endl;  //c形式的输出，
        cout << s1.c_str() << endl; //遇到\0就结束,\0是不可见的字符
        s[0] = 'd';
        s[2] = 'g';
        for (size_t i = 0; i < s.size(); i++)
        {
            cout << s[i] << " ";
        }

        cout << endl;
    }
    void test_string3()
    {
        string s("hello world");
        string::iterator it = s.begin();
        while (it != s.end())
        {
            *it += 2;
            it++;
        }
        it = s.begin();
        while (it != s.end())
        {
            cout << *it << " ";
            it++;
        }
        cout << endl;
        for (auto e : s) //编译之后就被自动替换成迭代器
        {
            cout << e << " ";
        }
        cout << endl;
    }
    void test_string4()
    {
        string s("hello world");
        s.push_back(' ');
        s.push_back('@');
        s.push_back('j');
        s.append("hellllo worlldddd");
        string s1;
        s1 += 'f';
        s1 += "hfih";
        cout << s1.c_str() << endl;
    }
    void test_string5()
    {
        string s("abcdefg");
        s.resize(8, 'b');
        size_t pos = s.find("cd");
        cout << pos << endl;
        s.insert(0, 'l');
        s.insert(0,"abdh");//尽量少用insert，0（n）
        cout << s.c_str() << endl;
        s.resize(2);
        cout << s.c_str() << endl;
    }
    void test_string6()
    {
        string s="hello";
        s.insert(0,"ab");
        //s.erase(1,2);//把2位置后面的东西全部都删除掉
        cout<<s.c_str()<<endl;
        string s2;
        cin>>s2;
        cout<<s2<<endl;
    }
}

//浅拷贝的问题：
//1.会稀构两次
//2.其中一个对象进行修改会影响另一个，
//3.

//引用计数的写时拷贝
//当有两个对象指向同一块空间的时候，计数为2,如果有一个对象要西沟，发现不是1,就减引用计数，为1的时候，再去西沟
//不写的时候，就都不去改变，如果写的话才要去做深拷贝，延迟拷贝

//inser erase 的先查看引用计数，不是1,才要去做生拷贝，再去修改
//如果不写就只是增加引用计数，不进行深拷贝，提高效率

//缺陷：引用计数存在线程安全问题，在多线程环境下要付出代价，在动态库，静态库中有些场景会存在问题