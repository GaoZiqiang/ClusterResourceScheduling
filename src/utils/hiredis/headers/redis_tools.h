#ifndef _H_REDIS_TOOLS_
#define _H_REDIS_TOOLS_
#include <iostream>
#include <vector>
#include "hiredis/hiredis.h"
#include "redis_config.h"
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
    pair<int,pair<int, double>> getHash(string key);

private:
    void init();
    redisContext *m_redis;
    RedisConfig m_config;
};

#endif