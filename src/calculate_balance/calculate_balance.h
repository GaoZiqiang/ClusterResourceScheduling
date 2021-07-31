/*************************************************
Copyright:gaoziqianghi@163.com
Author:gaoziqianghi@163.com
Date:2021-07-20
Description:计算节点的综合负载
**************************************************/

#ifndef CALCULATE_BALANCE_H_
#define CALCULATE_BALANCE_H_

#pragma once

/*计算各节点的综合负载*/
class calculateTotalBalance {
private:
    int resource_num;
    double *judge_matrix = new double[resource_num*resource_num];// 定义动态数组
    double RI[11];

public:
    /*构造函数*/
    calculateTotalBalance();
    /*delete []W问题*/
//    ~calculateTotalBalance();

    /* 计算各个节点的综合负载
     * 第一步:获取各个节点的资源剩余情况
     * 第二步:计算各计算节点的综合负载评价值
     *
     * */

    /* 1 获取各个节点的资源剩余情况
     * 包括CPU利用率、内存利用率、磁盘利用率、I/O吞吐量、网络带宽占用率、CPU核心温度
     * 虚拟内存使用率、运行中的任务数、资源能耗、响应时间等指标
     *
     * return@xxx：返回一个资源列表，类似于list或者json
     * */
    void getResourceInfo();



    /* 2 层次分析法AHP计算各计算节点的综合负载评价值
     *
     * param@resources:节点的资源情况
     *
     * return@返回节点的综合负载评价值--可以进一步引申为节点的优先级
     * */
//    int calculateTotalLoad(List<int> resources,);



    /*************************************************
    Function:calculateTotalLoad
    Description:计算节点的综合负载
    *************************************************/
    double calculateTotalLoad();



};

/*计算作业的类型*/
class calculateJobType {
public:
    calculateJobType();
    ~calculateJobType();

    /* 计算作业的类型
     * 第一步:获取作业对资源的需求情况
     * 第二步:计算作业的类型
     *
     * */


    /*作业的资源需求情况*/
    void resourceJobRequired();

    /*计算作业类型*/
    void calculateJobType_();
};
#endif