#include <iostream>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <pthread.h>
#include <unistd.h>

#include "../calculate_balance/calculate_balance.h"
#include "../resource_schedule/schedule.h"
#include "../interface_layer/receive_nodes_info.h"
#include "resource_info_utils.h"

using namespace std;

typedef pair<int, pair<int,double>> PAIR;

void thread01()
{
    for (int i = 0; i < 5; i++)
    {
        cout <<"Thread 01 is working ！" << endl;
        usleep(100);
    }
}
void thread02()
{
    for (int i = 0; i < 5; i++)
    {
        cout <<"Thread 02 is working ！" << endl;
        usleep(100);
    }
}


int main() {

//    thread task01(thread01);
//    thread task02(thread02);
//    task01.join();
//    task02.join();

//    for (int i = 0; i < 5; i++)
//    {
//        cout <<"Main thread is working ！" << endl;
//        usleep(200);
//    }

    /*整体流程测试*/
    /*1 slave node从属节点计算各自的节点负载*/
    calculateTotalBalance ctd;
    double node_load;
    node_load = ctd.calculateTotalLoad();

    /*模拟构造输入node_info_vec*/
    vector<PAIR> node_info_vec;

    node_info_vec.push_back({1, {0, node_load}});// 输入经过calculateTotalLoad()计算得到的node_load
    node_info_vec.push_back({2, {2, 0.61}});
    node_info_vec.push_back({3, {0, 0.87}});
    node_info_vec.push_back({4, {1, 0.45}});

    /*2 master node中心节点通过外部交互层接收slave node从属节点发送来的node_info_vec，并进行结构，重新封装成node_job_load_map*/
    unordered_map<int,unordered_map<int,double>> node_job_load_map =
            receiveSlaveNodesInfo(node_info_vec);

    /*3 master node中心节点 根据node_job_load_map，使用加权最小连接数法选择目标节点*/
    loadBalance lb;
    lb.weightedLeastConnection(node_job_load_map);

//    loadBalance lb;
//    lb.weightedLeastConnection(node_job_load_map);
//    calDiskInfo();
//    calMemInfo();
//    calCPUInfo();

    return 0;
}
