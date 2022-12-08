#include <iostream>
using namespace std;
#include <bitset>
#include <string>
#include <vector>

//字符串进行映射的话，会出现冲突，把字符串映射到某个int上，
//判断一个字符串，在的话可能会出现冲突,可能会出现误判
//如果不在的话，肯定是准确的
//我们既然解决不了问题，那么就减低出现问题的可能

//一个值进行映射多个位置，这样多个位置都出现冲突，才算出现冲突，这就不好出现误判
//误判率就被降低了

//这里我们给了3个hash函数,进行映射
struct BKDRHash //特化，全特化,这个是最强的
{
    size_t operator()(const string &key)
    {
        size_t ret = 0;
        for (auto e : key)
        {
            ret *= 31;
            ret += e;
        }
        return ret;
    }
};

struct APHash
{
    size_t operator()(const string &s)
    {
        size_t hash = 0;
        size_t ch;
        for (long i = 0; i < s.size(); i++)
        {
            if ((i & 1) == 0)
            {
                hash ^= ((hash << 7)) ^ s[i] ^ (hash >> 3);
            }
            else
            {
                hash ^= (~(hash << 11)) ^ s[i] ^ (hash >> 5);
            }
        }
        return hash;
    }
};

struct DJBHash
{
    size_t operator()(const string &s)
    {
        size_t hash = 5381;
        for (auto ch : s)
        {
            hash += (hash << 5) + ch;
        }
        return hash;
    }
};

template <size_t N, class K = string, class HashFunc1 = BKDRHash, class HashFunc2 = APHash, class HashFunc3 = DJBHash> //给个默认类型,一个值我们让他映射3个位置
class BloomFilter
{
private:
    bitset<N * 4> _bs; //给个模板参数，要开多少bit位
public:
    void set(const K &key)
    {
        //标记成存在
        HashFunc1 h1;
        HashFunc2 h2;
        HashFunc3 h3;

        size_t len = 4 * N;
        size_t index1 = h1(key) % len;
        size_t index2 = h2(key) % len;
        size_t index3 = h3(key) % len;
        cout << index1 << endl;
        cout << index2 << endl;
        cout << index3 << endl;
        _bs.set(index1);
        _bs.set(index2);
        _bs.set(index3);
    }

    bool test(const K &key)
    {
        //因为我们映射到了3个位置，所以一个在不能说明它就在，但是如果一个不在，他就不在
        HashFunc1 h1;
        HashFunc2 h2;
        HashFunc3 h3;

        size_t len = 4 * N;
        size_t index1 = h1(key) % len;
        size_t index2 = h2(key) % len;
        size_t index3 = h3(key) % len;
        // cout << index1 << endl;
        // cout << index2 << endl;
        // cout << index3 << endl;
        if (_bs.test(index1) == false)
            return false;
        if (_bs.test(index2) == false)
            return false;
        if (_bs.test(index3) == false)
            return false;

        //前面只要有一个位不在，就是不在
        //走到这里，就是三个位都存在，但是也可能会出现误判
        return true;
    }
    void reset(const K& key)
    {
        //把对应的位删除,删除可能会把冲突的删掉了,所以不支持删除
        //可能会影响其他值
        //那么如何扩展一下，使得布隆过滤器能够支持删除
        //每个位置存储多个bit位，存储引用计数，（有多少个值映射到了当前的位置）
        //支持删除，整体消耗空间变多了，但是效率也就下降了
        
        
    }
};

int main()
{
    //如何选择哈希函数和布隆过滤器长度（要开多少bit位）
    BloomFilter<100> bm; //这里开多少个值呢，我们要斟酌一下
    bm.set("sort");
    bm.set("right");
    bm.set("eat");
    // cout<<bm.test("sort")<<endl;
    // cout<<bm.test("ort")<<endl;
    // cout<<bm.test("aet")<<endl;

    BloomFilter<100> bf;
    size_t N = 100;
    vector<string> v2;
    for (int i = 0; i < N; i++)
    {
        string url = "asdsdsfsadasdfgwgrwwefqfrgfbfgb";
        url += to_string(1234 + i);
        v2.push_back(url);
    }
    size_t n2 = 0;
    for (auto &str : v2) //把里面的数据都设置进去
    {
        bf.set(str);
    }
    for (auto &str : v2)
    {
        cout << bf.test(str) << endl;
    }
    vector<string> vv;
    for (int i = 0; i < N; i++)
    {
        string url = "asdsdsfsadasdfgwgrwwefqfrgfbfgb";
        url += to_string(6789 + i);
        vv.push_back(url);
    }
    int nn = 0;
    for (auto &str : vv)
    {
        if (bf.test(str))
        {
            nn++;
        }
    }
    cout<<nn<<endl;//发现误判率是0.07，出现七个数误判
    

    return 0;
}

//布隆过滤器减少误判的最佳操作就是增大比率，开的远大于实际开的空间大小，因为我们一个数，映射到多个位置
//类比红黑树的负载因子，
//节省空间，一个值映射的增多，空间消耗就更多了，


//布隆过滤器的使用场景
/*
    需求：数据量大，节省空间，允许误判，这样的场景，就可以使用布隆过滤器
    注册的时候使用数据库：1.昵称（判断昵称有没有人用过）2.电话号码（同样的道理）快速判断
    就可以将数据库里的昵称，放到布隆过滤器里面，我们只要去布隆过滤器里面搜索即可,不在是准确的
    ,在是有误判的,所以如果在的话，可以再去数据库确认一遍，不在就不用去数据库了
    

    垃圾邮件：如果是垃圾邮件，就可以把它放到一个垃圾邮箱里面，就标记一个黑名单，在布隆，
    接收邮件，判断在不在黑名单，在就不收，不在就收

    

    利用布隆过滤器减少磁盘IO，或者网络请求,访问本地的数据库比较慢，
    先查在不在，在的话，就继续去前端查一下，
    如果不在的话，就直接返回，可以把不存在的给过滤掉(不存在是准确的)


*/

/*
    1.给两个文件，分别由100亿个query，我们只有1G内存，如何找到两个文件的交集？分别用精确和近似的算法
    :这100亿查询放进布隆过滤器，在就是交集，不在就不是交集,这个时候是近似的交集(会存在不是交集的进去)
    :
    哈希切分就可以实现精确查询
    A           B
    假设A有100G，那么切成多份，但是我们需要使用哈希切，而不是平均切
    读取query，使用hash算法，如i=BKDRhash（query）%200;我们要创建是200个小文件
    这个query就进入ai好的小文件

    同样B也是一样的

    这样相同的就进入同一个小文件
    Ai和Bi小文件找交集即可:因为相同的query一定进入了编号相同的小文件
    这样就加载到内存，放进set相同就有，不同就没

    有可能有的桶很大，有的桶很小，找交集不好找
    如果Ai和Bi都太大，超过内存，可以考虑换个哈希算法，再切分一次，


    一个超过100G的log file log中存这IP地址，涉及算法找到出现次数最多的IP(统计次数)，如何找到TOPK的IP，如何直接使用Linux系统命令
     一次读取ip,i=BKDRHash(ip)%200;//分成100份
     i是多少，ip就进入对应的编号的i小文件里面
     相同的ip一定进入了同一个小文件，那么我们使用map统计一个小文件中的ip的次数，就是它准确的次数
     pair<string,int> maxCountIP

     出现次数最多的10个ip
     priority_queue<pair<string,int>,vector<pair<string,int>>,greater> minhead;小堆
     
     
*/