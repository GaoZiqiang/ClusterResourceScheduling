#include "schedule.h"
#include "../utils/hiredis/headers/redis_tools.h"
#include "hiredis/hiredis.h"// from /usr/include/hiredis或者/usr/local/lib

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
Function:collectNodeInfoFromSubNodes
Description:从数据库读取各计算节点的负载和作业信息

Input@fileName:候选的node id列表--candidate_nodes_list--vector<int>
Return@itemResults:unordered_map<int,unordered_map<int, double>> node_infos;
*************************************************/
NODES_VECTOR_RS resourceSchedule::collectNodeInfoFromSubNodes(vector<int> &candidate_nodes_list) {
    // RedisTool 对象
    RedisTool redis_tool;
    // 封装HGETALL命令的查询key
//    vector<string> node_idx;
    // 存放候选节点的job_num和node_load信息
//    NODES_VECTOR_RS node_infos;
    for (int i = 0; i < candidate_nodes_list.size(); i++) {
        // 拼接key--node_id_info
        string node_id = "node_" + to_string(candidate_nodes_list[i]) + "_info";// node id从1开始

//        redis_tool.getHash(node_id);
        node_infos.push_back(redis_tool.getHash(node_id));
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
    /*1 进行vector预处理--对输入参数进行格式转换*/
    // 现在不需要对输入参数进行格式转换了
    /*2 根据各节点的作业数进行排序*/
    sort(node_infos.begin(), node_infos.end(), cmpByJob);  // vector根据cmp的规则进行排序

    /*3 再按照节点负载对作业数为0和1的节点分别进行排序，分别得到负载最低的节点*/
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

    /*4 比较作业数为0和1的节点的负载情况，选出最终节点*/
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
//    for (int i = 0; i < merged_vec.size(); i++) {
//        printf("merged_vec[0].node_id: %d\n", merged_vec[i].first);
//    }

    // 目标节点列表
    for (int i = 0; i < merged_vec.size(); i++)
        goal_node_idx.push_back(merged_vec[i].first);

    for (int i = 0; i < goal_node_idx.size(); i++)
        printf("目标节点列表-%d: %d\n", i + 1, goal_node_idx[i]);

    return goal_node_idx;
}