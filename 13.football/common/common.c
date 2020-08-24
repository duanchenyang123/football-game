/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月28日 星期六 20时21分19秒
 ************************************************************************/

#include"head.h"
char conf_ans[50] = {0};
char *get_value(char *path, char *key){
    FILE *fp = NULL;
    ssize_t nrd;
    char *line = NULL, *sub = NULL;
    size_t linecap;
    extern char conf_ans[50];
    
    if(path == NULL || key == NULL){
        fprintf(stderr, "Error in arguments!\n");
        return NULL;
    }
    if((fp = fopen(path, "r")) == NULL){
        perror("fopen");
        return NULL;
    }
    while((nrd = getline(&line, &linecap, fp)) != -1){
        if((sub = strstr(line, key)) == NULL) continue;
        else {
            if(line[strlen(key)] == '=') {
                strncpy(conf_ans, sub + strlen(key) + 1, nrd - strlen(key) - 2);
                *(conf_ans + nrd - strlen(key) - 2) = '\0';
                break;
            }

        }
    }

    free(line);
    fclose(fp);
    if(sub == NULL) return NULL;
    return conf_ans;
}

//两种方式实现ioctl，后一种有bug
void make_nonblock_ioctl(int fd){
    unsigned long ul = 1;
    ioctl(fd, FIONBIO, &ul);
}
void make_block_ioctl(int fd){
    unsigned long ul = 0;
    ioctl(fd, FIONBIO, &ul);
}


//设置为非阻塞socket
//直接设置O_NONBLOCK会覆盖读写等其他属性 
void make_nonblock(int fd){
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

//设置为阻塞socket
void make_block(int fd){
    fcntl(fd, F_SETFL, ~O_NONBLOCK);
}

