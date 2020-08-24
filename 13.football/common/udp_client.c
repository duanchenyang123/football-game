/*************************************************************************
	> File Name: ../common/udp_client.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月06日 星期六 11时11分00秒
 ************************************************************************/


#include"head.h"

int socket_udp() {

    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    printf ("Socket creat.\n");
    return sockfd;
}

