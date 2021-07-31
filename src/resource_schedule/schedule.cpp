#include "schedule.h"

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <vector>

#define MIN_NODE_LOAD 10.0

using namespace std;

// unordered_map 怎么安装key和value进行排序

// 定义新的类型：重新定义vector，然后对vector按照规则排序
typedef pair<int, pair<int,double>> PAIR;

/*按照节点作业数升序(降序？)排序*/
int cmp_by_job(const PAIR& x, const PAIR& y)
{
    return x.second.first < y.second.first;
}

/*按照节点负载升序(降序？)排序*/
int cmp_by_load(const PAIR& x, const PAIR& y)
{
    return x.second.second < y.second.second;
}


int loadBalance::weightedLeastConnection(unordered_map<int,unordered_map<int,double>> node_job_load_map) {

    /* node_job_load_map应作为输入参数
     *
     * node_job_load_map结构：{node_id,{job_num,node_load}}
     * */
//    int node1 = 1, node2 = 2, node3 = 3, node4 = 4;
//    unordered_map<int,unordered_map<int,double>> node_job_load_map = {
//            {node1, {{0,0.65}}},
//            {node2, {{2,0.61}}},
//            {node3, {{0,0.87}}},
//            {node4, {{1,0.45}}},
//    };

    /*1 进行vector预处理*/
    vector<PAIR> vec;  // 重新定义类型
    // 将unordered_map 中的值重新装入新定义的vector中
    for(auto it= node_job_load_map.begin(); it != node_job_load_map.end(); it ++) {
        int node_id = it->first;
        auto sub_it = it->second.begin();
        int node_job_num = sub_it->first;
        // printf("node_job_num:%d\n",node_job_num);
        double node_load = sub_it->second;
        // printf("node_load:%f\n",node_load);
        vec.push_back({node_id, {node_job_num,node_load}});
    }

    /*2 根据各节点的作业数进行排序*/
    sort(vec.begin(), vec.end(), cmp_by_job);  // vector根据cmp的规则进行排序

    /*3 再按照节点负载对作业数为0和1的节点分别进行排序，分别得到负载最低的节点*/
    /*分别用来暂存经过排序的job_num为0和1的节点序列*/
    vector<PAIR> sorted_vec0,sorted_vec1;  //
    /*作业数为0和1的节点最大负载*/
    double min_load_job_0 = MIN_NODE_LOAD, min_load_job_1 = MIN_NODE_LOAD;
    /*作业数为0和1的负载最大的节点的编号id*/
    int min_load_id_job_0 = -1, min_load_id_job_1 = -1;// 初始化为-1

    for(int i=0;i<vec.size();i++) {
        int node_id = vec[i].first;// 节点编号
        int job_num = vec[i].second.first;// 节点的作业数量
        double node_load = vec[i].second.second;// 节点负载
//        cout << "node_id: " << vec[i].first << endl;
//        int job_num = vec[i].second.first;
//        cout << "job_num: " << vec[i].second.first << endl;

        /*作业数为0的节点*/
        if (job_num == 0) {
            // 暂存经过排序的job_num为0的节点序列
            sorted_vec0.push_back({node_id, {job_num,node_load}});
            // 找job_num == 0的负载最小的节点
            if (node_load < min_load_job_0) {
                min_load_job_0 = node_load;// 更新保存node_load
                min_load_id_job_0 = node_id;// 更新保存node_id
            }
        }else if (job_num == 1) {
            // 暂存经过排序的job_num为1的节点序列
            sorted_vec1.push_back({node_id, {job_num,node_load}});
            // 找job_num == 1的负载最小的节点
            if (node_load < min_load_job_1) {
                min_load_job_1 = node_load;// 更新保存node_load
                min_load_id_job_1 = node_id;// 更新保存node_id
            }
        }else if (job_num == 2) {
            continue;
            //cout << "job_num为2，不参与节点分配" << endl;
        }

    }

//    cout << "max_load_job0: " << max_load_job0 << endl;
//    cout << "max_loadId_job0: " << max_loadId_job0 << endl;
//    cout << endl;
//    cout << "max_load_job1: " << max_load_job1 << endl;
//    cout << "max_loadId_job1: " << max_loadId_job1 << endl;

    /*4 比较作业数为0和1的节点的敷在情况，选出最终节点*/
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

    cout << "选出的目标节点id为: " << final_node_id << endl;

    return final_node_id;
}
