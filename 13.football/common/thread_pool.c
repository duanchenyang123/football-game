/*************************************************************************
	> File Name: thread_pool.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月11日 星期四 15时27分11秒
 ************************************************************************/

#include"thread_pool.h"
#include"color.h"
#include"common.h"
#include"udp_epoll.h"
#include"game.h"
#include"./ball_status.h"
#include"show_data_stream.h"
extern int repollfd, bepollfd;
extern struct BallStatus ball_status;

void do_echo(struct User *user){
    struct FootBallMsg msg;
    char tmp[512] = {0};
    bzero(&msg, sizeof(msg));
    int size = recv(user->fd, (void *)&msg, sizeof(msg), 0);
    DBG(BLUE"DO ECHO : %s, msg.type = %d\n"NONE, msg.msg, msg.type);
    user->flag = 10;
    //user->online = 1;
    if(msg.type & FT_ACK){
        if(user->team){
            DBG(BLUE"%s"NONE" ❤\n", user->name);
        }
        else{
            DBG(RED"%s"NONE" ❤\n", user->name);
        }
    }else if(msg.type & (FT_WALL | FT_MSG)){
        show_data_stream('n');
        if(user->team){
            DBG(BLUE"%s : %s\n"NONE, user->name, msg.msg);
        }
        else{
            DBG(RED"%s : %s\n"NONE, user->name, msg.msg);
        }
        strcpy(msg.name, user->name);
        msg.team = user->team;
        Show_Message( , user, msg.msg, );
        send(user->fd, (void *)&msg, sizeof(msg), 0);

    }else if(msg.type & FT_FIN){
        show_data_stream('e');
        DBG(RED"%s logout!\n", user->name);
        sprintf(tmp, "%s logout !", user->name);
        Show_Message( , NULL,  tmp, 1);
        user->online = 0;
        int epollfd_temp = (user->team ? bepollfd : repollfd);
        del_event(epollfd_temp, user->fd);
    }else if(msg.type & FT_CTL){
        Show_Message(, user, "ctl_msg", 0);
        if(msg.ctl.dirx || msg.ctl.diry){
            show_data_stream('n');
            user->loc.x += msg.ctl.dirx;
            user->loc.y += msg.ctl.diry;
            if(user->loc.x <= -1) user->loc.x = -1;
            if(user->loc.x >= court.width) user->loc.x = court.width;
            if(user->loc.y <= -1) user->loc.y = -1;
            if(user->loc.y >= court.height) user->loc.y = court.height;
        }
        if(msg.ctl.action & ACTION_KICK){
            show_data_stream('k');
            if(can_kick(&(user->loc), msg.ctl.strength)){
                ball_status.status = 0;
                ball_status.who = user->team;
                strcpy(ball_status.name, user->name);
            }
        }

        if(msg.ctl.action & ACTION_STOP){
            show_data_stream('s');
            if(can_access(&(user->loc))){
                ball_status.status = 0;
                ball_status.v.x = ball_status.v.y = ball_status.a.x = ball_status.a.y = 0; 
                ball_status.who = user->team;
                strcpy(ball_status.name, user->name);
            } 
        }

        if(msg.ctl.action & ACTION_CARRY){
            show_data_stream('c');
            if(can_access(&(user->loc))){
                ball_status.status = 1;
                ball_status.v.x = ball_status.v.y = ball_status.a.x = ball_status.a.y = 0; 
                ball_status.who = user->team;
                strcpy(ball_status.name, user->name);
            } 
        }
    }
}

void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd){
    taskQueue->sum = sum;
    taskQueue->epollfd = epollfd;
    taskQueue->team = calloc(sum, sizeof(void *));
    taskQueue->head = taskQueue->tail = 0;
    pthread_mutex_init(&taskQueue->mutex, NULL);
    pthread_cond_init(&taskQueue->cond, NULL);
}

void task_queue_push(struct task_queue *taskQueue, struct User *user){
    pthread_mutex_lock(&taskQueue->mutex);
    taskQueue->team[taskQueue->tail] = user;
    DBG(GREEN"Thread Pool: "NONE" Task Push %s\n", user->name);
    if(++taskQueue->tail == taskQueue->sum){
        DBG(GREEN"Thread  Pool: "NONE"Task Queue End\n");
        taskQueue->tail = 0;
    }
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
}

struct User *task_queue_pop(struct task_queue *taskQueue){
    
    pthread_mutex_lock(&taskQueue->mutex);
    while(taskQueue->tail == taskQueue->head){
        DBG(GREEN"Thread  Pool: "NONE"Task Queue Empty, waiting for task...\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    struct User *user = taskQueue->team[taskQueue->head];
    
    DBG(GREEN"Thread  Pool: "NONE"Task pop %s\n", user->name);
    if(++taskQueue->head == taskQueue->sum){
        DBG(GREEN"Thread  Pool: "NONE"Task Queue End\n");
        taskQueue->head = 0;
    }
    pthread_mutex_unlock(&taskQueue->mutex);
    return user;
}

void *thread_run(void *arg){
    pthread_t tid = pthread_self();
    pthread_detach(tid);
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while(1){
        struct User *user = task_queue_pop(taskQueue);
        do_echo(user);
    }
}
