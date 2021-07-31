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

/*自定义SysDiskInfo*/
typedef struct SysDiskInfo {
    double Disk_Total;
    double Disk_Free;
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

double calDiskInfo() {
    //    GetHostDiskInfo();
    printf("------ START ------");
    SysDiskInfo *sys_disk_info = (SysDiskInfo *)malloc(sizeof(SysDiskInfo));
    DiskInfo		*disk_info;
    struct statfs	fs_info;// 储存文件系统相关的信息
    struct mntent	*mnt_info;
    FILE			*fh;
    float			percent;
    unsigned long	sum_Total = 0, total_free = 0;

    if ((fh = setmntent("/etc/mtab", "r")) == NULL) {
        printf("Cannot open /etc/mtab file!:%s\n", strerror(errno));
        return 0.0;
    }
//    else {
//        printf("\nopen success!");
//    }

    // 以上都没问题
    printf("\nNow after setmntent\n");


    // 下面进行测试
    // mnt_info = getmntent(setmntent("/etc/mtab", "r"));

    // struct mntent *m;
    int i = 0;
    while (mnt_info = getmntent(fh)) {
        i += 1;//read next line
        printf("--- 第 %d 轮 ---\n",i);
        // printf("result:%d\n",statfs(mnt_info->mnt_dir, &fs_info));

        if (statfs(mnt_info->mnt_dir, &fs_info) < 0) {
            printf("statfs(mnt_info->mnt_dir, &fs_info) < 0");
            continue;
        }

//        if ((disk_info = (DiskInfo *)malloc(sizeof(DiskInfo))) == NULL) {
//            printf("Now in DiskInfo");
//            continue;
//        }




        // printf("%d 问题出在这里",i);
        // strcmp(mnt_info->mnt_type, "fuse.gvfs_fuse_daemon")中fuse.gvfs_fuse_daemon改成fuse.gvfsd-fuse
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
                // 问题出在这里
                // printf("%d 问题出在这里",i);
                // 百分比的计算方法有问题！！！
                percent = ((float)fs_info.f_blocks - (float)fs_info.f_bfree) * 100 / ((float)fs_info.f_blocks - (float)fs_info.f_bfree + fs_info.f_bavail) + 1;
                // 这是使用率
                //f_bfree:硬盘所有剩余空间
                // percent = (1.0 - ((float)fs_info.f_bfree / (float)fs_info.f_blocks));
                printf("%d percent: %f\n",i,percent);
            } else {
                // 使用率为0
                percent = 0;
                printf("%d fs_info.f_blocks = 0!!! percent: %f\n",i,percent);
            }
        } else {
            printf("continue %d\n",i);
            continue;
        }

        // 这里出问题了!!!

        if ((disk_info = (DiskInfo *)malloc(sizeof(DiskInfo))) == NULL) {
            printf("Now in DiskInfo");
            continue;
        }
        // disk_info->type = "gao";
        // printf("disk_info->type: %s\n",disk_info->type);
//        printf("%d mnt_info->mnt_type %s\n",i,mnt_info->mnt_type);
        disk_info->type = mnt_info->mnt_type;
        printf("%d 浅赋值=的disk_info->type: %s\n",i,disk_info->type);
        strcpy(disk_info->type, mnt_info->mnt_type);
        printf("%d 深赋值strcpy的disk_info->type: %s\n",i,disk_info->type);
        printf("type %d 成功\n",i);

        disk_info->device = mnt_info->mnt_fsname;
        strcpy(disk_info->device, mnt_info->mnt_fsname);
        printf("device %d 成功\n",i);

        disk_info->mntpnt = mnt_info->mnt_dir;
        strcpy(disk_info->mntpnt, mnt_info->mnt_dir);
        printf("mntpnt %d 成功\n",i);

        unsigned long block_size = fs_info.f_bsize/1024;
        disk_info->blocks = fs_info.f_blocks * block_size / 1024;
        disk_info->bfree = fs_info.f_bfree * block_size / 1024;
        disk_info->availiable_disk = fs_info.f_bavail * block_size / 1024;
        disk_info->bused = (fs_info.f_blocks - fs_info.f_bfree) * block_size / 1024;
        disk_info->bused_percent = percent;

        sum_Total += disk_info->blocks;
        total_free += disk_info->availiable_disk;

        /* clean auxiliary memory */
        free(disk_info);
        disk_info = NULL;

        printf("--- 第 %d 轮结束 ---\n\n",i);
        // printf("Drive %s, name %s,type  %s,opt  %s\n", mnt_info->mnt_dir, mnt_info->mnt_fsname,mnt_info->mnt_type,mnt_info->mnt_opts );
    }

    /*最终结果：Disk_Total和Disk_Free*/
    double disk_Total = sys_disk_info->Disk_Total = sum_Total/1024;
    double disk_Free = sys_disk_info->Disk_Free = total_free/1024;

    printf("sys_disk_info->Disk_Total: %f\n",disk_Total);
    printf("sys_disk_info->Disk_Free: %f\n",disk_Free);

    double free_percent = disk_Free / disk_Total;
    printf("磁盘空闲率: %f\n",free_percent);

    return free_percent;
}


