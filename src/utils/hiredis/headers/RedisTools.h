#ifndef _H_REDIS_TOOLS_
#define _H_REDIS_TOOLS_
#include <iostream>
#include <vector>
#include "hiredis/hiredis.h"
#include "RedisConfig.h"
#include <unordered_map>

using namespace std;

class RedisTool
{
public:
    RedisTool();
    ~RedisTool();
    int setString(string key, string value);
    string getString(string key);

    int setList(string key,vector<int> value);
    vector<int> getList(string key);

    int setHash(std::vector<std::pair<string, double>> node_info);
    unordered_map<int,unordered_map<double, int>> getHash(string key);

private:
    void init();
    redisContext *m_redis;
    RedisConfig m_config;
};

#endif