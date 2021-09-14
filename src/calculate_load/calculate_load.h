/*************************************************
Copyright:gaoziqianghi@163.com
Author:gaoziqianghi@163.com
Date:2021-07-20
Description:计算节点的综合负载
**************************************************/

#ifndef CALCULATE_BALANCE_H_
#define CALCULATE_BALANCE_H_
#include <vector>
#include <string>
#include <utility>

using namespace std;
typedef vector<pair<string, double>> NODE_VECTOR_CL;

/*计算各节点的综合负载*/
class calculateNodeLoad {
private:
    // 节点负载
    double node_load;
    // 发送给中心节点的信息
    NODE_VECTOR_CL node_info;

public:
    /*构造函数*/
    calculateNodeLoad();

    /* 1 获取各个节点的资源剩余情况
     * 包括CPU利用率、内存利用率、磁盘利用率、I/O吞吐量、网络带宽占用率、CPU核心温度
     * 虚拟内存使用率、运行中的任务数、资源能耗、响应时间等指标
     *
     * return@xxx：返回一个资源列表，类似于list或者json
     * */
    void getResourceInfo();

    /*************************************************
    Function:calculateTotalLoad
    Description:计算节点的综合负载
    *************************************************/
    NODE_VECTOR_CL calculateTotalLoad(double * weights);
};
#endif