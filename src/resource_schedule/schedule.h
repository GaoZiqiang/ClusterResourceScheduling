/*************************************************
Copyright:gaoziqianghi@163.com
Author:gaoziqianghi@163.com
Date:2010-08-25
Description:进行资源调度
**************************************************/


/* 进行资源调度
 *
 * 为计算作业选择计算节点的方法
 *
 * 两个辅助工作：计算作业类型和节点的综合负载评价值--从calculate_balance层传递
 *
 * */
#include <vector>
#include <unordered_map>

using namespace std;

typedef vector<pair<int,pair<int, double>>> NODES_VECTOR_RS;

class resourceSchedule {
private:
    // 从数据库中读取的节点信息
    NODES_VECTOR_RS node_infos;
    // 候选目标节点列表
    vector<int> goal_node_idx;

public:
    /*从数据库读取各计算节点的负载和作业信息*/
    NODES_VECTOR_RS collectNodeInfoFromSubNodes(vector<int> &candidate_nodes_list);

    /* 加权最小连接数法
     * param@jobType：作业的类型 CPU 密集型、内存密集型、I/O 密集型和网络密集型
     * return:selected serverId
     * */
    vector<int> weightedLeastConnection(vector<pair<int,pair<int, double>>> node_infos);
};