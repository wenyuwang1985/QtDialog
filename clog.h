/*************************************************
 * 文件名:  clog.h
 * 作者: 赵子轩
 * 日期: 2021-04
 * 内容描述: 日志库。1、支持windows和linux平台
					 2、支持多线程
					 3、支持指定日志路径、循环读写、级别控制。
					 4、（C++扩展）引入cpplogex.h，可支持函数监控
 * 配置说明：1、默认生成在运行模块目录，可通过配置DEFAULT_LOG_宏控制。
			 2、（可选）在运行目录下设置clog.conf，可配置日志路径、文件大小、日志级别。无配置文件走代码默认配置

*************************************************/

#ifndef __LOG_H__
#define __LOG_H__
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "stdarg.h"

#if defined(WIN32) || defined(_WINDOWS_)
#include <Windows.h>
#else
#include <dlfcn.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#endif


#define MAX_FILE_PATH (512)
#define CRYPTLOG_MARK __FILE__,__LINE__

typedef enum{
	ERROR_1=-1,
	ERROR_2=-2,
	ERROR_3=-3
}ERROR0;

typedef enum{
 	LOG_NONE=0,
	LOG_ERROR=1,
	LOG_WARN=2,
 	LOG_INFO_Ex=3,
 	LOG_DEBUG=4,
 	LOG_ALL=255
}CLOGLEVEL;

//#define LOG_NONE 0
//#define  LOG_ERROR 1
//#define LOG_WARN 2
//#define LOG_INFO 3
//#define LOG_DEBUG 4
//#define LOG_ALL 255

#define CUR_MODULE_PATH				"curpath"			//默认模块当前地址
#define DEFAULT_LOG_CONFIG			"log.conf"			//默认日志配置文件
#define DEFAULT_LOG_NAME			"cssm.log"			//默认日志文件名
#define DEFAULT_LOG_LEVEL			LOG_DEBUG//LOG_INFO			//默认日志级别
#define DEFAULT_LOG_FILE_MAX_LEN	(5*1024*1024)		//默认日志文件最大值

typedef struct log{
	char logtime[32];
	char filepath[MAX_FILE_PATH];
	FILE *logfile;
}LOG;

typedef struct logseting{
	char filepath[MAX_FILE_PATH];
	unsigned int maxfilelen;
	unsigned char loglevel;
}LOGSET;


typedef struct  Mutex {
	void (*init)(struct  Mutex *it);
	void (*uninit)(struct  Mutex *it);
	void (*lock)(struct  Mutex *it);
	void (*unlock)(struct  Mutex *it);

#if defined(WIN32) || defined(_WINDOWS_)
	CRITICAL_SECTION underlyingMutex_;
#else
	pthread_mutex_t underlyingMutex_;
#endif
	
}MUTEX; // class Mutex

extern int initlog(/*char *path*/);
extern void logheader();//第一次执行initlog时调用（防止多次初始化引起的日志隔断）
extern int c_logger(char* file, int nLine, int level, int status, const char *fmt, ...);
extern int exitlog();

#define log_error(fmt, ...)   	c_logger(CRYPTLOG_MARK, LOG_ERROR, 0, fmt, __VA_ARGS__)
#define log_warn(fmt, ...)   	c_logger(CRYPTLOG_MARK, LOG_WARN, 0, fmt, __VA_ARGS__)
#define log_info(fmt, ...)   	c_logger(CRYPTLOG_MARK, LOG_INFO_Ex, 0, fmt, __VA_ARGS__)
#define log_debug(fmt, ...)		c_logger(CRYPTLOG_MARK, LOG_DEBUG, 0, fmt, __VA_ARGS__)

//extern int logfuncbegin();
//extern int logfuncend();
//#define stack_trace				logfuncbegin()
//#define stack_trace_end			logfuncend()

#endif /* __LOG_H__ */