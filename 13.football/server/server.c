/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月05日 星期五 14时15分12秒
 ************************************************************************/

#include"../common/head.h"
#include"../common/udp_server.h"
#include"../common/color.h"
#include"../common/common.h"
#include"../common/game.h"
#include"../common/thread_pool.h"
#include"../common/udp_epoll.h"
#include"../common/sub_reactor.h"
#include"../common/heart_beat.h"
#include"../common/server_exit.h"
#include"../common/server_redraw.h"
#include"../common/show_data_stream.h"
#include"../common/cjson_data.h"
char *conf = "./server.conf";
//struct Map court;

int port = 0;
int repollfd, bepollfd;
struct User *rteam;
struct User *bteam;
int data_port;
int epoll_fd;

struct Bpoint ball;
struct BallStatus ball_status;
struct Score score;
int main(int argc, char **argv){
    int opt, listener;
    pthread_t draw_t, red_t, blue_t, heart_t, cjson_data_t;
    while((opt = getopt(argc, argv, "p:")) != -1){
        switch(opt){
            case 'p':{
                port = atoi(optarg);
                break;
            }
            default:{
                fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
                exit(1);
            }
        }
    }
    
    argc -= (optind - 1);
    argv += (optind - 1);
    
    if(argc > 1){
        fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
        exit(1);
    }

    if(!port) port = atoi(get_value(conf, "PORT"));
    data_port = atoi(get_value(conf, "DATA_PORT"));
    court.width = atoi(get_value(conf, "COLS"));
    court.height = atoi(get_value(conf, "LINES"));
    court.start.x = 3;
    court.start.y = 3;
    ball.x = court.width / 2;
    ball.y = court.height / 2;
    ball_status.status = 0;
    ball_status.v.x = ball_status.v.y = ball_status.a.x = ball_status.a.y = 0;

    rteam = (struct User *)calloc(MAX, sizeof(struct User));
    bteam = (struct User *)calloc(MAX, sizeof(struct User));

    score.red = score.blue = 0;

    if((listener = socket_create_udp(port)) < 0){
        perror("socket_create_udp");
        exit(1);
    }
    
    DBG(GREEN"INFO"NONE" : Server start on port %d\n", port);
#ifndef _D    
    pthread_create(&draw_t, NULL, draw, NULL);
#endif 
    epoll_fd = epoll_create(MAX * 2);
    repollfd = epoll_create(MAX);
    bepollfd = epoll_create(MAX);
    if(epoll_fd < 0 || repollfd < 0 || bepollfd < 0){
        perror("epoll_create");
        exit(1);
    }
    
    struct task_queue redQueue;
    struct task_queue blueQueue;
    task_queue_init(&redQueue, MAX, repollfd);
    task_queue_init(&blueQueue, MAX, bepollfd);
    
    pthread_create(&red_t, NULL, sub_reactor, (void *)&redQueue);
    pthread_create(&blue_t, NULL, sub_reactor, (void *)&blueQueue);
    pthread_create(&heart_t, NULL, heart_beat, NULL);
    pthread_create(&cjson_data_t, NULL, cjson_data_send, NULL);

    signal(SIGINT, server_exit);
    struct epoll_event ev, events[MAX * 2];

    ev.events = EPOLLIN;
    ev.data.fd = listener;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener, &ev);


    //struct LogData lg;    
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    char buff[512] = {0};
    
    signal(14, server_redraw);
    struct itimerval itimer;
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 50000;
    itimer.it_value.tv_sec = 5;
    itimer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itimer, NULL);

    //Show_Message( , , "Waiting for login", 1);

    while(1){
        //8888port用来登陆
        //数据信息用其他端口
        //w_gotoxy_puts(Message, 1, 1, "wait for login!");
        int nfds = epoll_wait(epoll_fd, events, MAX * 2, -1);
        //DBG(YELLOW"Main Thread\n"NONE);
        for(int i = 0; i < nfds; i++){
            struct User user;
            if(events[i].data.fd == listener){
                //accept();
                int new_fd = udp_accept(epoll_fd, listener, &user);
                if(new_fd > 0){
                    char buf[512] = {0};
                    sprintf(buf, "%s loginin.", user.name);
                    DBG(YELLOW"Main Thread"NONE" : Add %s to %s sub_reactor.\n", user.name, (user.team ? "BLUE" : "RED"));
                    add_to_sub_reactor(&user);
                    Show_Message( , , buf, 1 );
                    show_data_stream('l');
                }else{
                    recv(events[i].data.fd, buff, sizeof(buff), 0);
                    printf(PINK"RECV"NONE" : %s\n", buff);
                }
            }
            //char info[1024] = {0};
            //recvfrom(events[i].data.fd, (void *)&lg, sizeof(lg), 0, (struct sockaddr *)&client, &len);           
            //sprintf(info, "Login:%s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            //w_gotoxy_puts(Message, 1, 2, info);
        }
    }
    

    return 0;
}


