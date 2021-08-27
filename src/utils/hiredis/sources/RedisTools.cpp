#include "../headers/RedisTools.h"
//#include "../headers/RedisConfig.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "hiredis/hiredis.h"
#include <unordered_map>

using namespace std;

RedisTool::RedisTool()
{
    m_redis = NULL;
    init();
}
RedisTool::~RedisTool()
{
    if(m_redis != NULL)
    {
        redisFree(m_redis);//析构函数释放资源
        cout << "~RedisTool :: free redis connection " << endl;
    }
}

void RedisTool::init()
{
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds 设置连接等待时间
    char ip[255];
    strcpy(ip, m_config.getRedisIP().c_str());
    cout << "init : ip = " << ip << endl;
    m_redis = redisConnectWithTimeout(ip, m_config.getRedisPort(), timeout);//建立连接
    if (m_redis->err) {
        printf("RedisTool : Connection error: %s\n", m_redis->errstr);
    }
    else
    {
        cout << "init redis tool success " << endl;
        //REDIS_REPLY响应的类型type
        cout << "#define REDIS_REPLY_STRING 1"<< endl;
        cout << "#define REDIS_REPLY_ARRAY 2"<< endl;
        cout << "#define REDIS_REPLY_INTEGER 3"<< endl;
        cout << "#define REDIS_REPLY_NIL 4"<< endl;
        cout << "#define REDIS_REPLY_STATUS 5"<< endl;
        cout << "#define REDIS_REPLY_ERROR 6"<< endl;
    }
}

//向数据库写入string类型数据
int RedisTool::setString(string key, string value)
{
    if(m_redis == NULL || m_redis->err)//int err; /* Error flags, 错误标识，0表示无错误 */
    {
        cout << "Redis init Error !!!" << endl;
        init();
        return -1;
    }
    redisReply *reply;
    reply = (redisReply *)redisCommand(m_redis,     "SET %s %s", key.c_str(), value.c_str());//执行写入命令
    cout<<"set string type = "<<reply->type<<endl;//获取响应的枚举类型
    int result = 0;
    if(reply == NULL)
    {
        redisFree(m_redis);
        m_redis = NULL;
        result = -1;
        cout << "set string fail : reply->str = NULL " << endl;
        //pthread_spin_unlock(&m_redis_flock);
        return -1;
    }
    else if(strcmp(reply->str, "OK") == 0)//根据不同的响应类型进行判断获取成功与否
    {
        result = 1;
    }
    else
    {
        result = -1;
        cout << "set string fail :" << reply->str << endl;
    }
    freeReplyObject(reply);//释放响应信息

    return result;
}

//从数据库读出string类型数据
string RedisTool::getString(string key)
{
    if(m_redis == NULL || m_redis->err)
    {
        cout << "Redis init Error !!!" << endl;
        init();
        return NULL;
    }
    redisReply *reply;
    reply = (redisReply *)redisCommand(m_redis,"GET %s", key.c_str());
    cout<<"get string type = "<<reply->type<<endl;

    if(reply == NULL)
    {
        redisFree(m_redis);
        m_redis = NULL;
        cout << "ERROR getString: reply = NULL!!!!!!!!!!!! maybe redis server is down" << endl;
        return NULL;
    }
    else if(reply->len <= 0)
    {
        freeReplyObject(reply);
        return NULL;
    }
    else
    {
        stringstream ss;
        ss << reply->str;
        freeReplyObject(reply);
        return ss.str();
    }
}

//向数据库写入vector（list）类型数据
int RedisTool::setList(string key,vector<int> value)
{
    if(m_redis == NULL || m_redis->err)
    {
        cout << "Redis init Error !!!" << endl;
        init();
        return -1;
    }

    redisReply *reply;

    int valueSize = value.size();
    int result = 0;

    for(int i=0; i<valueSize; i++)
    {
        reply = (redisReply*)redisCommand(m_redis,"RPUSH %s %d", key.c_str(),value.at(i));
        cout<<"set list type = "<<reply->type<<" ";
        int old = reply->integer;
        if(reply == NULL)
        {
            redisFree(m_redis);
            m_redis = NULL;
            result = -1;
            cout << "set list fail : reply->str = NULL " << endl;
            //pthread_spin_unlock(&m_redis_flock);
            return -1;
        }
        else if(reply->integer == old++)
        {
            result = 1;
            cout<<"rpush list ok"<<endl;
            continue;
        }
        else
        {
            result = -1;
            cout << "set list fail ,reply->integer = " << reply->integer << endl;
            return -1;
        }

    }

    freeReplyObject(reply);
    cout<<"set List  success"<<endl;
    return result;

}

