/*************************************************************************
	> File Name: client_redraw.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月05日 星期日 18时47分14秒
 ************************************************************************/

#include"head.h"
#include"color.h"
#include"datatype.h"
#include"common.h"
#include"game.h"
extern struct User *rteam, *bteam;
//extern WINDOW *Football, *Football_t, *Score;
extern struct Bpoint ball;
extern struct BallStatus ball_status;
extern struct Score score;

int parse_cjson_football(char *cjson_data_football){
    
    bzero(rteam, sizeof(rteam));
    bzero(bteam, sizeof(bteam));   
    bzero(&ball, sizeof(ball));
    bzero(&ball_status, sizeof(ball_status));
    cJSON *cjson_football = cJSON_Parse(cjson_data_football);
    cJSON *reds = NULL;
    cJSON *blues = NULL;
    cJSON *red = NULL;
    cJSON *blue = NULL;
    cJSON *data_ball = NULL;
    int status = 0;
    if(cjson_football == NULL){
        status = 0;
        goto end;
    }
    reds = cJSON_GetObjectItemCaseSensitive(cjson_football, "red");
    int i = 0;
    cJSON_ArrayForEach(red, reds)
    {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(red, "name") ;
        cJSON *x = cJSON_GetObjectItemCaseSensitive(red, "x");
        cJSON *y = cJSON_GetObjectItemCaseSensitive(red, "y");
        DBG(RED"%s : (%d , %d)\n"NONE, name->valuestring, x->valueint, y->valueint);
        
        rteam[i].team = 0;
        strcpy(rteam[i].name, name->valuestring);
        rteam[i].online = 1;
        rteam[i].loc.x = x->valueint;
        rteam[i].loc.y = y->valueint;
        i++;
        for(int j = 0; j < i; j++){
            DBG(RED"i = %d : %s (%d , %d)\n"NONE, i, rteam[j].name, rteam[j].loc.x, rteam[j].loc.y);
        }
        
        //redraw_player(0, name->valuestring, x->valueint, y->valueint);
    }
    i = 0;
    blues = cJSON_GetObjectItemCaseSensitive(cjson_football, "blue");
    cJSON_ArrayForEach(blue, blues)
    {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(blue, "name") ;
        cJSON *x = cJSON_GetObjectItemCaseSensitive(blue, "x");
        cJSON *y = cJSON_GetObjectItemCaseSensitive(blue, "y");
        DBG(BLUE"%s : (%d , %d)\n"NONE, name->valuestring, x->valueint, y->valueint);
        
        bteam[i].team = 1;
        strcpy(bteam[i].name, name->valuestring);
        bteam[i].online = 1;
        bteam[i].loc.x = x->valueint;
        bteam[i].loc.y = y->valueint;
        i++;
        for(int j = 0; j < i; j++){
            DBG(RED"i = %d : %s (%d , %d)\n"NONE, i, bteam[j].name, bteam[j].loc.x, bteam[j].loc.y);
        }
        //redraw_player(1, name->valuestring, x->valueint, y->valueint);
    }

    data_ball = cJSON_GetObjectItemCaseSensitive(cjson_football, "ball");
    cJSON *ball_who = cJSON_GetObjectItemCaseSensitive(data_ball, "who");
    cJSON *ball_name = cJSON_GetObjectItemCaseSensitive(data_ball, "name");
    cJSON *ball_x = cJSON_GetObjectItemCaseSensitive(data_ball, "x");
    cJSON *ball_y = cJSON_GetObjectItemCaseSensitive(data_ball, "y");
    
    DBG(YELLOW"ball : %d %s %lf %lf\n"NONE, ball_who->valueint, ball_name->valuestring, ball_x->valuedouble, ball_y->valuedouble);
    ball_status.who = ball_who->valueint;
    strcpy(ball_status.name, ball_name->valuestring);
    ball.x = ball_x->valuedouble;
    ball.y = ball_y->valuedouble;

end:
    cJSON_Delete(cjson_football);
    return status;

}

int parse_cjson_score(char *cjson_data_score){
    cJSON *cjson_score = cJSON_Parse(cjson_data_score);
    cJSON *scores = NULL;
    //cJSON *score1 = NULL;
    cJSON *score_red = NULL;
    cJSON *score_blue = NULL;
    cJSON *name = NULL;
    cJSON *who = NULL;
    int status = 0;
    bzero(&score, sizeof(score));
    who = cJSON_GetObjectItemCaseSensitive(cjson_score, "who");
    if(who == NULL){
        DBG(RED"who == NULL!\n"NONE);
        goto end;
    }

    name = cJSON_GetObjectItemCaseSensitive(cjson_score, "name");
    if(name == NULL){
        DBG(RED"name == NULL!\n"NONE);
        goto end;
    }
    scores = cJSON_GetObjectItemCaseSensitive(cjson_score, "score");
    if(scores == NULL){
        DBG(RED"scores == NULL!\n"NONE);
        goto end;
    }
    /*
    cJSON_ArrayForEach(score, scores)
    {
        score_red = cJSON_GetObjectItemCaseSensitive(score, "");
        if(score_red == NULL){
            DBG(RED"score_red == NULL!\n"NONE);
            goto end;
        }
        score_blue = cJSON_GetObjectItemCaseSensitive(score, "*");
        if(score_blue == NULL){
            DBG(RED"score_blue == NULL!\n"NONE);
            goto end;
        }
    }
    */
    
    score_red = cJSON_GetObjectItemCaseSensitive(scores, "red");
    if(score_red == NULL){
        DBG(RED"score_red == NULL!\n"NONE);
        goto end;
    }
    
    score_blue = cJSON_GetObjectItemCaseSensitive(scores, "blue");
    if(score_blue == NULL){
        DBG(RED"score_blue == NULL!\n"NONE);
        goto end;
    }


    DBG(GREEN" %s : %d (red) %d : %d (blue)\n"NONE, name->valuestring, who->valueint, score_red->valueint, score_blue->valueint);
    
    score.red = score_red->valueint;
    score.blue = score_blue->valueint;

end:
    cJSON_Delete(cjson_score);
    return status;
}





/*
void showscore(){
    char msg[20] = {0};
    sprintf(msg, "(red) %d : %d (blue)", score.red, score.blue);
    w_gotoxy_puts(Score, 1, 1, msg);
    wrefresh(Score);
}

void ballgate(){
    for(int i = court.height / 2 - 3; i <= court.height / 2 + 3; i++){
            w_gotoxy_putc(Football_t, 1, i, '*');
            w_gotoxy_putc(Football_t, court.width + 2, i, '*');
    }    
}

*/
