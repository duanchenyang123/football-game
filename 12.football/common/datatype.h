/*************************************************************************
	> File Name: datatype.h
	> Author: 
	> Mail: 
	> Created Time: 2020年06月06日 星期六 20时24分22秒
 ************************************************************************/

#ifndef _DATATYPE_H
#define _DATATYPE_H
#define MAXMSG 1024
struct LogRequest{
    char name[20];
    int team;//0 red 1 blue
    char msg[512];
};

struct LogResponse{
    int type;//0 success   1 failed
    char msg[512];
};

//位置
struct Point{
    int x;
    int y;
};

//用户信息
struct User{
    int team;
    int fd;
    char name[20];
    int flag;//未响应次数
    int online;
    struct Point loc;
};

//地图信息
struct Map{
    int width;
    int height;
    struct Point start;
    int gate_width;
    int gate_height;
};

//对球的控制
struct Ctrl{
    int carry;
    int kick;
};

struct TransMsg{
    int dirx;
    int diry;//两个方向
    struct Ctrl ctrl;
};

#define FT_TEST 0x01
#define FT_WALL 0x02
#define FT_MSG 0x04
#define FT_ACK 0x08
#define FT_FIN 0x10

struct FootBallMsg{
    int type;
    int size;
    int team;
    char name[20];
    char msg[MAXMSG];
};


#endif
