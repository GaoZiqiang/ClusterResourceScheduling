#include "mysql_config.h"
#include <mysql/mysql.h>
#include <vector>
#include <utility>

using namespace std;

class MysqlTools {
private:
    void init();
    MYSQL *mysql;
    MysqlConfig m_config;

public:
    MysqlTools();
    ~MysqlTools();

    pair<int, vector<char **>> select(std::string sql);
    void insert(char * sql);
};