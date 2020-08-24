/*************************************************************************
	> File Name: game.h
	> Author: 
	> Mail: 
	> Created Time: 2020年06月05日 星期五 14时38分01秒
 ************************************************************************/

#include"head.h"

extern struct Map court;
extern WINDOW *Football, *Football_t, *Message, *Help, *Score, *Write;
int message_num = 0;
int football_t = 2;
WINDOW *create_newwin(int width, int height, int startx, int starty){
    WINDOW *win;
    win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    wrefresh(win);
    //move(LINES - 1, 1);
    return win;
}

void destroy_win(WINDOW *win){
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
    return ;
}

void gotoxy(int x, int y){
    move(y, x);
}
void gotoxy_putc(int x, int y, int c){
    move(y, x);
    addch(c);
    move(LINES - 1, 1);
    refresh();
}

void gotoxy_puts(int x, int y, char *s){
    move(y, x);
    addstr(s);
    move(LINES - 1, 1);
    refresh();
}

void w_gotoxy_putc(WINDOW *win, int x, int y, int c){
    mvwaddch(win, y, x, c);
    move(LINES - 1, 1);
    wrefresh(win);
}
void w_gotoxy_puts(WINDOW *win, int x, int y, char *s){
    mvwprintw(win, y, x, s);
    move(LINES - 1, 1);
    wrefresh(win);
}

void show_help(){
    w_gotoxy_puts(Help, 1, 1, "k: kick");
    w_gotoxy_puts(Help, 1, 2, "j: stop");
    w_gotoxy_puts(Help, 1, 3, "l: carry");
    wrefresh(Help);
}

void initfootball(){
    initscr();
    clear();
    if(!has_colors() || start_color() == ERR){
        endwin();
        fprintf(stderr, "终端不支持颜色！\n");
        exit(1);
    }
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_GREEN);    
    init_pair(8, COLOR_BLACK, COLOR_RED);    
    init_pair(9, COLOR_BLACK, COLOR_BLUE);    
    init_pair(10, COLOR_BLACK, COLOR_YELLOW);    
    init_pair(11, COLOR_BLACK, COLOR_CYAN);    
    init_pair(12, COLOR_BLACK, COLOR_WHITE);    
    
    
    Football_t = create_newwin(court.width + 4, court.height + 2, court.start.x - 2, court.start.y - 1);
    Football = subwin(Football_t,  court.height, court.width, court.start.y, court.start.x);

    box(Football, 0, 0);
    
    WINDOW *Message_t = create_newwin(court.width + 4, 7, court.start.x - 2, court.start.y + court.height + 1);
                 
    
    Message = subwin(Message_t, 5, court.width + 2, court.start.y + court.height + 2, court.start.x -1);
    scrollok(Message, 1);
     
    Help = create_newwin(20, court.height + 2, court.start.x + court.width +2, court.start.y - 1);
    Score = create_newwin(20, 7, court.start.x + court.width + 2, court.start.y + court.height + 1);
    
    Write = create_newwin(court.width + 24, 5, court.start.x - 2, court.start.y + court.height + 8);
    
    //move(LINES - 1, 1);
    show_help();


    return ;
}

void *draw(void *arg){
    initfootball();
    while(1){
        sleep(10);
    }
}


void show_message(WINDOW *win, struct User *user, char *msg, int type){
    time_t time_now = time(NULL);
    struct tm *tm = localtime(&time_now);
    char timestr[20] = {0};
    char username[80] = {0};
    sprintf(timestr, "%02d:%02d:%02d ", tm->tm_hour, tm->tm_min, tm->tm_sec);
    if(type){
        wattron(win, COLOR_PAIR(4));
        strcpy(username, "Server Info : ");
    }else{
        if(user->team){
            wattron(win, COLOR_PAIR(6));
        }else{
            wattron(win, COLOR_PAIR(2));
        }
        sprintf(username, "%s : ", user->name);
    }
    if(message_num <= 4){
        w_gotoxy_puts(win, 10, message_num, username);
        wattron(win, COLOR_PAIR(3));
        w_gotoxy_puts(win, 10 + strlen(username), message_num, msg);
        wattron(win, COLOR_PAIR(5));
        w_gotoxy_puts(win, 1, message_num, timestr);
        message_num++;
    }else{
        message_num = 4;
        scroll(win);
        w_gotoxy_puts(win, 10, message_num, username);
        wattron(win, COLOR_PAIR(3));
        w_gotoxy_puts(win, 10 + strlen(username), message_num, msg);
        wattron(win, COLOR_PAIR(5));
        w_gotoxy_puts(win, 1, message_num, timestr);
        message_num++;
    }
    wrefresh(win);
}


