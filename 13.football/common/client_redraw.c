/*************************************************************************
	> File Name: client_redraw.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月08日 星期三 18时46分38秒
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
    w_gotoxy_putc(Football, (int)(ball.x), (int)(ball.y), 'O');
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

void *client_redraw(){
    while(1){
        
        usleep(100000);
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
}

