// 注意编译命令：
// g++ main.cpp headers/* sources/* -o main -L/usr/local/lib/ -lhiredis
// 或者
// g++ main.cpp headers/* sources/* -o main -L/usr/include/ -lhiredis

#include <iostream>
#include <vector>
#include "headers/RedisTools.h"
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
    string field1 = "load", field2 = "job_num";
    redis.setHash("nodes:4",field1,"0.77",field2,"2");
    redis.setHash("nodes:5",field1,"0.01",field2,"0");

    unordered_map<int,unordered_map<double, int>> hashResult = redis.getHash("nodes:5");
    printf("hashResult.begin()->first: %d\n",hashResult.begin()->first);
    //    printf("hashResult.size: %d\n",hashResult.size());
//    printf("hashResult[0]: %s\n",hashResult[0].c_str());
//    for(int i=0;i<hashResult.size();i++)
//    {
//        cout<< hashResult[i] <<" ";
//    }

    return 0;
}