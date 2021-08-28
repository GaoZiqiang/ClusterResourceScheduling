#include <string>
#include <vector>
#include <iostream>// std::cout
#include <utility>// std::pair

using namespace std;

int main() {
    // 参数形式:std::vector<std::pair>
    // 存入redis--存储格式:std::vector<std::pair>
    // 一个数组代表一个节点
    vector<pair<string,double>> node_id_info;
    node_id_info = {make_pair("node_id",1), make_pair("node_load",0.36),make_pair("cpu",0.69), make_pair("mem",0.88),
                    make_pair("disk",0.56), make_pair("net",0.88)};
    cout << "node_id_info:\n" << node_id_info[0].first << ": " << node_id_info[0].second << "\n" <<
         node_id_info[1].first << ": " << node_id_info[1].second << endl;
    // 遍历输出
    for (int i = 0; i < node_id_info.size(); i++)
        cout << node_id_info[i].first << ": " << node_id_info[i].second << " ";

    // 获取node id--直接用string--node_id_info
    // 获取其他指标和值--封装存入函数
    // redis.setHash("nodes:4",field1,"0.77",field2,"2");


}

void storeIntoRedis(std::vector<std::pair<string, double>> node_info) {
    // 代码太丑了！！！
    // node_id
    string key_node_id = node_info[0].first;// node_id
    double value_node_id = node_info[0].second;// node load

    // resource keys
    string key_load = node_info[1].first, key_cpu = node_info[2].first, key_mem = node_info[3].first,
    key_disk = node_info[4].first, key_net = node_info[5].first;
    // values
    double value_load = node_info[1].second, value_cpu = node_info[2].second, value_mem = node_info[3].second,
    value_disk = node_info[4].second, value_net = node_info[5].second;

    // node_id转int-->string
    int node_id = int(value_node_id);
    redis.setHash(to_string(node_id),key_load,value_load, key_cpu,value_cpu, key_mem,value_mem,
                  key_disk,value_disk, key_net,value_net);
}