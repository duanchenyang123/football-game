/*************************************************************************
	> File Name: client_recever.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月13日 星期六 14时13分15秒
 ************************************************************************/

#include"head.h"
#include"common.h"
#include"color.h"
#include"game.h"


#include"./client_parse_cjson.h"
extern int sockfd;

void *client_recv(void *arg){
    while(1){
        struct FootBallMsg msg;
        struct User user;
        bzero(&msg, sizeof(msg));
        recv(sockfd, (void *)&msg, sizeof(msg), 0);
        strcpy(user.name, msg.name);
        if(msg.type & FT_TEST){
            DBG(RED"HeartBeat from server 💓\n"NONE);
            msg.type = FT_ACK;
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        }else if(msg.type &  FT_MSG){
            DBG(GREEN"Server Msg: "NONE"%s\n", msg.msg);
            Show_Message(, &user, msg.msg, 0);
        }else if(msg.type & FT_WALL){
            Show_Message( , NULL, msg.msg, 1 );
        }
        else if(msg.type & FT_FIN){
            DBG(GREEN"Server is going to stop!\n"NONE);
            endwin();
            exit(0);
        }else if(msg.type & FT_GAME){
            DBG(YELLOW"Server CJSON GAME: "NONE"%s\n", msg.msg);
            parse_cjson_football(msg.msg);
            //Show_Message(, NULL, "CJSON GAME!", 1);
        }else if(msg.type & FT_SCORE){
            DBG(YELLOW"Server CJSON SCORE: "NONE"%s\n", msg.msg);
            parse_cjson_score(msg.msg);
            //Show_Message(, NULL, "CJSON SCORE!", 1);
        }else{
            DBG(GREEN"Msg Unsupport!\n"NONE);
        }
    }
}