//从数据库读出vector（list）类型数据
vector<int> RedisTool::getList(string key)
{

    if(m_redis == NULL || m_redis->err)
    {
        cout << "Redis init Error !!!" << endl;
        init();
        return vector<int>{};//返回空的向量
    }

    redisReply *reply;
    reply = (redisReply*)redisCommand(m_redis,"LLEN %s", key.c_str());
    int valueSize = reply->integer;
    cout<<"List size is :"<<reply->integer<<endl;

    reply = (redisReply*)redisCommand(m_redis,"LRANGE %s %d %d", key.c_str(),0,valueSize-1);
    cout<<"get list type = "<<reply->type<<endl;
    cout<<"get list size = "<<reply->elements<<endl;//对于数组类型可以用elements元素获取数组长度

    redisReply** replyVector = reply->element;//获取数组指针
    vector<int> result;
    for(int i=0;i<valueSize;i++)
    {
        string temp =(*replyVector)->str;//遍历redisReply*数组,存入vector向量
        int a =atoi(temp.c_str());
        result.push_back(a);
        replyVector++;
    }

    cout<<"result size:"<<result.size()<<endl;
    return result;

}

//向数据库写入hash类型数据
int RedisTool::setHash(std::vector<std::pair<string, double>> node_info)
{
    if(m_redis == NULL || m_redis->err)//int err; /* Error flags, 错误标识，0表示无错误 */
    {
        cout << "Redis init Error !!!" << endl;
        init();
        return -1;
    }


    // 代码太丑了！！！
    // node_id
    string key_node_id = node_info[0].first;// node_id
    double value_node_id = node_info[0].second;// node load

    // resource keys
    string key_load = node_info[1].first, key_cpu = node_info[2].first, key_mem = node_info[3].first,
            key_disk = node_info[4].first, key_net = node_info[5].first;
    // values
    double value_load = node_info[1].second, value_cpu = node_info[2].second, value_mem = node_info[3].second,
            value_disk = node_info[4].second, value_net = node_info[5].second;

    // node_id转int-->string-->拼接成node_id_info
    int node_id_ = int(value_node_id);
    string node_id = "node_" + to_string(node_id_) + "_info";
//    redis.setHash(to_string(node_id),key_load,value_load, key_cpu,value_cpu, key_mem,value_mem,
//                  key_disk,value_disk, key_net,value_net);




    redisReply *reply;
    reply = (redisReply *)redisCommand(m_redis,"HMSET %s %s %s %s %s %s %s %s %s %s %s", node_id.c_str(), key_load.c_str(), to_string(value_load).c_str(),
                                       key_cpu.c_str(), to_string(value_cpu).c_str(),key_mem.c_str(), to_string(value_mem).c_str(),key_disk.c_str(), to_string(value_disk).c_str(),
                                       key_net.c_str(), to_string(value_net).c_str());//执行写入命令
    cout<<"set string type = "<<reply->type<<endl;//获取响应的枚举类型
    int result = 0;
    if(reply == NULL)
    {
        redisFree(m_redis);
        m_redis = NULL;
        result = -1;
        cout << "set string fail : reply->str = NULL " << endl;
        //pthread_spin_unlock(&m_redis_flock);
        return -1;
    }
    else if(strcmp(reply->str, "OK") == 0)//根据不同的响应类型进行判断获取成功与否
    {
        result = 1;
    }
    else
    {
        result = -1;
        cout << "set string fail :" << reply->str << endl;
    }
    freeReplyObject(reply);//释放响应信息

    return result;
}

//从数据库读出hash类型数据
unordered_map<int,unordered_map<double, int>> RedisTool::getHash(string key)
{

    if(m_redis == NULL || m_redis->err)
    {
        cout << "Redis init Error !!!" << endl;
        init();
        return unordered_map<int,unordered_map<double, int>>{};//返回空的向量
    }

    redisReply *reply;
    reply = (redisReply*)redisCommand(m_redis,"HLEN %s", key.c_str());
    int valueSize = reply->integer;
    cout<<"Hash size is :"<<reply->integer<<endl;

    reply = (redisReply*)redisCommand(m_redis,"HGETALL %s", key.c_str());
////    reply = (redisReply*)redisCommand(m_redis,"LRANGE %s %d %d", key.c_str(),0,valueSize-1);
    cout<<"get hash type = "<<reply->type<<endl;
    cout<<"get hash size = "<<reply->elements<<endl;//对于数组类型可以用elements元素获取数组长度
    int ele_num = reply->elements;
//
    redisReply** replyVector = reply->element;//获取数组指针
    vector<string> result;

    for(int i=0;i<ele_num;i++)
    {
        string temp =(*replyVector)->str;//遍历redisReply*数组,存入vector向量
        //int a =atoi(temp.c_str());
        //string a = temp.c_str();
        result.push_back(temp);
        cout << "temp:" << temp << endl;
        replyVector++;
    }

    printf("printf--reslut[0]: %s\n",result[0].c_str());
    cout << "result[0]: " << result[0] << endl;

    // result封装成一个map--unorderedmap
    // node_info--{node_id,{load,job_num}}
    unordered_map<int,unordered_map<double, int>> node_info;
    int node_id = 5;
    node_info = {{node_id,{{atof(result[1].c_str()),atoi(result[3].c_str())}}}};


//
//    cout<<"result size:"<<result.size()<<endl;
    //vector<string> result = {"hello"};
    return node_info;

}
