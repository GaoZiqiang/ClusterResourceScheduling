#include "mysql_tools.h"
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

MysqlTools::MysqlTools() {
    mysql = NULL;
    init();
//    printf("MysqlTools 初始化成功!\n");
}


MysqlTools::~MysqlTools() {
    if (mysql != NULL) {
        mysql_close(mysql);
//        printf("MysqlTools::~MysqlTools()解析成功!\n");
    }
}


void MysqlTools::init() {
    char ip[255], database[20];
    // string转char *
    strcpy(ip, m_config.getMysqlIP().c_str());
    strcpy(database, m_config.getDBName().c_str());

    if ((mysql = mysql_init(mysql)) == NULL) {
        printf("mysql_init()失败!\n");
        exit(1);
    }

    // connect
    mysql_real_connect(mysql, ip ,"root" ,"root" ,
                       database,m_config.getMysqlPort(),NULL,0);
}

pair<int, vector<char **>> MysqlTools::select(std::string sql) {
    if (mysql == NULL) {
        printf("mysql == NULL, exist!\n");
        exit(1);
    }

    // query
    if ((mysql_query(mysql, sql.c_str())) == 0) {
        printf("查询成功！\n");
    } else {
        printf("查询失败!%s!\n",strerror(errno));
        exit(1);
    }

    MYSQL_RES *result = NULL;
    if ((result = mysql_store_result(mysql)) == NULL) {
        printf("查询失败! %s!\n", strerror(errno));
        exit(1);
    }

    // 所得记录条数
    int row_count = mysql_num_rows( result );

    // 每条记录的字段个数和字段名
    int field_count = mysql_num_fields( result );

    // 得到所有字段的名字
    MYSQL_FIELD* field = NULL;

    // 显示表中的所有数据
    MYSQL_ROW row = NULL;
    row = mysql_fetch_row( result );

    // 保存查询所得记录
    vector<char **> rows;
    while ( NULL != row )
    {
        rows.push_back(row);
        row = mysql_fetch_row( result );
    }

    // 封装返回值
    pair<int, vector<char **>> select_result = make_pair(field_count,rows);

    return select_result;
}

void MysqlTools::insert(char * sql) {
    if ((mysql_query(mysql, sql)) == 0) {
        printf("插入成功！\n");
        return ;
    } else {
        printf("插入失败!%s!\n",strerror(errno));
        exit(1);
    }
}