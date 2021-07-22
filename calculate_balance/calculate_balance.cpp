#include "calculate_balance.h"
#include "../utils/eign_utils.h"

#include <iostream>

using namespace std;

calculateTotalBalance::calculateTotalBalance() {
    cout << "创建了一个calculateJobType对象！" << endl;
}

/*************************************************
Function:calculateTotalLoad
Description:计算节点的综合负载

Input:判断矩阵、矩阵维数、RI对应表。。。
Return:节点的综合负载评分
*************************************************/
int calculateTotalBalance::calculateTotalLoad() {
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

    /*计算指标权重*/
    double *weights = new double[n];
    weights = AHP(n,p,RI);
    for (int i = 0; i < n; i++)
        cout << weights[i] << endl;



    /*3 使用线性加权矩阵求得各节点的负载评价分*/
};