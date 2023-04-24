#include <iostream>
#include <vector>
#include <bitset>
using namespace std;
#include <zlib.h>
#include <string>
#include <xxhash.h>
// #include <MurmurHash3.h>
//在实际使用中，需要对每个哈希值进行分组，根据不同的压缩算法和参数进行压缩，将布隆过滤器压缩成字节数组,

//在查询的时候，也需要进行解压缩，还原哈希值位比特数组

/*
    compressbloomfilter filter;
  1.往布隆过滤器中添加元素
    filter.add()
  2. 在需要的时候将其压缩成字节数组
    filter.compress()

    将压缩后的元数据(压缩之后的大小和压缩之前的大小)存放在文件或网络中，以便之后还原
  3. 当需要查询数据是否在布隆过滤器中的时，将压缩后的数据和元数据还原为布隆过滤器的比特数组
    vector<bool> bits=filter.decompress()

  4. 使用还原后的布隆过滤器来查询
  


*/

class CompressBloomFilter
{
public:
    bitset<1024> bloomfilter_; //原始的布隆过滤器，用于数据的查询
    //压缩后的布隆过滤器及其相关的元数据
private:
    vector<char> compressData_;
    uLongf compressed_size_ = 0;   //被压缩之后的数据大小，uLongf,无符号浮点数
    uLongf uncompressed_size_ = 0; //还没被压缩之前的数据大小
    size_t count_=0;
public:
    //往布隆过滤其中添加数据
    void add(const string &element)
    {
        hash<string> h1;
        bloomfilter_.set(h1(element) % 1024);                                //设置进位图中
        //xxhash具有良好的分布型和低碰撞性
        unsigned long long hash = XXH32(element.c_str(), element.size(), 0); //这个是通过xxhash计算一个32位的hash值
        bloomfilter_.set(hash % 1024);
        count_++;
    }

    //压缩
    void compress()
    {
        //如插入了{1,3,5}对应转化之后的就是101010
        string bitset_str = bloomfilter_.to_string(); //把位图变成一个字符串的形式(二进制的形式)
        //压缩位图中的数据
        uLongf buffersize = bitset_str.size();
        vector<char> buffer(buffersize, 0); //开buffersize个空间，每个空间上都是

        compress2((Bytef *)&buffer[0], &buffersize, (const Bytef *)bitset_str.c_str(), bitset_str.size(), Z_BEST_COMPRESSION);
        //记录压缩后的数据以及元数据，
        compressData_ = buffer;                 //现在bufer中的都是已经被压缩过的数据
        compressed_size_ = buffersize;          //压缩的数据大小
        uncompressed_size_ = bitset_str.size(); //还没被压缩之前的数据
    }
    //将压缩之后的bloom filter解压并还原成为比特数据
    bitset<1024> decompress()
    {
        //解压缩压缩后的数据
        uLongf buffersize = uncompressed_size_;
        vector<char> buffer(buffersize, 0);//把解压缩的数据放到这个buffer中
        //解压缩
        uncompress((Bytef *)&buffer[0], &buffersize, (const Bytef *)&compressData_[0], compressed_size_);
        //将解压缩后的数据转化成比特数组
        string uuncompressdata(buffer.begin(), buffer.end());
        bitset<1024> bitset(uuncompressdata);//使用字符串01串来初始化这个位图
        return bitset; //直接返回一个位图
    }

    bool hashElement(const string &element)
    {
        hash<string> h1;
        unsigned long long hash_ = XXH32(element.c_str(), element.size(), 0);
        return bloomfilter_.test(h1(element) % 1024) && bloomfilter_.test(hash_ % 1024);
    }
    
    //计数bloom filter
    size_t size()
    {
        return count_;
    }
};