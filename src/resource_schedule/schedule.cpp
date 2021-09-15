#include "schedule.h"
#include "../utils/redis/redis_tools.h"
#include "hiredis/hiredis.h"// from /usr/include/hiredis或者/usr/local/lib
#include "../utils/mysql/mysql_tools.h"

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#define MIN_NODE_LOAD 10.0

using namespace std;

// unordered_map 怎么安装key和value进行排序

// 定义新的类型：重新定义vector，然后对vector按照规则排序
typedef pair<int, pair<int,double>> PAIR;

/*按照节点作业数升序(降序？)排序*/
int cmpByJob(const PAIR& x, const PAIR& y)
{
    return x.second.first < y.second.first;
}

/*按照节点负载升序(降序？)排序*/
int cmpByLoad(const PAIR& x, const PAIR& y)
{
    return x.second.second < y.second.second;
}

/*拼接vector*/
vector<PAIR> mergeVec(const vector<PAIR>& vec1, const vector<PAIR>& vec2, const vector<PAIR>& vec3) {
    vector<PAIR> merged_vec;

    merged_vec.insert(merged_vec.end(), vec1.begin(), vec1.end());
    merged_vec.insert(merged_vec.end(), vec2.begin(), vec2.end());
    merged_vec.insert(merged_vec.end(), vec3.begin(), vec3.end());

    return merged_vec;
}


/*************************************************
Function:collectNodeInfoByRedis
Description:从数据库读取各计算节点的负载和作业信息

Input@fileName:候选的node id列表--candidate_nodes_list--vector<int>
Return@itemResults:unordered_map<int,unordered_map<int, double>> node_infos;
*************************************************/
NODES_VECTOR_RS resourceSchedule::collectNodeInfoByRedis(vector<int> &candidate_nodes_list) {
    // RedisTool 对象
    RedisTool redis_tool;

    for (int i = 0; i < candidate_nodes_list.size(); i++) {
        // 拼接key--node_id_info
        string node_id = "node_" + to_string(candidate_nodes_list[i]) + "_info";// node id从1开始
        node_infos.push_back(redis_tool.getHash(node_id));
    }

    return node_infos;
}


/*从可用节点库中选取*/
NODES_VECTOR_RS resourceSchedule::collectNodeInfoFromAvailNodesByMysql(std::string table) {
    // 从mysql中查询相应记录的job_num和node_load
    MysqlTools mysql_tools;
    //std::string table = "node_info";// 数据表--应该作为形参
    std::string sql = "select * from " + table + " where isfault = 0";// node_info为可用节点表
    printf("sql: %s\n",sql.c_str());
    pair<int, vector<char **>> result = mysql_tools.select(sql);

    int field_count = result.first;// 每条记录的字段个数
    vector<char **> rows = result.second;// 所有查询到的记录
    for (int i = 0; i < rows.size(); i++) {
        printf("node_id: %d, job_num: %d, node_load: %f\n", atoi(rows[i][0]),
               atoi(rows[i][1]),atof(rows[i][2]));

        NODE_PAIR node_info = make_pair(atoi(rows[i][0]), make_pair(atoi(rows[i][1]), atof(rows[i][2])));
        // 存入node_infos
        node_infos.push_back(node_info);
    }

    return node_infos;

}


/*从候选节点队列中选取*/
NODES_VECTOR_RS resourceSchedule::collectNodeInfoFromCandNodesByMysql(std::string table, vector<int> &candidate_nodes_list) {
    MysqlTools mysql_tools;

    for (int i = 0; i < candidate_nodes_list.size(); i++) {
        std::string sql = "select * from " + table + " where node_id = " + to_string(candidate_nodes_list[i]);
        printf("sql: %s\n",sql.c_str());
        pair<int, vector<char **>> result = mysql_tools.select(sql);

        int field_count = result.first;// 每条记录的字段个数
        vector<char **> rows = result.second;// 所有查询到的记录
        // 每次查询只有一条结果--rows[0]
        for (int i = 0; i < rows.size(); i++) {
            printf("node_id: %d, job_num: %d, node_load: %f\n", atoi(rows[i][0]),
                   atoi(rows[i][1]), atof(rows[i][2]));

            NODE_PAIR node_info = make_pair(atoi(rows[i][0]), make_pair(atoi(rows[i][1]), atof(rows[i][2])));
            // 存入node_infos
            node_infos.push_back(node_info);
        }
    }

    return node_infos;
}

