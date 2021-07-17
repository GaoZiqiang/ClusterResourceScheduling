/*计算各节点的综合负载*/
class calculateTotalBalance {
public:
    /* 计算各个节点的综合负载
     * 第一步:获取各个节点的资源剩余情况
     * 第二步:计算各计算节点的综合负载评价值
     *
     * */

    /*获取各个节点的资源剩余情况*/
    void getResourceInfo();

    /* 层次分析法AHP计算各计算节点的综合负载评价值
     *
     * param@resources:节点的资源情况
     *
     * return@返回节点的综合负载评价值
     * */
    int calculateTotalLoad(List<int> resources,);



};

/*计算作业的类型*/
class calculateJobType {
public:
    /* 计算作业的类型
     * 第一步:获取作业对资源的需求情况
     * 第二步:计算作业的类型
     *
     * */


    /*作业的资源需求情况*/
    void resourceJobRequired();

    /*计算作业类型*/
    void calculateJobType();
};