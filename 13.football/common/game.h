/*************************************************************************
	> File Name: game.h
	> Author: 
	> Mail: 
	> Created Time: 2020年06月05日 星期五 14时38分01秒
 ************************************************************************/

#ifndef _GAME_H
#define _GAME_H
#include<curses.h>
#define DEFARG(name, default_value) ((#name[0]) ? (name + 0) : default_value)
#define Show_Message(arg0, arg1, arg2, arg3) show_message(DEFARG(arg0, Message), DEFARG(arg1, NULL), arg2, DEFARG(arg3, 0))


#define MAX 50
char data_stream[20];

struct Map court;
WINDOW *Football_t, *Football, *Message, *Help, *Score, *Write;

WINDOW *create_newwin(int width, int height, int startx, int starty);
void destroy_win(WINDOW *win);
void gotoxy(int x, int y);
void gotoxy_putc(int x, int y, int c);
void gotoxy_puts(int x, int y, char *s);
void w_gotoxy_putc(WINDOW *win, int x, int y, int c);
void w_gotoxy_puts(WINDOW *win, int x, int y, char *s);
void initfootball();
void *draw(void *arg);
void show_message(WINDOW *win, struct User *user, char *msg, int type);
#endif