/*CPU和内存*/
typedef struct MEMPACKED         //定义一个mem occupy的结构体
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

typedef struct CPUPACKED         //定义一个cpu occupy的结构体
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


void get_memoccupy(MEM_OCCUPY *mem) //对无类型get函数含有一个形参结构体类弄的指针O
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


int get_cpuoccupy(CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;
    char buff[256];
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy = cpust;

    fd = fopen("/proc/stat", "r");
    fgets(buff, sizeof(buff), fd);

    sscanf(buff, "%s %u %u %u %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice, &cpu_occupy->system, &cpu_occupy->idle, &cpu_occupy->lowait, &cpu_occupy->irq, &cpu_occupy->softirq);


    fclose(fd);

    return 0;
}

/*计算单次cpu空闲率*/
double cal_cpuoccupy_base(CPU_OCCUPY *o, CPU_OCCUPY *n)
{
    unsigned long od, nd;
    double cpu_use = 0;

    //old?
    od = (unsigned long)(o->user + o->nice + o->system + o->idle + o->lowait + o->irq + o->softirq);//第一次(用户+优先级+系统+空闲)的时间再赋给od
    //new?
    nd = (unsigned long)(n->user + n->nice + n->system + n->idle + n->lowait + n->irq + n->softirq);//第二次(用户+优先级+系统+空闲)的时间再赋给od
    double sum = nd - od;
    double idle = n->idle - o->idle;
    cpu_use = idle / sum;
    idle = n->user + n->system + n->nice - o->user - o->system - o->nice;
    cpu_use = idle / sum;
//    printf("CPU利用率：%.3f\n",cpu_use);
    double cpu_free_ratio = 1.0 - cpu_use;
    printf("CPU空闲率：%.3f\n",1.0 - cpu_use);

    return cpu_free_ratio;
}

double cal_cpuoccupy() {
    // 要不要把这两个cpu_stat作为形参？
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;

    //第一次获取cpu使用情况
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);

    // sleep100毫秒
    // 100000也可以作为形参
    long long sleep_time = 100000;
    usleep(sleep_time);

    //第二次获取cpu使用情况
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);
    //计算cpu使用率
    return cal_cpuoccupy_base((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
}


/*加一个参数：MEM_OCCUPY m;*/
double cal_memoccupy() {
    //获取内存
    //(MemTotal - MemFree)/ MemTotal

    MEM_OCCUPY m;
    get_memoccupy((MEM_OCCUPY *)&m);

    // 计算内存空闲率
    /*从应用程序的角度*/
    double mem_free_ratio_user = (m.MemFree + m.Buffers + m.Cached) * 1.0 / ( m.MemTotal * 1.0  );
    /*从操作系统的角度*/
    double mem_free_ratio_os = m.MemFree * 1.0 / ( m.MemTotal * 1.0  );
    printf("内存空闲率_应用程序角度：%.3f\n",  mem_free_ratio_user);
    printf("内存空闲率_操作系统角度：%.3f\n",  mem_free_ratio_os);

    return mem_free_ratio_user;// 从应用程序角度
}

//int main( int argc, char **argv )
//{
//    MEM_OCCUPY mem_stat;
//    //CPU_OCCUPY cpu_stat1;
//    //CPU_OCCUPY cpu_stat2;
//
//    cal_memoccupy(mem_stat);
//
//    //获取内存
//    //(MemTotal - MemFree)/ MemTotal
//    // get_memoccupy((MEM_OCCUPY *)&mem_stat);
//    //printf(" [MemTotal] = %lu \n [MemFree] = %lu \n [Buffers] = %lu \n [Cached] = %lu \n [SwapCached] = %lu \n", mem_stat.MemTotal, mem_stat.MemFree, mem_stat.Buffers, mem_stat.Cached, mem_stat.SwapCached);
//
//    // 计算内存空闲率
//    // printf("内存空闲率：%.3f\n", mem_stat.MemFree * 1.0 / ( mem_stat.MemTotal * 1.0  ) );
//
//
//    //第一次获取cpu使用情况
//    //get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
//
//    //usleep(100000);
//
//    //第二次获取cpu使用情况
//    //get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);
//    //计算cpu使用率
//    //cal_cpuoccupy_base((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
//
//    cal_cpuoccupy();
//    return 0;
//}


//int main() {
//    GetHostDiskInfo();
//}