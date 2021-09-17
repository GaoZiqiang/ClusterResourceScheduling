#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void* arg);
void send_data(FILE* fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(char* message);
void getParams(char * str);

int main(int argc, char *argv[])
{
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
		// why 2 times?
		printf("time %d Connection Request : %s:%d\n",
			n,inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
		//printf("before pthread_create\n");
		pthread_create(&t_id, NULL, request_handler, (socklen_t*)&clnt_sock);
		pthread_detach(t_id);
        //printf("after pthread_create\n");
	}
	close(serv_sock);
	return 0;
}

void* request_handler(void *arg)
{
    // ??clnt_sock
	int clnt_sock=*((int*)arg);
	printf("clnt_sock: %d\n",clnt_sock);
	char req_line[SMALL_BUF];
	FILE* clnt_read;
	FILE* clnt_write;

	char message[500];
	
	char method[10];// method??????
	char ct[15];
	char file_name[30];

//    int str_len = read(clnt_sock,message,sizeof(message)-1);
//    if (str_len != -1)
//        printf("message: %s\n",message);

	clnt_read=fdopen(clnt_sock, "r");
	clnt_write=fdopen(dup(clnt_sock), "w");
	// only request line?other information?
	fgets(req_line, 500, clnt_read);

    printf("Get from client: %s",req_line);

    // 调用mysql接口，将数解析并存入数据库
//    // ?????http??
//	if(strstr(req_line, "HTTP/")==NULL)// strstr(str1,str2) ?????????str2???str1???
//	{
//		send_error(clnt_write);
//		fclose(clnt_read);
//		fclose(clnt_write);
//		return;
//	 }
//
//	// ??method strtok()????????????method
//	strcpy(method, strtok(req_line, " /"));// C ??? char *strtok(char *str, const char *delim)????? str ???????delim ????
//	// ????:????index.html??ct???
//	strcpy(file_name, strtok(NULL, " /"));// ???????
//    printf("file_name: %s\n",file_name);
//	strcpy(ct, content_type(file_name));// ??Content-type
//    printf("ct: %s\n",ct);
//
//	if (!strcmp(method, "GET")) {
//        fclose(clnt_read);
//        send_data(clnt_write, ct, file_name); // file_name:a file path
//	} else if (!strcmp(method, "POST")) {
//        printf("this is a post request!\n");
//        fclose(clnt_read);
////        getParams(req_line);
//        send_data(clnt_write, ct, file_name); // file_name:a file path
//	} else {
//        send_error(clnt_write);
//        fclose(clnt_read);
//        fclose(clnt_write);
//        return;
//	}

    fclose(clnt_read);
    fclose(clnt_write);
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

char* content_type(char* file)
{
	char extension[SMALL_BUF];
	char file_name[SMALL_BUF];
	strcpy(file_name, file);
	strtok(file_name, ".");// ????????--???--index
    printf("file_name2: %s\n",file_name);
	strcpy(extension, strtok(NULL, "."));// ???????--html--?????
    printf("extension: %s\n",extension);
	
	if(!strcmp(extension, "html")||!strcmp(extension, "htm")) 
		return "text/html";
	else
		return "text/plain";
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

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void getParams(char * str) {
//    const char s[2] = "/";
//    char *token;

    /* ????????? */
    char * token = strtok(str, "/");
    printf("%s\n",token);
    free(token);
    char * token2 = strtok(NULL, "/");
    printf("%s\n",token2);

    char * temp = strtok(token2,"?");
    printf("%s\n",temp);
    char * params1 = strtok(NULL,"");
    printf("%s\n",params1);

    char * params2 = strtok(params1," ");
    printf("%s\n",params2);

    char * param_n1 = strtok(params2,"&");
    printf("%s\n",param_n1);
    char * param_n2= strtok(NULL,"&");
    printf("%s\n",param_n2);

    char * n1 = strtok(param_n1,"=");
    printf("%s\n",n1);
    n1 = strtok(NULL,"=");
    printf("%s\n",n1);// n1

    char * n2 = strtok(param_n2,"=");
    printf("%s\n",n2);
    n2 = strtok(NULL,"=");
    printf("%s\n",n2);// n2

    printf("params: n1 = %s, n2 = %s\n",n1, n2);
}