/*************************************************
Function:weightedLeastConnection
Description:加权最小连接数法选取/计算目标节点

Input@fileName:node_id,job_num,node_load
Return@itemResults:目标节点id
*************************************************/
vector<int> resourceSchedule::weightedLeastConnection(vector<pair<int,pair<int, double>>> node_infos) {
    /*1 根据各节点的作业数进行排序*/
    sort(node_infos.begin(), node_infos.end(), cmpByJob);  // vector根据cmp的规则进行排序

    /*2 按照节点负载对作业数为0和1的节点分别进行排序，分别得到负载最低的节点*/
    /*分别用来暂存经过排序的job_num为0和1的节点序列*/
    vector<PAIR> sorted_vec0, sorted_vec1, sorted_vec2;  //
    /*作业数为0和1的节点最大负载*/
    double min_load_job_0 = MIN_NODE_LOAD, min_load_job_1 = MIN_NODE_LOAD;
    /*作业数为0和1的负载最大的节点的编号id*/
    int min_load_id_job_0 = -1, min_load_id_job_1 = -1;// 初始化为-1

    for(int i = 0; i <node_infos.size(); i++) {
        int node_id = node_infos[i].first;// 节点编号
        int job_num = node_infos[i].second.first;// 节点的作业数量
        double node_load = node_infos[i].second.second;// 节点负载

        /*作业数为0的节点*/
        if (job_num == 0) {
            // 暂存经过排序的job_num为0的节点序列
            sorted_vec0.push_back({node_id, {job_num, node_load}});

            // 找job_num == 0的负载最小的节点
            if (node_load < min_load_job_0) {
                min_load_job_0 = node_load;// 更新保存node_load
                min_load_id_job_0 = node_id;// 更新保存node_id
            }
        }else if (job_num == 1) {
            // 暂存经过排序的job_num为1的节点序列
            sorted_vec1.push_back({node_id, {job_num, node_load}});

            // 找job_num == 1的负载最小的节点
            if (node_load < min_load_job_1) {
                min_load_job_1 = node_load;// 更新保存node_load
                min_load_id_job_1 = node_id;// 更新保存node_id
            }
        }else if (job_num == 2) {
            sorted_vec2.push_back({node_id, {job_num, node_load}});
            //cout << "job_num为2，不参与节点分配" << endl;
        }

    }

    /*3 比较作业数为0和1的节点的负载情况，选出最终节点*/
    int final_node_id = -1;

    if (min_load_job_0 > 0 && min_load_job_0 < 10.0) {// 若作业数为0的节点不存在（即max_load_job0 < 0），则从作业数为1的节点中选择
        final_node_id = min_load_id_job_0;
        // return final_node_id;
        // 优化
        // return min_load_id_job_0;
    }else {// 从作业数为1的节点中选择
        if (min_load_job_1 > 0 && min_load_job_1 < 10.0) {
            final_node_id = min_load_id_job_1;
            // return final_node_id;
        }else
            cout << "没有可用节点!" << endl;
    }

    // 返回目标节点
    cout << "最优目标节点id为: " << final_node_id << endl;
//    return final_node_id;

    // 返回目标节点列表
    sort(sorted_vec0.begin(), sorted_vec0.end(), cmpByLoad);
    sort(sorted_vec1.begin(), sorted_vec1.end(), cmpByLoad);
    sort(sorted_vec2.begin(), sorted_vec2.end(), cmpByLoad);

    vector<PAIR> merged_vec = mergeVec(sorted_vec0, sorted_vec1, sorted_vec2);
    // 目标节点列表
    for (int i = 0; i < merged_vec.size(); i++)
        goal_node_idx.push_back(merged_vec[i].first);

    printf("------ 目标节点列表 ------\n");
    for (int i = 0; i < goal_node_idx.size(); i++)
        printf("目标节点-%d: %d\n", i + 1, goal_node_idx[i]);

    return goal_node_idx;
}