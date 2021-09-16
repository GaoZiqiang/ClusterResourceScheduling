#include <iostream>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "calculate_load/calculate_load.h"
#include "resource_schedule/schedule.h"
#include "remote_procedure_call/receive_nodes_info.h"
#include "utils/get_ahp_params.h"
#include "utils/mysql/mysql_tools.h"

//#include "utils/resource_info_utils.h"
//#include "utils/eign_utils.h"

using namespace std;

typedef pair<int, pair<int,double>> PAIR;

int main() {

    /*mysql数据库插入*/
//    MysqlTools mysql_tools;
//    char sql[200];
//    int node_id = 8, job_num = 0;
//    double total_load = 0.16, cpu = 0.21, mem = 0.21, disk = 0.09, net = 0.01;
//    sprintf(sql,"insert into node_info (node_id,job_num,total_load,cpu,mem,disk,net) values(%s,%s,%s,%s,%s,%s,%s);",
//            to_string(node_id).c_str(),to_string(job_num).c_str(),to_string(total_load).c_str(),to_string(cpu).c_str(),
//            to_string(mem).c_str(),to_string(disk).c_str(),to_string(net).c_str());
//    printf("sql: %s\n",sql);
//    mysql_tools.insert(sql);

    /*选取目标节点测试--可用节点库mysql*/
//    resourceSchedule rs;
//    // 收集子节点的node_load和job_num信息
//    std::string table_name = "node_info";
//    NODES_VECTOR_RS node_infos = rs.collectNodeInfoFromAvailNodesByMysql(table_name);
//    for (int i = 0; i < node_infos.size(); i++) {
//        cout << "node id: " << node_infos[i].first << " " << "job num: " << node_infos[i].second.first << " "
//             << "node load: " << node_infos[i].second.second << endl;
//    }
//    // 选取目标节点
//    rs.weightedLeastConnection(node_infos);

    /*选取目标节点测试--候选节点列表mysql*/
    resourceSchedule rs;
    // 收集子节点的node_load和job_num信息
    std::string table_name = "node_info";
    vector<int> candidte_nodes = {1,2,3,5};
    NODES_VECTOR_RS node_infos = rs.collectNodeInfoFromCandNodesByMysql(table_name, candidte_nodes);
    for (int i = 0; i < node_infos.size(); i++) {
        cout << "node id: " << node_infos[i].first << " " << "job num: " << node_infos[i].second.first << " "
             << "node load: " << node_infos[i].second.second << endl;
    }
    // 选取目标节点
    rs.weightedLeastConnection(node_infos);



//    MysqlTools m_tool;
//
//    std::string sql = "select * from node_info where isfault = 0";
//
////    select_result *sr;
//    pair<int, vector<char **>> result = m_tool.select(sql);
//
//    printf("rs->field_count: %d\n",result.first);
//    int field_count = result.first;
//    vector<char **> rows = result.second;
//
//    for (int i = 0; i < rows.size(); i++) {
//        printf("node_id: %d, job_num: %d, node_load: %f\n", atoi(rows[i][0]),atoi(rows[i][1]),atof(rows[i][2]));
//    }
//    for (int i = 0; i < rows.size(); i++) {
//        printf("第 %d 行:\n",(i+1));
////        int len = result[i].size();
//        for (int j = 0; j < field_count; j++)
//            printf("%s ",rows[i][j]);
//        printf("\n");
//    }



//    int field_count = sr->field_count;
//    vector<char **> * result = sr->rows;
//    result[0];
//    for (auto iter = result->begin(); iter != result->end(); iter++) {
//        //printf("第 %d 行:\n",(i+1));
////        char * vec = iter;
////        printf("%s\n",iter);
//        for (int j = 0; j < 2; j++)
//            printf("%s ",iter[j]);
//        printf("\n");
//    }

//    for (int i = 0; i < result->size(); i++) {
//        printf("第 %d 行:\n",(i+1));
////        int len = result[i].size();
//        for (int j = 0; j < field_count; j++)
//            printf("%s ",result[i][j]);
//        printf("\n");
//    }


    /*节点负载测试1--周期性计算*/
//    char param_file[] = "../config/ahp_param.txt";
//    double *weights = resourceParamsInit(param_file);
//
//    uint32_t now_time = 0;
//    uint32_t last_time = time(0);
//    int i = 0;
//
//    calculateNodeLoad ctd;
//    NODE_VECTOR_CL node_info;
//
//    while (true) {
//        now_time = time(0);
////        printf("now_time: %d\n",now_time);
////        printf("last_time: %d\n",last_time);
//        if (now_time - last_time >= 10) {
//            printf("第 %d 次, now_time: %d\n",++i, now_time);
//            node_info = ctd.calculateTotalLoad(weights);
//            printf("node_info[0].second: %f\n",node_info[0].second);
//            last_time = now_time;
//        }
//    }

    /*计算节点负载测试2*/
//    clock_t start_time = clock();
//    /*整体流程测试*/
//    /*1 计算slave node从属子节点的节点负载*/
//    calculateTotalBalance ctd;
//    // 程序运行报错--std::bad_alloc
//    vector<pair<string, double>> node_info;
//    node_info = ctd.calculateTotalLoad();
//    ctd.calculateTotalLoad();

    /*选取目标节点测试*/
//    // 候选节点列表--redis
//    vector<int> candidte_nodes = {1,2,3,4,5,6};
//    /*3 master node中心节点 根据node_job_load_map，使用加权最小连接数法选择目标节点*/
//    resourceSchedule rs;
//    // 收集子节点的node_load和job_num信息
//    vector<pair<int, pair<int, double>>> node_infos = rs.collectNodeInfoByRedis(candidte_nodes);
//    for (int i = 0; i < node_infos.size(); i++) {
//        cout << "node id: " << node_infos[i].first << " " << "job num: " << node_infos[i].second.first << " "
//             << "node load: " << node_infos[i].second.second << endl;
//    }
//    // 选取目标节点
//    rs.weightedLeastConnection(node_infos);

    /*集成测试用*/
//    // 节点信息
//    pair<int, pair<int, double>> node_info1 = make_pair(1, make_pair(0, 0.25));
//    pair<int, pair<int, double>> node_info2 = make_pair(2, make_pair(0, 0.36));
//    pair<int, pair<int, double>> node_info3 = make_pair(3, make_pair(1, 0.60));
//    pair<int, pair<int, double>> node_info4 = make_pair(4, make_pair(1, 0.55));
//    pair<int, pair<int, double>> node_info5 = make_pair(5, make_pair(2, 0.92));
//
//    // 候选节点列表--模拟从数据库中读取的节点作业和负载信息
//    vector<pair<int, pair<int, double>>> node_infos;
//    node_infos.push_back(node_info1);
//    node_infos.push_back(node_info2);
//    node_infos.push_back(node_info3);
//    node_infos.push_back(node_info4);
//    node_infos.push_back(node_info5);
//
//    resourceSchedule rs;
//    rs.weightedLeastConnection(node_infos);

    return 0;
}
