/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: 2020年03月28日 星期六 20时23分19秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
char *get_value(char *path, char *key);
void make_nonblock_ioctl(int);
void make_block_ioctl(int);
void make_nonblock(int);
void make_block(int);

#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#endif


