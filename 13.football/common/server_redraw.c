/*************************************************************************
	> File Name: server_redraw.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月14日 星期日 20时30分21秒
 ************************************************************************/

#include"head.h"
#define MAX 50
#include"game.h"

extern struct User *rteam, *bteam;
extern WINDOW *Football, *Football_t, *Score;
extern struct Bpoint ball;
extern struct BallStatus ball_status;
extern struct Score score;

void re_draw_player(int team, char *name, struct Point *loc){
    char p = 'k';
    if(team){
        wattron(Football, COLOR_PAIR(6));

    }else{
        wattron(Football, COLOR_PAIR(2));
    }
    
    //w_gotoxy_putc(Football, loc->x, loc->y, p);
    //w_gotoxy_puts(Football, loc->x + 1, loc->y - 1, name);
    
    w_gotoxy_putc(Football, loc->x, loc->y, p);
    w_gotoxy_puts(Football, loc->x + 1, loc->y - 1, name);

}

void re_draw_team(struct User *team){
    for(int i = 0; i < MAX; i++){
        if(!team[i].online) continue;
        re_draw_player(team[i].team, team[i].name, &team[i].loc);
    }
}
void redraw_ball(){
    double t = 0.1;
    //double old_vx = 0;
    //double old_vy = 0;
    if(ball_status.status == 1){
        struct User user;
        bzero(&user, sizeof(user));
        for(int i = 0; i < MAX; i++){
            if(!strcmp(rteam[i].name, ball_status.name)) {
                user = rteam[i];
                break;
            }
            if(!strcmp(bteam[i].name, ball_status.name)) {
                user = bteam[i];
                break;
            }
        }
        //w_gotoxy_putc(Football, ball.x, ball.y, ' ');
        ball.x = user.loc.x + 1;
        ball.y = user.loc.y;

    }
    if(ball_status.v.x != 0 || ball_status.v.y != 0) {
        ball.x = ball.x + ball_status.v.x * 0.1 + 0.5 * ball_status.a.x * 0.01;
        ball.y = ball.y + ball_status.v.y * 0.1 + 0.5 * ball_status.a.y * 0.01;
        double tempvx = ball_status.v.x;
        double tempvy = ball_status.v.y;
        ball_status.v.x += ball_status.a.x;
        ball_status.v.y += ball_status.a.y;
        
        if((int)ball.x >= court.width - 1){
            ball.x = court.width - 1;
            ball.y = (int)ball.y;
            ball_status.v.x = ball_status.v.y = ball_status.a.x = ball_status.a.y = 0;
        }

        if((int)ball.y >= court.height - 1){
            ball.y = court.height - 1;
            ball.x = (int)ball.x;
            ball_status.v.x = ball_status.v.y = ball_status.a.x = ball_status.a.y = 0;
        }

        if((int)ball.x <= 0){
            ball.x = 0;
            ball.y = (int)ball.y;
            ball_status.v.x = ball_status.v.y = ball_status.a.x = ball_status.a.y = 0;
        }

        if((int)ball.y <= 0){
            ball.y = 0;
            ball.x = (int)ball.x;
            ball_status.v.x = ball_status.v.y = ball_status.a.x = ball_status.a.y = 0;
        }
         
        /*old_vx = ball_status.v.x;
        old_vy = ball_status.v.y;
        if(old_vx * ball_status.v.x <= 0){
            ball_status.v.x = ball_status.a.x = 0;
        }
        if(old_vy * ball_status.v.y <= 0){
            ball_status.v.y = ball_status.a.y = 0;
        }*/

        if (tempvx * ball_status.v.x < 0 || tempvy * ball_status.v.y < 0 ) ball_status.v.x = ball_status.v.y = 0;
    }   
    w_gotoxy_putc(Football, (int)(ball.x), (int)(ball.y), 'O');

    if(ball.x <= 2 && ball.y >= court.height / 2 - 3 && ball.y <= court.height / 2 + 3){
        score.red++;
        ball.x = (court.width)/ 2;
        ball.y = (court.height)/ 2;
        return ;
    }
    if(ball.x >= court.width - 1 && ball.y >= court.height / 2 - 3 && ball.y <= court.height / 2 + 3){
        score.blue++;
        ball.x = (court.width) / 2;
        ball.y = (court.height) / 2;
        return ;
    }
    return ;
}

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

void server_redraw(){
    werase(Football_t);
    box(Football_t, 0, 0);
    box(Football, 0, 0);
    ballgate();
    showscore();
    re_draw_team(rteam);
    re_draw_team(bteam);
    redraw_ball();
    wrefresh(Football_t);
    //wrefresh(Football);
}

