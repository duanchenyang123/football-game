/*************************************************************************
	> File Name: cjson_data.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月05日 星期日 15时13分35秒
 ************************************************************************/

#include"head.h"
#include"common.h"
#include"color.h"
#include"game.h"
#define MAX 50
#include"./datatype.h"
extern struct User *rteam, *bteam;
extern struct Bpoint ball;
extern struct BallStatus ball_status;
extern struct Score score;
extern WINDOW *Message;

char *cjson_data_football(){
    cJSON *data_football = cJSON_CreateObject();
    cJSON *reds = cJSON_AddArrayToObject(data_football, "red");
    cJSON *blues = cJSON_AddArrayToObject(data_football, "blue");

    if (reds == NULL || blues == NULL)
    {
        goto end;
    }

    for (int i = 0; i < MAX; i++)
    {
        if(!rteam[i].online) continue;
        cJSON *red = cJSON_CreateObject();
        if(cJSON_AddStringToObject(red, "name", rteam[i].name) == NULL){
            goto end;
        }

        if (cJSON_AddNumberToObject(red, "x", rteam[i].loc.x) == NULL)
        {
            goto end;
        }

        if (cJSON_AddNumberToObject(red, "y", rteam[i].loc.y) == NULL)
        {
            goto end;
        }

        cJSON_AddItemToArray(reds, red);
    }
    for (int i = 0; i < MAX; i++)
    {
        if(!bteam[i].online) continue;
        cJSON *blue = cJSON_CreateObject();
        if(cJSON_AddStringToObject(blue, "name", bteam[i].name) == NULL){
            goto end;
        }

        if (cJSON_AddNumberToObject(blue, "x", bteam[i].loc.x) == NULL)
        {
            goto end;
        }

        if (cJSON_AddNumberToObject(blue, "y", bteam[i].loc.y) == NULL)
        {
            goto end;
        }

        cJSON_AddItemToArray(blues, blue);
    }
    cJSON *data_ball = cJSON_CreateObject();
    if(cJSON_AddNumberToObject(data_ball, "who", ball_status.who) == NULL){
        goto end;
    }
    if(cJSON_AddStringToObject(data_ball, "name", ball_status.name) == NULL){
        goto end;
    }
    if(cJSON_AddNumberToObject(data_ball, "x", ball.x) == NULL){
        goto end;
    }
    if(cJSON_AddNumberToObject(data_ball, "y", ball.y) == NULL){
        goto end;
    }
    cJSON_AddItemToObject(data_football, "ball", data_ball); 

    char *string = cJSON_Print(data_football);
    if(string == NULL) fprintf(stderr, "Failed to print monitor.\n");
end:
    cJSON_Delete(data_football);
    return string;



}
char *cjson_data_score(){

    cJSON *data_score = cJSON_CreateObject();
    cJSON *scores = cJSON_CreateObject();
    if(scores == NULL) goto end;
    
    if(cJSON_AddNumberToObject(scores, "red", score.red) == NULL) goto end;
    if(cJSON_AddNumberToObject(scores, "blue", score.blue) == NULL) goto end;
    cJSON_AddItemToObject(data_score, "score", scores);
    if(cJSON_AddNumberToObject(data_score, "who", ball_status.who) == NULL) goto end;
    if(cJSON_AddStringToObject(data_score, "name", ball_status.name) == NULL) goto end;
    //哪队得分
    char *string = cJSON_Print(data_score);
    if(string == NULL) fprintf(stderr, "Failed to print monitor.\n");
end:
    cJSON_Delete(data_score);
    return string;
}

void *cjson_data_send(){
    while(1){
        //sleep(1);
        usleep(200000);
        struct FootBallMsg msg_football, msg_score;
        
        bzero(&msg_football, sizeof(msg_football));
        bzero(&msg_score, sizeof(msg_score));
        msg_football.type = FT_GAME;
        msg_score.type = FT_SCORE;
        strcpy(msg_football.msg, cjson_data_football());
        strcpy(msg_score.msg, cjson_data_score());
        
        DBG(RED"CJSON DATA : \n"NONE"%s\n%s\n", msg_football.msg, msg_score.msg);
        
        for(int i = 0; i < MAX; i++){
            if(!rteam[i].online) continue;
            send(rteam[i].fd, (void *)&msg_football, sizeof(msg_football), 0);
            send(rteam[i].fd, (void *)&msg_score, sizeof(msg_score), 0);
        } 
        for(int i = 0; i < MAX; i++){
            if(!bteam[i].online) continue;
            send(bteam[i].fd, (void *)&msg_football, sizeof(msg_football), 0);
            send(bteam[i].fd, (void *)&msg_score, sizeof(msg_score), 0);
        }
        
        //Show_Message(Message, , "broadcast!", );
        
    }
}

