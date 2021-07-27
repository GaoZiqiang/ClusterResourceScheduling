#include <unordered_map>
#include <vector>

using namespace std;

typedef pair<int, pair<int,double>> PAIR;

/* 获取各子计算节点的运行作业数量和各子节点的负载值
 *
 * Input@node_info_vec：元素类型为PAIR的数组vector（监控模块的 所有 节点的信息要封装成vector<PAIR>类型的vector）
 * */
unordered_map<int,unordered_map<int,double>> receiveSlaveNodesInfo(vector<PAIR> node_info_vec) {

    unordered_map<int,unordered_map<int,double>> node_job_load_map;

    // 可以遍历node_info_vec
    for (auto vec = node_info_vec.begin(); vec != node_info_vec.end(); vec++) {
        /*初始化节点信息*/
        int node_id = vec->first;
        int job_num = vec->second.first;
        double node_load = vec->second.second;
        /*定义该节点的unordered_map元素*/
        unordered_map<int,double> node_map = {{job_num,node_load}};
        /*插入node_job_load_map*/
        node_job_load_map.insert({node_id,node_map});
    }

//    /*初始化节点信息*/
//    int node_id = node_info_vec.begin()->first;// node_id
//    int job_num = node_info_vec.begin()->second.first;
//    double node_load = node_info_vec.begin()->second.second;
//    /*定义该节点的unordered_map元素*/
//    unordered_map<int,double> node_map = {{job_num,node_load}};
//    /*插入node_job_load_map*/
//    node_job_load_map.insert({node_id,node_map});

//    // 方案一：换成unordered_map<int,vector<double>> map->second换成一个数组 job_num的数据类型换成double
//    unordered_map<int,vector<double>> new_map;
//    new_map.insert({1,{2,0.2}});

//    // 方案二：预定义待插入的元素格式
//    int node5 = 5;
//    unordered_map<int,double> node5_map = {{1, 0.2}};
//    node_job_load_map.insert({node5, node5_map});

    return node_job_load_map;
}