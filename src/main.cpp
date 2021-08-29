#include <iostream>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "calculate_balance/calculate_balance.h"
#include "resource_schedule/schedule.h"
#include "remote_procedure_call/receive_nodes_info.h"
//#include "utils/resource_info_utils.h"
//#include "utils/eign_utils.h"

using namespace std;

typedef pair<int, pair<int,double>> PAIR;

void thread01()
{
    // clock_t thread01_start_time = clock();
    cout << "thread01_start_time: " << clock() << endl;
    for (int i = 0; i < 5; i++)
    {
        // cout <<"Thread 01 is working ！" << endl;
        usleep(100);
    }
    cout << "thread01_end_time: " << clock() << endl;
}
void thread02()
{
    cout << "thread02_start_time: " << clock() << endl;
    for (int i = 0; i < 5; i++)
    {
        // cout <<"Thread 02 is working ！" << endl;
        usleep(100);
    }
    cout << "thread02_end_time: " << clock() << endl;
}


int main() {

//    thread task01(thread01);
//
//
//    thread task02(thread02);
//    task02.join();
//    task01.join();




    clock_t start_time = clock();
    /*整体流程测试*/
    /*1 计算slave node从属子节点的节点负载*/
    calculateTotalBalance ctd;
    // 程序运行报错--std::bad_alloc
    vector<pair<string, double>> node_info;
//    node_info = ctd.calculateTotalLoad();
    ctd.calculateTotalLoad();
//    node_info.swap(node_info);
//    ctd.calculateTotalLoad();
//    vector<pair<string, double>>(node_info).swap(node_info);

    clock_t end_time = clock();
//
    cout << "total time: " << (end_time - start_time) << endl;


    /*1 模拟构造输入node_infos*/
//    vector<pair<int, pair<int, double>>> node_infos;
//    node_infos = {
//            make_pair(1,make_pair(1,0.69)),
//            make_pair(2,make_pair(1,0.32)),
//    };
//    for (int i = 0; i < node_infos.size(); i++) {
//        cout << "node id: " << node_infos[i].first << " " << "job num: " << node_infos[i].second.first << " "
//             << "node load: " << node_infos[i].second.second << endl;
//    }

//    // 候选节点列表
//    vector<int> candidte_nodes = {1,2,3,4};
//    /*3 master node中心节点 根据node_job_load_map，使用加权最小连接数法选择目标节点*/
//    loadBalance lb;
//    // 收集子节点的node_load和job_num信息
//    vector<pair<int, pair<int, double>>> node_infos = lb.collectNodeInfoFromSubNodes(candidte_nodes);
//    for (int i = 0; i < node_infos.size(); i++) {
//        cout << "node id: " << node_infos[i].first << " " << "job num: " << node_infos[i].second.first << " "
//             << "node load: " << node_infos[i].second.second << endl;
//    }
//    // 选取目标节点
//    lb.weightedLeastConnection(node_infos);






//    vector<PAIR> node_info_vec;
//
//    node_info_vec.push_back({1, {0, 0.48}});// 输入经过calculateTotalLoad()计算得到的node_load
//    node_info_vec.push_back({2, {2, 0.87}});
//    node_info_vec.push_back({3, {0, 0.45}});
//    node_info_vec.push_back({4, {1, 0.62}});
//    node_info_vec.push_back({5, {2, 0.79}});
//    node_info_vec.push_back({6, {2, 0.87}});
//    node_info_vec.push_back({7, {1, 0.77}});
//    node_info_vec.push_back({8, {2, 0.90}});
//    node_info_vec.push_back({9, {0, node_load}});
//    node_info_vec.push_back({10, {1, 0.65}});
//
//    /*2 master node中心节点通过外部交互层接收slave node从属节点发送来的node_info_vec，并进行结构，重新封装成node_job_load_map*/
//    unordered_map<int,unordered_map<int,double>> node_job_load_map =
//            receiveSlaveNodesInfo(node_info_vec);
//
//    /*3 master node中心节点 根据node_job_load_map，使用加权最小连接数法选择目标节点*/
//    loadBalance lb;
//    lb.weightedLeastConnection(node_job_load_map);

//    loadBalance lb;
//    lb.weightedLeastConnection(node_job_load_map);
//    calDiskInfo();
//    calMemInfo();
//    calCPUInfo();

    return 0;
}
