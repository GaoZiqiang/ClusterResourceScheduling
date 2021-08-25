// 注意编译命令：
// g++ main.cpp Headers/* Sources/* -o main -L/usr/local/lib/ -lhiredis
// 或者
// g++ main.cpp Headers/* Sources/* -o main -L/usr/include/ -lhiredis

#include <iostream>
#include <vector>
#include "Headers/RedisTools.h"
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

    return 0;
}