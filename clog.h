/*************************************************
 * �ļ���:  clog.h
 * ����: ������
 * ����: 2021-04
 * ��������: ��־�⡣1��֧��windows��linuxƽ̨
					 2��֧�ֶ��߳�
					 3��֧��ָ����־·����ѭ����д��������ơ�
					 4����C++��չ������cpplogex.h����֧�ֺ������
 * ����˵����1��Ĭ������������ģ��Ŀ¼����ͨ������DEFAULT_LOG_����ơ�
			 2������ѡ��������Ŀ¼������clog.conf����������־·�����ļ���С����־�����������ļ��ߴ���Ĭ������

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

#define CUR_MODULE_PATH				"curpath"			//Ĭ��ģ�鵱ǰ��ַ
#define DEFAULT_LOG_CONFIG			"log.conf"			//Ĭ����־�����ļ�
#define DEFAULT_LOG_NAME			"cssm.log"			//Ĭ����־�ļ���
#define DEFAULT_LOG_LEVEL			LOG_DEBUG//LOG_INFO			//Ĭ����־����
#define DEFAULT_LOG_FILE_MAX_LEN	(5*1024*1024)		//Ĭ����־�ļ����ֵ

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
extern void logheader();//��һ��ִ��initlogʱ���ã���ֹ��γ�ʼ���������־���ϣ�
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