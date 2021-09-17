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
    char port[] = "9090";// 端口写死--容易端口占用
    serv_addr.sin_port=htons(atoi(port));

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("connected to server!\n");
    readfp=fdopen(sock, "r");
    writefp=fdopen(sock, "w");

//	while(1)
//	{
//		if(fgets(buf, sizeof(buf), readfp)==NULL)
//			break;
//		fputs(buf, stdout);
//		fflush(stdout);
//	 }

    // 发送信息
    fputs(data, writefp);
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);
}


int main(int argc, char *argv[])
{

    char data[] = "Hello server!\n";
    sendData(data);
//	int sock;
//	char buf[BUF_SIZE];
//	struct sockaddr_in serv_addr;
//
//	FILE * readfp;
//	FILE * writefp;
//
//	sock=socket(PF_INET, SOCK_STREAM, 0);
//	memset(&serv_addr, 0, sizeof(serv_addr));
//	serv_addr.sin_family=AF_INET;
//	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
//	serv_addr.sin_port=htons(atoi("9999"));
//
//	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
//	printf("connected to server!\n");
//	readfp=fdopen(sock, "r");
//	writefp=fdopen(sock, "w");
//
////	while(1)
////	{
////		if(fgets(buf, sizeof(buf), readfp)==NULL)
////			break;
////		fputs(buf, stdout);
////		fflush(stdout);
////	 }
//
//    // 发送信息
//	fputs("FROM CLIENT: Hello! \n", writefp);
//	fflush(writefp);
//	fclose(writefp);
//	fclose(readfp);
	return 0;
}
