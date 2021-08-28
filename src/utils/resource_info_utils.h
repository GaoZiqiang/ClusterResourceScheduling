/*************************************************
Copyright:gaoziqianghi@163.com
Author:gaoziqianghi@163.com
Date:2021-07-20
Description:计算各资源的使用、空闲情况
**************************************************/
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

#ifndef RESOURCE_INFO_UTILS_H
#define RESOURCE_INFO_UTILS_H

#define BUFFER_SIZE 256 // 缓冲区

typedef long clock_t;

using namespace std;

/*磁盘信息*/
/*自定义SysDiskInfo结构体*/
typedef struct SysDiskInfo {
    double sys_disk_total;
    double sys_disk_free;
};

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
};


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

/*************************************************
Function:calDiskInfo
Description:计算节点的磁盘使用情况，包括空闲率和使用率

Input@fileName:操作系统的文件子系统使用配置文件 /proc/mtab
Return@disk_free_percent:节点的磁盘空闲率
*************************************************/
double calDiskInfo() {
    cout << "calDiskInfo time: " << clock() << endl;
    SysDiskInfo *sys_disk_info = (SysDiskInfo *)malloc(sizeof(SysDiskInfo));
    DiskInfo		*disk_info;
    struct statfs	fs_info;// 储存文件系统相关的信息
    struct mntent	*mnt_info;
    FILE			*fh;
    float			percent;
    unsigned long	sum_total = 0, total_free = 0;

    if ((fh = setmntent("/etc/mtab", "r")) == NULL) {
        printf("Cannot open /etc/mtab file!:%s\n", strerror(errno));
        return 0.0;
    }
//    else {
//        printf("\nopen success!");
//    }

    while (mnt_info = getmntent(fh)) {
        if (statfs(mnt_info->mnt_dir, &fs_info) < 0) {
            continue;
        }

        if (strcmp(mnt_info->mnt_type, "proc") ||
            strcmp(mnt_info->mnt_type, "devfs") ||
            strcmp(mnt_info->mnt_type, "usbfs") ||
            strcmp(mnt_info->mnt_type, "sysfs") ||
            strcmp(mnt_info->mnt_type, "tmpfs") ||
            strcmp(mnt_info->mnt_type, "devpts") ||
            strcmp(mnt_info->mnt_type, "fusectl") ||
            strcmp(mnt_info->mnt_type, "debugfs") ||
            strcmp(mnt_info->mnt_type, "binfmt_misc") ||
            strcmp(mnt_info->mnt_type, "fuse.gvfsd-fuse") ||
            strcmp(mnt_info->mnt_type, "securityfs") ||
            strcmp(mnt_info->mnt_type, "udev")) {
            if (fs_info.f_blocks != 0) {

                /*此处percent的计算方式存疑*/
                percent = ((float)fs_info.f_blocks - (float)fs_info.f_bfree) * 100 / ((float)fs_info.f_blocks - (float)fs_info.f_bfree + fs_info.f_bavail) + 1;// f_bfree:硬盘所有剩余空间
                // percent = (1.0 - ((float)fs_info.f_bfree / (float)fs_info.f_blocks));
            } else {
                /*使用率为0*/
                percent = 0;
            }
        } else {
            continue;
        }

        /*创建DiskInfo节点*/
        if ((disk_info = (DiskInfo *)malloc(sizeof(DiskInfo))) == NULL) {
            continue;
        }

        /*disk_info->type，防止空指针（待优化）*/
        disk_info->type = mnt_info->mnt_type;
        strcpy(disk_info->type, mnt_info->mnt_type);

        disk_info->device = mnt_info->mnt_fsname;
        strcpy(disk_info->device, mnt_info->mnt_fsname);

        disk_info->mntpnt = mnt_info->mnt_dir;
        strcpy(disk_info->mntpnt, mnt_info->mnt_dir);

        unsigned long block_size = fs_info.f_bsize/1024;
        disk_info->blocks = fs_info.f_blocks * block_size / 1024;
        disk_info->bfree = fs_info.f_bfree * block_size / 1024;
        disk_info->availiable_disk = fs_info.f_bavail * block_size / 1024;
        disk_info->bused = (fs_info.f_blocks - fs_info.f_bfree) * block_size / 1024;
        disk_info->bused_percent = percent;

        sum_total += disk_info->blocks;
        total_free += disk_info->availiable_disk;

        /*清除辅助空间*/
        free(disk_info);
        disk_info = NULL;
    }

    /*最终结果：Disk_Total和Disk_Free*/
    double disk_total = sys_disk_info->sys_disk_total = sum_total / 1024;
    double disk_free = sys_disk_info->sys_disk_free = total_free / 1024;

//    printf("sys_disk_info->sys_disk_total: %f\n",disk_total);
//    printf("sys_disk_info->sys_disk_free: %f\n",disk_free);

    double disk_free_percent = disk_free / disk_total;
    printf("磁盘空闲率: %f\n", disk_free_percent);

    return disk_free_percent;
    // 优化
    // return (disk_free / disk_total);
}


