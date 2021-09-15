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
typedef pair<int,pair<int, double>> NODE_PAIR;

class resourceSchedule {
private:
    // 从数据库中读取的节点信息
    NODES_VECTOR_RS node_infos;
    // 候选目标节点列表
    vector<int> goal_node_idx;

public:
    /*从数据库读取各计算节点的负载和作业信息*/
    /*从redis中读取*/
    NODES_VECTOR_RS collectNodeInfoByRedis(vector<int> &candidate_nodes_list);
    /*从mysql中读取*/
    // 从可用节点库中选取
    NODES_VECTOR_RS collectNodeInfoFromAvailNodesByMysql(std::string table);
    // 从候选节点列表中选取
    NODES_VECTOR_RS collectNodeInfoFromCandNodesByMysql(std::string table, vector<int> &candidate_nodes_list);

    /*选取目标节点--加权最小连接数法*/
    vector<int> weightedLeastConnection(vector<pair<int,pair<int, double>>> node_infos);
};