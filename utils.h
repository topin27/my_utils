#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdbool.h>
#include <sys/types.h>

/*
 * 根据进程名获取进程的pid
 * 参数： proc_name = 进程名
 * 返回值： -2 = 进程不存在
 *          -1 = /proc文件夹打开失败
 *          其他值 = 进程的id*/
pid_t get_pid_by_name(const char *proc_name);

/*
 * 判断字符串是否是数字
 * 参数： str = 字符串
 * 返回值： 判断结果，true or false*/
bool string_isnumber(const char *str);

/* 获取指定进程的父进程pid
 * 参数： pid = 指定进程的pid
 * 返回值： -1 = 获取失败
 *          其他值 = 进程的父进程pid*/
pid_t get_ppid(pid_t pid);

/*
 * 获取指定进程pid的uid；
 * 参数： pid = 指定进程的pid
 * 返回值： -1 = 获取失败
 *          其他值 = 指定进程的uid*/
uid_t get_uid_by_pid(pid_t pid);

/*
 * 获取指定进程的进程名；
 * 参数： pid = 指定进程的pid
 *        procname = 保存输出的进程名
 *        len = procname长度
 * 返回值： -1 = 获取失败
 *          0 = 获取成功
 *          1 = 进程名为空*/
int get_name_by_pid(pid_t pid, char *procname, int len);

#endif
