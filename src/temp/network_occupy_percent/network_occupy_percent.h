#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 256

long long* getNetworkOccupy() {
    FILE *stream;
    char buffer[BUFFER_SIZE];//缓冲区大小
    char *line_return;//记录每次返回值（行）
    int line_count = 0;//记录行数
    char tmp_itemName[32];//临时存放文件中的每行的项目名称
    long long itemReceive;//存放每一个网卡的接受到的字节总数（单位：Byte）
    long long itemTransmit;//存放每一个网卡的已发送的字节总数（单位：Byte）

    // system("cat /proc/net/dev");
    // printf("**************************************************************************************************************");

    /*打开/proc/net/dev配置文件*/
    // stream =fopen("/proc/net/dev","r");

    /*打开/proc/net/dev配置文件*/
    if ((stream =fopen("/proc/net/dev","r")) == NULL) {
        printf("Cannot open /proc/net/dev file!:%s\n",strerror(errno));
        return 0;
    }

    /*先读出前两行*//*fgets()函数是按照顺序一次读取的，且处读出一行，缓冲区中就少一行*/
    /*第一行*/
    line_return =fgets (buffer, BUFFER_SIZE *sizeof(char), stream);//读取第一行
    line_count++;
//    printf("\n[MAIN] line %d: %s\n", line_count, line_return);//输出第一行

    /*第二行*/
    line_return =fgets (buffer, BUFFER_SIZE *sizeof(char), stream);//读取第二行
    line_count++;
//    printf("\n[MAIN] line %d: %s\n", line_count, line_return);//输出第二行

//    printf("**************************************************************************************************************");

    long long itemReceives;
    long long itemTransmits;
    while(line_return != NULL){
        line_return =fgets (buffer, BUFFER_SIZE *sizeof(char), stream);
        line_count++;
//        printf("\n[MAIN] line %d: %s", line_count, line_return);
        // 这里看不懂
        sscanf( buffer,
                "%s%lld%lld%lld%lld%lld%lld%lld%lld%lld",
                tmp_itemName,// 该行第一列为网卡名
                &itemReceive,
                &itemTransmit,
                &itemTransmit,
                &itemTransmit,
                &itemTransmit,
                &itemTransmit,
                &itemTransmit,
                &itemTransmit,
                &itemTransmit);// itemTransmit会不停的重复赋值（不停覆盖），最终得到的itemTransmit就是实际的itemTransmit，不失为一种好办法
//        printf("line %d: itemReceive: %lld\n", line_count, itemReceive);
        itemReceives += itemReceive;
//        printf("line %d:itemTransmit: %lld\n\n", line_count, itemTransmit);
        itemTransmits += itemTransmit;
    }

    long long* itemResults = new long long[2];
    itemResults[0] = itemReceives;
    itemResults[1] = itemTransmits;
    printf("itemReceives:%lld\n",itemReceives);
    printf("itemTransmits:%lld\n",itemTransmits);

    return itemResults;
}

void calNetworkInfo() {
    // 网络带宽,Mbps
    static float totalBandWidth = 1000;

    /*第一次采集流量数据*/
    float startTime = clock();


    long long *itemResults1;
    itemResults1 = getNetworkOccupy();
    long long inSize1 = itemResults1[0];
    printf("inSize1:%lld\n",inSize1);
    long long outSize1 = itemResults1[1];
    printf("outSize1:%lld\n",outSize1);


    long long sleep_time = 1000;
    usleep(sleep_time);

    /*第二次采集流量数据*/
    long endTime = clock();
    long long *itemResults2;
    itemResults2 = getNetworkOccupy();
    long long inSize2 = itemResults1[0];
    printf("inSize2:%lld\n",inSize2);
    long long outSize2 = itemResults1[1];
    printf("outSize2:%lld\n",outSize2);

    float interval = (float)(endTime - startTime)/1000;
    printf("interval:%f\n",interval);
    float sizes = (float)(inSize2 - inSize1 + outSize2 - outSize1);
    printf("sizes:%f\n",sizes);
    float curRate = (float)((long long)(inSize2 - inSize1 + outSize2 - outSize1)*8/(1000000*interval));

    float netUsage = curRate / totalBandWidth;

    printf("netUsage:%f",netUsage);
//    float totalTime = endTime - startTime;


}