/*************************************************
Copyright:gaoziqianghi@163.com
Author:gaoziqianghi@163.com
Date:2021-07-20
Description:计算各资源的使用、空闲情况
**************************************************/

#ifndef RESOURCE_INFO_UTILS_H
#define RESOURCE_INFO_UTILS_H

#include <sys/statfs.h>
#include <mntent.h>
#include <string>
#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/vfs.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

using namespace std;

namespace resource {
    #define BUFFER_SIZE 256 // 缓冲区
    typedef long clock_t;

/*磁盘信息*/
/*自定义SysDiskInfo结构体*/
    typedef struct SysDiskInfo {
        double sys_disk_total;
        double sys_disk_free;
    }SysDiskInfo;

/*自定义DiskInfo*/
    typedef struct DiskInfo {
        char *type;
        char *device;
        char *mntpnt;
        unsigned long blocks;
        unsigned long bfree;
        unsigned long availiable_disk;
        unsigned long bused;
        float bused_percent;
    }DiskInfo;

/*CPU和内存*/
/*mem occupy的结构体*/
    typedef struct MEMPACKED
    {
        char name1[20];      //定义一个char类型的数组名name有20个元素
        unsigned long MemTotal;
        char name2[20];
        unsigned long MemFree;
        char name3[20];
        unsigned long Buffers;
        char name4[20];
        unsigned long Cached;
        char name5[20];
        unsigned long SwapCached;
    }MEM_OCCUPY;

/*cpu occupy的结构体*/
    typedef struct CPUPACKED
    {
        char name[20];      //定义一个char类型的数组名name有20个元素
        unsigned int user; //定义一个无符号的int类型的user
        unsigned int nice; //定义一个无符号的int类型的nice
        unsigned int system;//定义一个无符号的int类型的system
        unsigned int idle; //定义一个无符号的int类型的idle
        unsigned int lowait;
        unsigned int irq;
        unsigned int softirq;
    }CPU_OCCUPY;

    /*计算磁盘使用率*/
    double calDiskInfo();
    /*计算内存使用率*/
    void getMemOccupy(MEM_OCCUPY *mem);
    double calMemInfo();
    /*计算CPU使用率*/
    void getCPUOccupy(CPU_OCCUPY *cpust);
    double calCPUInfoBase(CPU_OCCUPY *o, CPU_OCCUPY *n);
    double calCPUInfo();
    /*计算网络带宽使用率*/
    long long* getNetworkOccupy();
    double calNetworkInfo();
}
#endif