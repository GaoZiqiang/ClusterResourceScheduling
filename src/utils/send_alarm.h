#include "stdio.h"

void sendAlarmToCenter(int node_id, double node_load) {
    printf("发送报警信息给中心节点,node_id: %d,node_load: %0.4f!\n",node_id, node_load);
    // 通过通信模块将信息发送给中心节点
}