#include"CompressBloomFilter.hpp"

int main()
{
    CompressBloomFilter f;
    f.add("asd");
    f.add("ddd");
    f.add("dddas");
    f.compress();//添加完进行压缩
    //压缩布隆过滤器处于压缩状态的时候，不能进行插入数据，因为此时布隆过滤器已经被压缩成二进制流了,插入数据后会影响数据的正确性

    f.add("123");
    cout<<f.hashElement("123")<<endl;
    
    //插入数据的时候，必须要保证处于解压缩状态
    f.bloomfilter_=f.decompress();
    cout<<f.hashElement("f")<<endl;
    cout<<f.hashElement("asd")<<endl;
    cout<<f.hashElement("dddas")<<endl;
    cout<<f.hashElement("ddas")<<endl;
    cout<<f.hashElement("dddasdfas")<<endl;
    cout<<f.size()<<endl;
    return 0;
}