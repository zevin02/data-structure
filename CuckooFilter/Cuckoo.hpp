#include <iostream>
#include <vector>
using namespace std;
#include <functional>
#include <string>
//定义哈希函数类型
using Hash = std::function<uint64_t(const string)>;

// 简单的FNV哈希函数，n就是插入数据的字节数
uint64_t FnvHash(const string data)
{
    const uint64_t FNV_offset_basis = 14695981039346656037ULL;
    const uint64_t FNV_prime = 1099511628211ULL;
    uint64_t hash = FNV_offset_basis;
    size_t n = data.size();
    for (size_t i = 0; i < n; i++)
    {
        hash = hash ^ data[i];
        hash = hash * FNV_prime;
    }
    return hash;
}

class CuckooFilter
{
private:
    vector<vector<uint32_t>> table_; //每个bucket中可以最多存储4个元素
    uint32_t max_entry_;             //存储的元素个数
    uint32_t kMaxNumKicks;           //如果操作500次还没插入成功，就判定插入失败
    uint32_t num_bucket_;            // bucket个数
    Hash finger_hash_ = FnvHash;     //计算指纹的哈希函数
    uint8_t fingerprint_size_;       //存放的指纹位大小
    uint8_t num_entry_per_bucket_;   //每个bucket中存放的元素个数
public:
    CuckooFilter(uint32_t num_bucket, uint32_t finger_size, uint8_t num_entry_per_bucket)
        : num_bucket_(num_bucket), fingerprint_size_(finger_size), num_entry_per_bucket_(num_entry_per_bucket), kMaxNumKicks(500), max_entry_(0)
    {
        table_.resize(num_bucket_, vector<uint32_t>(num_entry_per_bucket_, 0)); //每个桶进行初始化
    }
    uint32_t size()
    {
        return max_entry_;
    }
    bool Insert(const string &data)
    {
        //获得指纹,获得低finger位置
        uint32_t fp = finger_hash_(data) & ((1 << fingerprint_size_) - 1); //这个是对应的指纹位最大也才10位，所以不会超出限制
        uint32_t b1, b2;                                                   //每个元素对应两个索引位置
        generateIndexHash(data, fp, b1, b2);
        if (InsertImpl(fp, b1))
            return true;
        if (InsertImpl(fp, b2))
            return true;
        //两个都满了(2个bucket的8个位置都满了)，所以需要进行一个迭代kick
        for (uint32_t i = 0; i < kMaxNumKicks; i++)
        {
            //选择其中一个桶的元素，并让该元素离开
            uint32_t b = (rand() % 2 == 0) ? b1 : b2;
            uint32_t r = rand() % num_entry_per_bucket_; //获得要替代的元素
            uint32_t tmp_fp = table_[b][r];
            table_[b][r] = fp; //插入
            fp = tmp_fp;       //将该kick出去的fp要获得他其他的索引位
            //根据该fingerprint获得其对应的两个索引位置
            //还是b1和b2两个位置
            if (b == b1)
            {
                //因为在转化的时候，要替换的元素只有一个位置不一样
                if (b2 = (finger_hash_(to_string(fp)) ^ b1) % num_bucket_; InsertImpl(fp, b2))
                    return true;
            }
            if (b == b2)
            {
                if (b1 = (finger_hash_(to_string(fp)) ^ b2) % num_bucket_; InsertImpl(fp, b1))
                    return true;
            }
        }
        return false;
    }
    bool Delete(const string &data) //删除一个元素
    {
        uint32_t fp = finger_hash_(data) & ((1 << fingerprint_size_) - 1); //这个是对应的指纹位最大也才10位，所以不会超出限制

        //获得他的索引位置
        uint32_t b1, b2;
        generateIndexHash(data, fp, b1, b2);
        //因为一个元素只会出现一个bucket中，所以找到一个就算可以了
        if (DeleteImpl(fp, b1) || DeleteImpl(fp, b2))
        {
            return true;
        }

        return false;
    }
    bool lookUp(const string &data) //查找一个元素
    {
        uint32_t fp = finger_hash_(data) & ((1 << fingerprint_size_) - 1); //这个是对应的指纹位最大也才10位，所以不会超出限制

        uint32_t b1, b2;
        generateIndexHash(data, fp, b1, b2);
        if (lookUpImpl(fp, b1) || lookUpImpl(fp, b2))
        {
            return true;
        }
        return false;
    }

private:
    bool DeleteImpl(uint32_t fp, uint32_t index) //实现删除的逻辑
    {
        vector<uint32_t> &bucket = table_[index];
        for (uint8_t i = 0; i < num_entry_per_bucket_; i++)
        {
            if (bucket[i] == fp)
            {
                //找到了
                max_entry_; //总个数减少
                bucket[i] = 0;
                return true;
            }
        }
        return false;
    }

    bool lookUpImpl(uint32_t fp, uint32_t index)
    {
        vector<uint32_t> &bucket = table_[index];
        for (uint8_t i = 0; i < num_entry_per_bucket_; i++)
        {
            if (bucket[i] == fp)
            {
                return true;
            }
        }
        return false;
    }
    void generateIndexHash(const string &data, uint32_t fp, uint32_t &b1, uint32_t &b2) //根据该finger生成获得对应的存储位置
    {
        b1 = finger_hash_(data) % num_bucket_; //生成两个索引位
        b2 = (b1 ^ finger_hash_(to_string(fp))) % num_bucket_;
    }
    // fp是要插入的指纹，bucket_index
    bool InsertImpl(uint32_t fp, uint32_t bucket_index)
    {
        vector<uint32_t> &bucket = table_[bucket_index]; //获得对应的桶
        //因为每个桶存4个元素
        for (uint8_t i = 0; i < num_entry_per_bucket_; i++)
        {
            if (!bucket[i])
            {
                max_entry_++; //存储的元素++
                bucket[i] = fp;
                return true;
            }
        }

        return false;
    }
};