/*************************************************
Function:getMemOccupy
Description:获取节点的内存资源使用情况

Input@fileName:操作系统的使用配置文件 /proc/meminfo
Return@void:
*************************************************/
void getMemOccupy(MEM_OCCUPY *mem) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
    char buff[256];
    MEM_OCCUPY *m;
    m = mem;

    fd = fopen("/proc/meminfo", "r");
    //MemTotal: 515164 kB
    //MemFree: 7348 kB
    //Buffers: 7892 kB
    //Cached: 241852  kB
    //SwapCached: 0 kB
    //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m->name1, &m->MemTotal);// 取出文件中前面两个数据分别赋值给m->name1和m->MemTotal
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m->name2, &m->MemFree);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m->name3, &m->Buffers);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", m->name4, &m->Cached);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu", m->name5, &m->SwapCached);

    fclose(fd);     //关闭文件fd
}

/*************************************************
Function:getCPUOccupy
Description:获取节点的CPU资源使用情况

Input@fileName:操作系统的使用配置文件 /proc/meminfo
Return@void:
*************************************************/
void getCPUOccupy(CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
    char buff[256];
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy = cpust;

    fd = fopen("/proc/stat", "r");
    fgets(buff, sizeof(buff), fd);

    sscanf(buff, "%s %u %u %u %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice, &cpu_occupy->system, &cpu_occupy->idle, &cpu_occupy->lowait, &cpu_occupy->irq, &cpu_occupy->softirq);


    fclose(fd);
}

/*加一个参数：MEM_OCCUPY m;*/
/*************************************************
Function:calMemInfo
Description:计算节点的内存使用率和空闲率

Input@MEM_OCCUPY m:内存资源使用情况
Return@mem_free_percent_user:从应用程序角度看的内存空闲率
*************************************************/
double calMemInfo() {
    cout << "calMemInfo time: " << clock() << endl;
    //获取内存
    //(MemTotal - MemFree)/ MemTotal

    MEM_OCCUPY m;
    getMemOccupy((MEM_OCCUPY *) &m);

    // 计算内存空闲率
    /*从应用程序的角度*/
    double mem_free_percent_user = (m.MemFree + m.Buffers + m.Cached) * 1.0 / (m.MemTotal * 1.0  );
    /*从操作系统的角度*/
    double mem_free_percent_os = m.MemFree * 1.0 / (m.MemTotal * 1.0  );
    printf("内存空闲率_应用程序角度：%.3f\n", mem_free_percent_user);
    printf("内存空闲率_操作系统角度：%.3f\n", mem_free_percent_os);

    return mem_free_percent_user;// 从应用程序角度
}


/*计算单次cpu空闲率*/
/*************************************************
Function:calCPUInfoBase
Description:计算节点的单次CPU使用率和空闲率

Input@CPU_OCCUPY *o & CPU_OCCUPY *n:CPU资源使用情况
Return@cpu_free_percent:CPU空闲率_单次
*************************************************/
double calCPUInfoBase(CPU_OCCUPY *o, CPU_OCCUPY *n)
{
    unsigned long od, nd;
    double cpu_use_percent = 0;

    //old?
    od = (unsigned long)(o->user + o->nice + o->system + o->idle + o->lowait + o->irq + o->softirq);//第一次(用户+优先级+系统+空闲)的时间再赋给od
    //new?
    nd = (unsigned long)(n->user + n->nice + n->system + n->idle + n->lowait + n->irq + n->softirq);//第二次(用户+优先级+系统+空闲)的时间再赋给od
    double sum = nd - od;
    double idle = n->idle - o->idle;
    // cpu_use_percent = idle / sum;
    idle = n->user + n->system + n->nice - o->user - o->system - o->nice;
    cpu_use_percent = idle / sum;
//    printf("CPU利用率：%.3f\n",cpu_use_percent);
    double cpu_free_percent = 1.0 - cpu_use_percent;
    printf("CPU空闲率：%.3f\n", 1.0 - cpu_use_percent);

    return cpu_free_percent;
}

// 参数问题待优化

/*************************************************
Function:calCPUInfo
Description:计算节点的CPU使用率和空闲率

Input@CPU_OCCUPY cpu_stat1 & CPU_OCCUPY cpu_stat2:CPU资源使用情况
Return@cpu_free_percent:CPU空闲率
*************************************************/
double calCPUInfo() {
    cout << "calCPUInfo time: " << clock() << endl;
    cout << endl;
    // 要不要把这两个cpu_stat作为形参？
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;

    //第一次获取cpu使用情况
    getCPUOccupy((CPU_OCCUPY *) &cpu_stat1);

    // sleep100毫秒
    // 100000也可以作为形参
    long long sleep_time = 1000000;
    usleep(sleep_time);

    //第二次获取cpu使用情况
    getCPUOccupy((CPU_OCCUPY *) &cpu_stat2);
    //计算cpu使用率
    return calCPUInfoBase((CPU_OCCUPY *) &cpu_stat1, (CPU_OCCUPY *) &cpu_stat2);
}

/*************************************************
Function:getNetworkOccupy
Description:获取节点的网络资源使用情况--接收/发送字节总数

Input@fileName:操作系统的使用配置文件 /proc/net/dev
Return@itemResults:系统至今（当前）接收/发送字节总数
*************************************************/
long long* getNetworkOccupy() {
    FILE *stream;
    char buffer[BUFFER_SIZE];//缓冲区大小
    char *line_return;//记录每次返回值（行）
    int line_count = 0;//记录行数
    char tmp_itemName[32];//临时存放文件中的每行的项目名称
    long long itemReceive;//存放每一个网卡的接受到的字节总数（单位：Byte）
    long long itemTransmit;//存放每一个网卡的已发送的字节总数（单位：Byte）

    /*打开/proc/net/dev配置文件*/
    if ((stream =fopen("/proc/net/dev","r")) == NULL) {
        printf("Cannot open /proc/net/dev file!:%s\n",strerror(errno));
        return 0;
    }

    /*先读出前两行*//*fgets()函数是按照顺序一次读取的，且处读出一行，缓冲区中就少一行*/
    /*第一行*/
    line_return =fgets (buffer, BUFFER_SIZE *sizeof(char), stream);//读取第一行

    /*第二行*/
    line_return =fgets (buffer, BUFFER_SIZE *sizeof(char), stream);//读取第二行

    long long itemReceives = 0;// 切记：一定要初始化！！！
    long long itemTransmits = 0;
    while(line_return != NULL){
        line_return =fgets (buffer, BUFFER_SIZE *sizeof(char), stream);
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
        itemReceives += itemReceive;
        itemTransmits += itemTransmit;
    }

    long long* itemResults = new long long[2];
    itemResults[0] = itemReceives;
    itemResults[1] = itemTransmits;

    return itemResults;
}

/*************************************************
Function:calNetworkInfo
Description:计算节点的网络带宽使用率和空闲率

Input@xxx:一段时间间隔
Return@net_free_percent:网络带宽空闲率
*************************************************/
double calNetworkInfo() {
    cout << "calNetworkInfo time: " << clock() << endl;
    /*网络带宽,Mbps：每秒传输百万位*/
    static float totalBandWidth = 1000;

    /*第一次采集流量数据*/
    clock_t startTime = clock();

    long long *itemResults1;
    itemResults1 = getNetworkOccupy();
    long long inSize1 = itemResults1[0], outSize1 = itemResults1[1];

    long long sleep_time = 1000000;// 延迟时间：1000毫秒
    usleep(sleep_time);

    /*第二次采集流量数据*/
    clock_t endTime = clock();// 时间单位为毫秒
    long long *itemResults2;
    itemResults2 = getNetworkOccupy();

    long long inSize2 = itemResults2[0], outSize2 = itemResults2[1];

    /*时间单位换算成秒*/
    double interval = (double)(endTime - startTime) / CLOCKS_PER_SEC;// endTime - startTime为滴答数 CLOCKS_PER_SEC = 1000000
    double sizes = (double)(inSize2 - inSize1 + outSize2 - outSize1);
    /*计算网速，单位为MiB/s*/
    double avg_rate = (double)(((long long)(inSize2 + outSize2 - inSize1 - outSize1) * 8) / (1000000 * interval));

    // printf("avg_rate:%f\n", avg_rate);
    double net_usage = avg_rate / totalBandWidth;

    // printf("net_usage:%f\n", net_usage);

    double net_free_percent = 1.0 - net_usage;
    printf("网络带宽空闲率:%f\n",net_free_percent);
//    float totalTime = endTime - startTime;

    return net_free_percent;
}
#endif