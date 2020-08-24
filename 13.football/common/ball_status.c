/*************************************************************************
	> File Name: ball_status.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月02日 星期四 18时18分05秒
 ************************************************************************/

#include"head.h"
#include<math.h>
#include"game.h"
extern WINDOW *Message;
extern struct Bpoint ball;
extern struct BallStatus ball_status;

/*
int can_kick(struct Point *loc, int strength){
    int px = loc->x;
    int py = loc->y;
    
    //int px = loc->x - 2;
    //int py = loc->y - 1;
    
    //char msg[50] = {0};
    //sprintf(msg, "p = (%d, %d), ball = (%d, %d)", px, py, (int)(ball.x), (int)(ball.y));
    //Show_Message(Write,,msg,);

    if(abs(px - (int)(ball.x) <= 2 && abs(py - (int)(ball.y)) <= 2)){
        if(px == (int)(ball.x) && py == (int)(ball.y)){
            return 0;
        }
        
        double dx, dy;
        if((int)ball.x > px) dx = 1;
        else if((int)ball.x == px) dx = 0;
        else dx = -1;

        if((int)ball.y > py) dy = 1;
        else if((int)ball.y == py) dy = 0;
        else dy = -1;
        
        //double dx = (ball.x - px) / pow(((ball.x - px) * (ball.x - px) + (ball.y - py) * (ball.y - py)), 0.5);
        //double dy = (ball.y - py) / pow(((ball.x - px) * (ball.x - px) + (ball.y - py) * (ball.y - py)), 0.5);
        //ball_status.a.x = -3 * dx;
        //ball_status.a.y = -3 * dy;

        ball_status.a.x = -3 * dx;
        ball_status.a.y = -3 * dy;
        ball_status.v.x = strength * 8 * dx;
        ball_status.v.y = strength * 8 * dy;
        //if(ball_status.v.x <= 0){
        //    ball_status.a.x = ball_status.a.y = ball_status.v.x = ball_status.v.y = 0;
        //}
        return 1;
    }else{
        return 0;
    }
}
*/

int can_kick(struct Point *loc, int strength) {
   // int px = loc->x - 2, py = loc->y - 1;
    int px = loc->x , py = loc->y ;
    if (abs(px - (int)ball.x) <= 2 && abs(py - (int) ball.y <= 2) ) {
        if (px == (int)ball.x && py == (int)ball.y) {
            return 0;
        } 
        //double dx = (ball.x - px) / pow(pow((ball.x - px), 2) + pow((ball.y - py), 2), 0.5);
        //double dy = (ball.y - py) / pow(pow((ball.x - px), 2) + pow((ball.y - py), 2), 0.5);
        double dx, dy;
        if((int)ball.x > px) dx = 1;
        else if((int)ball.x == px) dx = 0;
        else dx = -1;

        if((int)ball.y > py) dy = 1;
        else if((int)ball.y == py) dy = 0;
        else dy = -1;
        ball_status.a.x = -3 * dx;
        ball_status.a.y = -3 * dy;
        ball_status.v.x = strength * 8 * dx;
        ball_status.v.y = strength * 8 * dy;
        return 1;
    }
    return 0;
}


int can_access(struct Point *loc){
        
    int px = loc->x;
    int py = loc->y;
    
    if(abs(px - (int)(ball.x) <= 2 && abs(py - (int)(ball.y)) <= 2)){
        if(px == (int)(ball.x) && py == (int)(ball.y)){
            return 0;
        }
        return 1;
    }else {
        return 0;
    }
}

int can_carry(struct Point *loc){    
    int px = loc->x;
    int py = loc->y;
    if(abs(px - (int)ball.x) <= 1 && abs(py - (int) ball.y <= 1)) return 1;
    return 0;
}




