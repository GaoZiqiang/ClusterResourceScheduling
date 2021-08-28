#include "calculate_balance.h"
#include "../utils/eign_utils.h"
//#include "../utils/resource_info_utils.h"

#include <iostream>
#include <thread>

using namespace std;

calculateTotalBalance::calculateTotalBalance() {
    // cout << "创建了一个calculateJobType对象！" << endl;
}

//calculateTotalBalance::~calculateTotalBalance() {
//    cout << "析构函数" << endl;
//}


/*************************************************
Function:calculateTotalLoad
Description:计算节点的综合负载

Input:各资源指标值、指标权重
Return:节点的综合负载评分
*************************************************/
double calculateTotalBalance::calculateTotalLoad() {
    /*1 输入各个节点的资源列表*/

    /* 2 使用AHP计算指标权重值
     *
     * 输入：
     * 判断矩阵A
     * RI
     *
     *
     * */

    /*这些变量要放在形参里面的*/
    // 资源指标个数
    int n = 4;

    // 判断矩阵A
    double p[16] = {1, 1, 3, 5, 1, 1, 3, 5, 0.3333, 0.3333, 1, 2, 0.2, 0.2, 0.5, 1};
    // RI对应表
    double RI[11]={0,0,0.58,0.90,1.12,1.24,1.32,1.41,1.45,1.49,1.51};

    /*2 计算指标权重*/
//    double *weights;// 只需要定义一个指针数组的首指针即可（或者说只需要定义一个指针即可！！！）
//    weights = AHP(n,p,RI);
//    for (int i = 0; i < n; i++)
//        cout << weights[i] << endl;

    /*3 获取各项资源的指标值*/
    cout << "各项资源指标为：" << endl;
    double R_cpu,R_mem,R_net,R_disk;



    thread thread_cpu(calCPUInfo);
    thread thread_mem(calMemInfo);
    thread thread_net(calNetworkInfo);
    thread thread_disk(calDiskInfo);
    /*R_cpu*/
//    R_cpu = calCPUInfo();
//    thread thread_cpu(calCPUInfo);
    thread_cpu.join();

    /*R_mem*/
//    R_mem = calMemInfo();
//    thread thread_mem(calMemInfo);
    thread_mem.join();

    /*R_net*/
//     R_net = calNetworkInfo();
//    thread thread_net(calNetworkInfo);
    thread_net.join();

    /*R_disk*/
//     R_disk = calDiskInfo();
//    thread thread_disk(calDiskInfo);
    thread_disk.join();

    /*4 使用线性加权矩阵求得各节点的负载评价分*/
    /*各项指标权重*/
    double *weights;// 只需要定义一个指针数组的首指针即可（或者说只需要定义一个指针即可！！！）
//    double weights[4] = {0.11,0.13,0.45,0.69};
    weights = AHP(n,p,RI);
//    thread thread_ahp(AHP,n,p,RI);
//    thread_ahp.join();
    double w1 = weights[0],w2 = weights[1],w3 = weights[2],w4 = weights[3];
    // weights用完后要及时释放--本质是释放指针所指向的内存空间，而不是删除指针本身
    delete [] weights;
    /*各项指标值*/
    // double R_cpu,R_mem,R_net,R_disk;
    /*计算节点总负载*/
    double free_score = w1 * R_cpu + w2 * R_mem + w3 * R_net + w4 * R_disk;
    // cout << "total free_score = " << free_score << endl;
    double node_load = 1.0 - free_score;
    cout << "节点综合负载：" << node_load << endl;

    return node_load;
};