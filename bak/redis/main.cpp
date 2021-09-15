// 注意编译命令：
// g++ main.cpp headers/* sources/* -o main -L/usr/local/lib/ -lhiredis
// 或者
// g++ main.cpp headers/* sources/* -o main -L/usr/include/ -lhiredis

#include <iostream>
#include <vector>
#include <string>
#include <utility>// std::pair
#include "../../src/utils/redis/redis_tools.h"
#include "hiredis/hiredis.h"

using namespace std;

int main()
{
    RedisTool redis;
    //测试 string
    redis.setString("wangbaojia","test1");
    string result = redis.getString("wangbaojia");
    cout<<"result="<<result<<endl;
    //测试list
    vector<int> vec ={1,2,3,4};
    redis.setList("bao",vec);
    vector<int> vecResult = redis.getList("bao");
    for(int i=0;i<vecResult.size();i++)
    {
        cout<<vecResult[i]<<" ";
    }

    // 测试hash
    // node_info格式：array{pair<指标,指标值>,pair<指标,指标值>,pair<指标,指标值>}
    vector<pair<string,double>> node_info;
    node_info = {
            make_pair("node_id",2), make_pair("node_load",0.88),make_pair("cpu",0.69), make_pair("mem",0.88),
            make_pair("disk",0.56), make_pair("net",0.88), make_pair("job_num",1)
    };
//    cout << "node_id_info:\n" << node_info[0].first << ": " << node_info[0].second << "\n" <<
//         node_info[1].first << ": " << node_info[1].second << endl;
    // 遍历输出
    for (int i = 0; i < node_info.size(); i++)
        cout << node_info[i].first << ": " << node_info[i].second << " ";

    vector<pair<string, double>> node_info2 = {
            make_pair("node_id",1), make_pair("node_load",0.50),make_pair("cpu",0.69), make_pair("mem",0.88),
            make_pair("disk",0.56), make_pair("net",0.88), make_pair("job_num",0)
    };

    vector<pair<string, double>> node_info3 = {
            make_pair("node_id",3), make_pair("node_load",0.95),make_pair("cpu",0.69), make_pair("mem",0.88),
            make_pair("disk",0.56), make_pair("net",0.88), make_pair("job_num",2)
    };

    vector<pair<string, double>> node_info4 = {
            make_pair("node_id",4), make_pair("node_load",0.44),make_pair("cpu",0.69), make_pair("mem",0.88),
            make_pair("disk",0.56), make_pair("net",0.88), make_pair("job_num",0)
    };

    vector<pair<string, double>> node_info5 = {
            make_pair("node_id",5), make_pair("node_load",0.80),make_pair("cpu",0.69), make_pair("mem",0.88),
            make_pair("disk",0.56), make_pair("net",0.88), make_pair("job_num",1)
    };

    vector<pair<string, double>> node_info6 = {
            make_pair("node_id",6), make_pair("node_load",0.96),make_pair("cpu",0.69), make_pair("mem",0.88),
            make_pair("disk",0.56), make_pair("net",0.88), make_pair("job_num",2)
    };
//    string field1 = "load", field2 = "job_num";
    redis.setHash(node_info);
    redis.setHash(node_info2);
    redis.setHash(node_info3);
    redis.setHash(node_info4);
    redis.setHash(node_info5);
    redis.setHash(node_info6);

//    redis.setHash("nodes:5",field1,"0.01",field2,"0");
//
    pair<int,pair<int, double>> hashResult = redis.getHash("node_2_info");

    //    printf("hashResult.begin()->first: %d\n",hashResult.begin()->first);
    //    printf("hashResult.size: %d\n",hashResult.size());
//    printf("hashResult[0]: %s\n",hashResult[0].c_str());
//    for(int i=0;i<hashResult.size();i++)
//    {
//        cout<< hashResult[i] <<" ";
//    }

//    // 参数形式:std::vector<std::pair>
//    // 存入redis--存储格式:std::vector<std::pair>
//    // 一个数组代表一个节点
//    vector<pair<string,double>> node_id_info;
//    node_info = {make_pair("node_id",1), make_pair("node_load",0.36),make_pair("cpu",0.69), make_pair("mem",0.88),
//                    make_pair("disk",0.56), make_pair("net",0.88)};
//    cout << "node_id_info:\n" << node_info[0].first << ": " << node_info[0].second << "\n" <<
//         node_info[1].first << ": " << node_info[1].second << endl;
//    // 遍历输出
//    for (int i = 0; i < node_info.size(); i++)
//        cout << node_info[i].first << ": " << node_info[i].second << " ";


    return 0;
}