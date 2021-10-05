#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void sendData(char * data) {
    int sock;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_addr;

    FILE * readfp;
    FILE * writefp;

    sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    char ip[] = "127.0.0.1";
    serv_addr.sin_addr.s_addr=inet_addr(ip);
    char port[] = "9999";// 端口写死--容易端口占用
    serv_addr.sin_port=htons(atoi(port));

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //printf("connected to server!\n");
    readfp=fdopen(sock, "r");
    writefp=fdopen(sock, "w");

    // 发送数据
    fputs(data, writefp);
    printf("数据发送完毕\n");
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);
}