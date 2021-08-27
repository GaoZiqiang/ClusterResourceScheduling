// 注意编译命令：
// g++ main.cpp headers/* sources/* -o main -L/usr/local/lib/ -lhiredis
// 或者
// g++ main.cpp headers/* sources/* -o main -L/usr/include/ -lhiredis

#include <iostream>
#include <vector>
#include <string>
#include <utility>// std::pair
#include "headers/RedisTools.h"
#include "hiredis/hiredis.h"

using namespace std;


//void storeIntoRedis(RedisTool &redis, std::vector<std::pair<string, double>> node_info) {
//    // 代码太丑了！！！
//    // node_id
//    string key_node_id = node_info[0].first;// node_id
//    double value_node_id = node_info[0].second;// node load
//
//    // resource keys
//    string key_load = node_info[1].first, key_cpu = node_info[2].first, key_mem = node_info[3].first,
//            key_disk = node_info[4].first, key_net = node_info[5].first;
//    // values
//    double value_load = node_info[1].second, value_cpu = node_info[2].second, value_mem = node_info[3].second,
//            value_disk = node_info[4].second, value_net = node_info[5].second;
//
//    // node_id转int-->string
//    int node_id = int(value_node_id);
//    redis.setHash(to_string(node_id),key_load,value_load, key_cpu,value_cpu, key_mem,value_mem,
//                  key_disk,value_disk, key_net,value_net);
//}

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
    vector<pair<string,double>> node_info;
    node_info = {make_pair("node_id",2), make_pair("node_load",0.88),make_pair("cpu",0.69), make_pair("mem",0.88),
                    make_pair("disk",0.56), make_pair("net",0.88)};
    cout << "node_id_info:\n" << node_info[0].first << ": " << node_info[0].second << "\n" <<
         node_info[1].first << ": " << node_info[1].second << endl;
    // 遍历输出
    for (int i = 0; i < node_info.size(); i++)
        cout << node_info[i].first << ": " << node_info[i].second << " ";


//    string field1 = "load", field2 = "job_num";
    redis.setHash(node_info);

//    redis.setHash("nodes:5",field1,"0.01",field2,"0");
//
//    unordered_map<int,unordered_map<double, int>> hashResult = redis.getHash("nodes:5");
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