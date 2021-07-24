#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <iostream>
#include <unistd.h>

using namespace std;

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



double cal_memoccupy(MEM_OCCUPY m) {
    //获取内存
    //(MemTotal - MemFree)/ MemTotal
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

int main( int argc, char **argv )
{
    MEM_OCCUPY mem_stat;
    //CPU_OCCUPY cpu_stat1;
    //CPU_OCCUPY cpu_stat2;

    cal_memoccupy(mem_stat);

    //获取内存
    //(MemTotal - MemFree)/ MemTotal
    // get_memoccupy((MEM_OCCUPY *)&mem_stat);
    //printf(" [MemTotal] = %lu \n [MemFree] = %lu \n [Buffers] = %lu \n [Cached] = %lu \n [SwapCached] = %lu \n", mem_stat.MemTotal, mem_stat.MemFree, mem_stat.Buffers, mem_stat.Cached, mem_stat.SwapCached);

    // 计算内存空闲率
    // printf("内存空闲率：%.3f\n", mem_stat.MemFree * 1.0 / ( mem_stat.MemTotal * 1.0  ) );


    //第一次获取cpu使用情况
    //get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);

    //usleep(100000);

    //第二次获取cpu使用情况
    //get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);
    //计算cpu使用率
    //cal_cpuoccupy_base((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);

    cal_cpuoccupy();
    return 0;
}