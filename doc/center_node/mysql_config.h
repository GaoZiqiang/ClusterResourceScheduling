#ifndef MYSQL_CONFIG
#define MYSQL_CONFIG

#include <string>

class MysqlConfig {
public:
    MysqlConfig();

    std::string getMysqlIP();// 获取ip
    int getMysqlPort();// 获取端口

    // 应该返回一个数据库列表和表的列表
    // 这样设计不合理--应该有一个setDBName()方法
    std::string getDBName();// 数据库
};

#endif