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

//using namespace std;

/*自定义*/
// 用不着了
//typedef struct mntent {
//    char *mnt_fsname; /* 挂载的文件系统的名字 */
//    char *mnt_dir; /* 挂载点 */
//    char *mnt_type; /* 文件系统类型：ufs、nfs 等 */
//    char *mnt_opts; /* 选项，以逗号为分隔符 */
//    int mnt_freq; /* Dump 的频率(以天为单位) */
//    int mnt_passno; /* fsck检查的次序 */
//};

/*自定义SysDiskInfo*/
typedef struct SysDiskInfo {
    double Disk_Total;
    double Disk_Free;
};

/*自定义DiskInfo*/
// 这是关键！！！
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
//
    if ((fh = setmntent("/etc/mtab", "r")) == NULL) {
        printf("Cannot open /etc/mtab file!:%s\n", strerror(errno));
        return 0.0;
    }else {
        // fh = setmntent("/etc/mtab", "r");
        printf("\nopen success!");
    }
//

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
    printf("free_percent: %f\n",free_percent);

    return free_percent;
}

int main() {
    GetHostDiskInfo();
//    printf("------ START ------");
//    SysDiskInfo *sys_disk_info = (SysDiskInfo *)malloc(sizeof(SysDiskInfo));
//    DiskInfo		*disk_info;
//    struct statfs	fs_info;// 储存文件系统相关的信息
//    struct mntent	*mnt_info;
//    FILE			*fh;
//    float			percent;
//    unsigned long	sum_Total = 0, total_free = 0;
////
//    if ((fh = setmntent("/etc/mtab", "r")) == NULL) {
//        printf("Cannot open /etc/mtab file!:%s\n", strerror(errno));
//        return 0;
//    }else {
//        // fh = setmntent("/etc/mtab", "r");
//        printf("\nopen success!");
//    }
////
//
//    // 以上都没问题
//    printf("\nNow after setmntent\n");
//
//
//    // 下面进行测试
//    // mnt_info = getmntent(setmntent("/etc/mtab", "r"));
//
//    // struct mntent *m;
//    int i = 0;
//    while (mnt_info = getmntent(fh)) {
//        i += 1;//read next line
//        printf("--- 第 %d 轮 ---\n",i);
//        // printf("result:%d\n",statfs(mnt_info->mnt_dir, &fs_info));
//
//        if (statfs(mnt_info->mnt_dir, &fs_info) < 0) {
//            printf("statfs(mnt_info->mnt_dir, &fs_info) < 0");
//            continue;
//        }
//
////        if ((disk_info = (DiskInfo *)malloc(sizeof(DiskInfo))) == NULL) {
////            printf("Now in DiskInfo");
////            continue;
////        }
//
//
//
//
//        // printf("%d 问题出在这里",i);
//        // strcmp(mnt_info->mnt_type, "fuse.gvfs_fuse_daemon")中fuse.gvfs_fuse_daemon改成fuse.gvfsd-fuse
//        if (strcmp(mnt_info->mnt_type, "proc") ||
//            strcmp(mnt_info->mnt_type, "devfs") ||
//            strcmp(mnt_info->mnt_type, "usbfs") ||
//            strcmp(mnt_info->mnt_type, "sysfs") ||
//            strcmp(mnt_info->mnt_type, "tmpfs") ||
//            strcmp(mnt_info->mnt_type, "devpts") ||
//            strcmp(mnt_info->mnt_type, "fusectl") ||
//            strcmp(mnt_info->mnt_type, "debugfs") ||
//            strcmp(mnt_info->mnt_type, "binfmt_misc") ||
//            strcmp(mnt_info->mnt_type, "fuse.gvfsd-fuse") ||
//            strcmp(mnt_info->mnt_type, "securityfs") ||
//            strcmp(mnt_info->mnt_type, "udev")) {
//            if (fs_info.f_blocks != 0) {
//                // 问题出在这里
//                // printf("%d 问题出在这里",i);
//                // 百分比的计算方法有问题！！！
//                percent = ((float)fs_info.f_blocks - (float)fs_info.f_bfree) * 100 / ((float)fs_info.f_blocks - (float)fs_info.f_bfree + fs_info.f_bavail) + 1;
//                // 这是使用率
//                //f_bfree:硬盘所有剩余空间
//                // percent = (1.0 - ((float)fs_info.f_bfree / (float)fs_info.f_blocks));
//                printf("%d percent: %f\n",i,percent);
//            } else {
//                // 使用率为0
//                percent = 0;
//                printf("%d fs_info.f_blocks = 0!!! percent: %f\n",i,percent);
//            }
//        } else {
//            printf("continue %d\n",i);
//            continue;
//        }
//
//        // 这里出问题了!!!
//
//        if ((disk_info = (DiskInfo *)malloc(sizeof(DiskInfo))) == NULL) {
//            printf("Now in DiskInfo");
//            continue;
//        }
//        // disk_info->type = "gao";
//        // printf("disk_info->type: %s\n",disk_info->type);
////        printf("%d mnt_info->mnt_type %s\n",i,mnt_info->mnt_type);
//        disk_info->type = mnt_info->mnt_type;
//        printf("%d 浅赋值=的disk_info->type: %s\n",i,disk_info->type);
//        strcpy(disk_info->type, mnt_info->mnt_type);
//        printf("%d 深赋值strcpy的disk_info->type: %s\n",i,disk_info->type);
//        printf("type %d 成功\n",i);
//
//        disk_info->device = mnt_info->mnt_fsname;
//        strcpy(disk_info->device, mnt_info->mnt_fsname);
//        printf("device %d 成功\n",i);
//
//        disk_info->mntpnt = mnt_info->mnt_dir;
//        strcpy(disk_info->mntpnt, mnt_info->mnt_dir);
//        printf("mntpnt %d 成功\n",i);
//
//        unsigned long block_size = fs_info.f_bsize/1024;
//        disk_info->blocks = fs_info.f_blocks * block_size / 1024;
//        disk_info->bfree = fs_info.f_bfree * block_size / 1024;
//        disk_info->availiable_disk = fs_info.f_bavail * block_size / 1024;
//        disk_info->bused = (fs_info.f_blocks - fs_info.f_bfree) * block_size / 1024;
//        disk_info->bused_percent = percent;
//
//        sum_Total += disk_info->blocks;
//        total_free += disk_info->availiable_disk;
//
//        /* clean auxiliary memory */
//        free(disk_info);
//        disk_info = NULL;
//
//        printf("--- 第 %d 轮结束 ---\n\n",i);
//        // printf("Drive %s, name %s,type  %s,opt  %s\n", mnt_info->mnt_dir, mnt_info->mnt_fsname,mnt_info->mnt_type,mnt_info->mnt_opts );
//    }
//
//    /*最终结果：Disk_Total和Disk_Free*/
//    sys_disk_info->Disk_Total = sum_Total/1024;
//    sys_disk_info->Disk_Free = total_free/1024;
//
//    printf("sys_disk_info->Disk_Total: %f\n",sys_disk_info->Disk_Total);
//    printf("sys_disk_info->Disk_Free: %f\n",sys_disk_info->Disk_Free);

    return 0;
}