#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "thread_pool.h"

#define BUF_SIZE 1024
#define SMALL_BUF 100

void request_handler(void* arg);
void send_data(FILE* fp, char* ct, char* file_name);
void error_handling(char* message);
void send_error(FILE* fp);

int main(int argc, char *argv[])
{
    // 创建线程池
    ThreadPool * pool = new ThreadPool(1, 100);

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_size;
    char buf[BUF_SIZE];
    pthread_t t_id;
    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");
    if(listen(serv_sock, 20)==-1)
        error_handling("listen() error");

    int times = 0;
    int n = 0;
    while(1)
    {
        clnt_adr_size=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, (socklen_t*)&clnt_adr_size);
        n++;
        printf("time %d Connection Request : %s:%d clnt_sock: %d\n",
               n,inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port),clnt_sock);
        //printf("before pthread_create\n");
        int* num = (int*)malloc(sizeof(int));
        *num = clnt_sock;
        // 总是addTask失败
        Task task;
        task.function = request_handler, task.arg = num;// 任务函数和参数
        pool->addTask(task);// 添加到任务队列
        printf("in server m_aliveNum: %d\n", pool->getAliveNumber());
        printf("in server m_busyNum: %d\n", pool->getBusyNumber());
        TaskQueue task_queue;
        printf("in server task num: %d\n", task_queue.taskNumber());

//        pthread_create(&t_id, NULL, request_handler, (socklen_t*)&clnt_sock);
//        // 非阻塞--线程结束后自动释放资源
//        pthread_detach(t_id);
//        //printf("after pthread_create\n");
    }

    close(serv_sock);
    return 0;
}

void request_handler(void *arg)
{
    int clnt_sock = *((int*)arg);
    printf("clnt_sock: %d\n",clnt_sock);
    char buf[SMALL_BUF];
    int str_len;

    while (1) {
        str_len = read(clnt_sock, buf, BUF_SIZE);

        if (str_len == 0) {
            close(clnt_sock);
            printf("closed client: %d \n", clnt_sock);
            return ;
        } else {
            buf[str_len] = 0;
            printf("From client %d:%s\n", clnt_sock,buf);
            write(clnt_sock, buf, str_len);    // echo!
        }
    }

    return 0;
}

void send_data(FILE* fp, char* ct, char* file_name)
{
    // status line
    char protocol[]="HTTP/1.0 200 OK\r\n";

    // message header
    char server[]="Server:Linux Web Server \r\n";
    char cnt_len[]="Content-length:2048\r\n";
    char cnt_type[SMALL_BUF];// content type

    char buf[BUF_SIZE];
    // message body--a html file
    FILE* send_file;

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    printf("send-cnt_type: %s",cnt_type);
    send_file=fopen(file_name, "r");
    if(send_file==NULL)
    {
        send_error(fp);
        return;
    }

    /*?????*/
    // ????
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    /*??????*/
    while(fgets(buf, BUF_SIZE, send_file)!=NULL)// ?sendfile?????????buf?
    {
        fputs(buf, fp);// ?buf????????fp
        fflush(fp);// fflush(fp)?????
    }
    fflush(fp);
    fclose(fp);
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void send_error(FILE* fp)
{
    char protocol[]="HTTP/1.0 400 Bad Request\r\n";
    char server[]="Server:Linux Web Server \r\n";
    char cnt_len[]="Content-length:2048\r\n";
    char cnt_type[]="Content-type:text/html\r\n\r\n";
    char content[]="<html><head><title>NETWORK</title></head>"
                   "<body><font size=+5><br>���� �߻�! ��û ���ϸ� �� ��û ��� Ȯ��!"
                   "</font></body></html>";

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fflush(fp);
}