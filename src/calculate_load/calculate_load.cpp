#include "calculate_load.h"
#include "../utils/eign_utils.h"
//#include "../utils/get_ahp_params.h"
#include "../utils/send_alarm.h"
#include "../utils/resource_info_utils.h"

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

calculateNodeLoad::calculateNodeLoad() {
    // cout << "创建了一个calculateJobType对象！" << endl;
}

//calculateTotalBalance::~calculateTotalBalance() {
//    cout << "析构函数" << endl;
//}


//double * calculateNodeLoad::getAHPParams(char * file_path) {
//    using namespace std;
//    ifstream inFile;        // object for handling file input
//
//    inFile.open(file_path);  // associate inFile with a file
//    if (!inFile.is_open())  // failed to open file
//    {
//        cout << "Could not open the file " << file_path << endl;
//        cout << "Program terminating.\n";
//        exit(1);
//    }
//    double value;
//    double *ahp_params = new double[4];
//    int i = 0;
//
//    inFile >> value;        // get first value
//    while (inFile.good())   // while input good and not at EOF
//    {
//        ahp_params[i++] = value;
//        inFile >> value;    // get next value
//    }
//    if (inFile.eof()) {
////        cout << "End of file reached.\n";
//        inFile.close();
//        return ahp_params;
//    }
//    else if (inFile.fail()) {
//        cout << "Input terminated by data mismatch.\n";
//        exit(1);
//    }
//    else {
//        cout << "Input terminated for unknown reason.\n";
//        exit(1);
//    }
//}

/*************************************************
Function:calculateTotalLoad
Description:计算节点的综合负载

Input:各资源指标值、指标权重
Return:节点的综合负载评分
*************************************************/
NODE_VECTOR_CL calculateNodeLoad::calculateTotalLoad(double * weights) {
    /*1 获取各项资源的指标值*/
    double R_cpu,R_mem,R_net,R_disk;

    R_cpu = resource::calCPUInfo();
    R_mem = resource::calMemInfo();
    R_net = resource::calNetworkInfo();
    R_disk = resource::calDiskInfo();

    // 多线程并发执行--还有待优化
//    thread thread_cpu(calCPUInfo);
//    thread thread_mem(calMemInfo);
//    thread thread_net(calNetworkInfo);
//    thread thread_disk(calDiskInfo);
//
//    thread_cpu.join();
//    thread_mem.join();
//    thread_net.join();
//    thread_disk.join();

    /*2 各项指标权重*/
//    static double *weights = getAHPParams(param_file);// 只需要定义一个指针数组的首指针即可（或者说只需要定义一个指针即可！！！）

    /*计算节点总负载*/
    double free_score = weights[0] * R_cpu + weights[1] * R_mem + weights[2] * R_net + weights[3] * R_disk;
    // weights用完后要及时释放--本质是释放指针所指向的内存空间，而不是删除指针本身
//    delete [] weights;
    node_load = 1.0 - free_score;
    cout << "节点综合负载：" << node_load << endl;

    node_info = {
            make_pair("node_id",1), make_pair("node_load",node_load),make_pair("cpu",R_cpu), make_pair("mem",R_mem),
            make_pair("disk",R_disk), make_pair("net",R_net), make_pair("job_num",0)
    };

    int node_id = 1;// 从数据库中读取节点id
    if (node_load > 0.95)
        sendAlarmToCenter(node_id, node_load);

    return node_info;// 然后发送给中心节点
};