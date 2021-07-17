/* 进行资源调度
 *
 * 为计算作业选择计算节点的方法
 *
 * 两个辅助工作：计算作业类型和节点的综合负载评价值--从calculate_balance层传递
 *
 * */
#include <vector>

/*负载均衡算法*/
class loadBalance {
public:
    /*负载均衡算法--为计算作业选择计算节点的方法*/

    /* 轮询法
     * param@jobType：作业的类型 CPU 密集型、内存密集型、I/O 密集型和网络密集型
     * return:selected serverId
     * */
    int roundRobin(vector<int> jobList, vector<int> serverList);

    /*随机法*/
    int Random(vector<int> jobList, vector<int> serverList);

    /*Hash法*/
    int Hash(vector<int> jobList, vector<int> serverList);


    /*还有其他的一些负载均衡算法*/
};

/*根据作业类型和节点的负载情况，为计算作业选择合适的计算节点*/
class resourceSchedule {
public:

    /* 两个预备工作
     * 1 计算作业类型
     * 2 获取节点的综合负载情况(综合负载评价值)
     * */

    /* 计算作业类型
     * 计算细节在calculate_balance层
     * */
    int getJobType();

    /* 获取节点的综合负载评价值--传递
     * 计算细节在calculate_balance层
     * */
    int getServerBalance();

    /*根据作业类型和节点的负载情况，为计算作业选择合适的计算节点
     *
     * param@loadName:选择要使用的负载均衡算法
     * */
    void selectNodeForJob(int jobId, char * const jobName, char loadName);

};