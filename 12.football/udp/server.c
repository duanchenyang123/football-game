/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月06日 星期六 10时45分00秒
 ************************************************************************/
//connect函数在udp中的使用

#include"../common/head.h"
#include"../common/color.h"
#include"../common/common.h"
#include"../common/udp_server.h"

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s port!\n", argv[0]);
        exit(1);
    }
    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in client;
    socklen_t len = sizeof(struct sockaddr_in);
    if((sockfd = socket_create_udp(port)) < 0){
        perror("socket_create_udp");
        exit(1);
    }
    char msg[512] = {0};
    recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&client, &len);
    
    printf("D: %s\n", msg);
    
    if(connect(sockfd, (struct sockaddr *)&client, len) < 0){
        perror("connect");
        exit(1);
    }

    send(sockfd, "hello!", sizeof("hello!"), 0);
    
    return 0;
}
