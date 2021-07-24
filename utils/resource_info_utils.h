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
    // 要不要把这两个cpu_stat作为形参？
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;

    //第一次获取cpu使用情况
    getCPUOccupy((CPU_OCCUPY *) &cpu_stat1);

    // sleep100毫秒
    // 100000也可以作为形参
    long long sleep_time = 100000;
    usleep(sleep_time);

    //第二次获取cpu使用情况
    getCPUOccupy((CPU_OCCUPY *) &cpu_stat2);
    //计算cpu使用率
    return calCPUInfoBase((CPU_OCCUPY *) &cpu_stat1, (CPU_OCCUPY *) &cpu_stat2);
}