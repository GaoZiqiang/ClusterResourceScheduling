#include "mysql_config.h"

MysqlConfig::MysqlConfig() {

}

std::string MysqlConfig::getMysqlIP() {
    return "127.0.0.1";
}

int MysqlConfig::getMysqlPort() {
    return 3306;
}

std::string MysqlConfig::getDBName() {
    return "resource_schedule";
}


