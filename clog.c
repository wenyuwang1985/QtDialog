 /** log.c **/

#include "clog.h"

#define MAXLEVELNUM (3)

LOGSET logsetting;
LOG loging;
MUTEX mutex;
 
const static char LogLevelText[5][10]={"NONE","ERROR","WARN","INFO","DEBUG"};
 
static char * getdate(char *date);
 
static void mutexinit(MUTEX *it) {
#if defined(WIN32) || defined(_WINDOWS_)
	InitializeCriticalSection(&(it->underlyingMutex_));
#else
	pthread_mutex_init(&(it->underlyingMutex_), NULL);
#endif
}

static void mutexuninit(MUTEX *it) {
#if defined(WIN32) || defined(_WINDOWS_)
	DeleteCriticalSection(&(it->underlyingMutex_));
#else
	pthread_mutex_destroy(&(it->underlyingMutex_));
#endif

}

static void mutexlock(MUTEX *it) {
#if defined(WIN32) || defined(_WINDOWS_)
	EnterCriticalSection(&(it->underlyingMutex_));
#else
	pthread_mutex_lock(&(it->underlyingMutex_));
#endif
}

static void mutexunlock(MUTEX *it) {
#if defined(WIN32) || defined(_WINDOWS_)
	LeaveCriticalSection(&(it->underlyingMutex_));
#else
	pthread_mutex_unlock(&(it->underlyingMutex_));
#endif
}

static unsigned char getcode(char *path){
	unsigned char code=255;
	if(strcmp("INFO",path)==0)
		code=3;
	else if(strcmp("WARN",path)==0)
		code=2;
	else if(strcmp("ERROR",path)==0)
		code=1;
	else if(strcmp("NONE",path)==0)
		code=0;
	else if(strcmp("DEBUG",path)==0)
		code=4;
	return code;
}


static int createfolder(char* path){
	char logpath[MAX_FILE_PATH] = {0x0};
	char *tmp = NULL;

	strcpy(logpath, path);
	tmp = strrchr(logpath, '/');
	if (!tmp) {
		tmp = strrchr(logpath, '\\');
	}
	tmp[1] = 0; 

	if(access(logpath, 0) == -1){
#if defined(WIN32) || defined(_WINDOWS_)
		if (_mkdir(logpath) == -1)
#endif
			return -1;
	}
	return 0;
}


static unsigned char readlogconfig(char *path){
	char value[512] = {0x0};
	char data[50] = {0x0};
	unsigned int maxsize = 0;
	int level = 0;

	FILE *fpath = fopen(path, "r");
	if(fpath == NULL)
		return -1;
	fscanf(fpath, "logpath=%s\n", value);
	//getdate(data);
	//strcat(data,".log");
	//strcat(value,"/");
	//strcat(value,data);
	if(strcmp(value, CUR_MODULE_PATH)!=0){	
		if (!createfolder(value))
			memcpy(logsetting.filepath, value, strlen(value));
	}else{
		char *tmp = strrchr(path, '/');

		if (!tmp) {
			tmp = strrchr(path, '\\');
		}
		tmp[1] = 0; 
		strcat(path, DEFAULT_LOG_NAME);
		strcpy(logsetting.filepath, path);
	}
	memset(value, 0, sizeof(value));
 
	fscanf(fpath, "filemaxsize=%d\n", &maxsize);
	
	logsetting.maxfilelen = maxsize;

	fscanf(fpath, "loglevel=%d\n", &level);
	logsetting.loglevel = level;//getcode(level);
	fclose(fpath);

	return 0;
}

/*
 *日志设置信息
 * */
static void getlogconfig(){
 	char path[MAX_FILE_PATH] = {0x0};
	char confpath[MAX_FILE_PATH] = {0x0};
	char* tmp;

#if defined(WIN32) || defined(_WINDOWS_)
	int rv = GetModuleFileName(NULL, path, MAX_FILE_PATH);
	if(rv == 0){
		return;
	}
#else
	Dl_info dl_info;
	int rv = dladdr((void *)getlogconfig, &dl_info);
	if(rv == 0){
		printf("get library path failed, cannot get so self path");
		return;
	}
	strcpy(path, dl_info.dli_fname);
#endif
	tmp = strrchr(path, '/');
	if (!tmp) {
		tmp = strrchr(path, '\\');
	}
	tmp[1] = 0; 
	strcpy(confpath, path);

 	strcat(confpath, DEFAULT_LOG_CONFIG);
 	if(access(confpath, 4)!=0 || readlogconfig(confpath)!=0){
		strcat(path, DEFAULT_LOG_NAME);
		strcpy(logsetting.filepath, path);
		logsetting.loglevel = 3;//DEFAULT_LOG_LEVEL;
		logsetting.maxfilelen = DEFAULT_LOG_FILE_MAX_LEN;
 	}
 	return;
 }


/*
 *获取时间
 * */
static void settime(){
	time_t timer = time(NULL);
	strftime(loging.logtime, 20, "%Y-%m-%d %H:%M:%S", localtime(&timer));
}

static void logtime(char *result) 
{
#if defined(WIN32) || defined(_WINDOWS_)
	SYSTEMTIME system_time = { 0 };
	GetLocalTime(&system_time);
	_snprintf(result, 32, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
		system_time.wYear, system_time.wMonth, system_time.wDay, 
		system_time.wHour, system_time.wMinute, system_time.wSecond,
		system_time.wMilliseconds);
#else
	char loctime[32] = {0};
	struct timeval tv;
	struct timezone tz;
	gettimeofday (&tv , &tz);
	strftime(loctime, 20, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
	sprintf(result, "%s.%03ld", loctime, tv.tv_usec/1000);
#endif
}

//static 
void logheader()
{
	if(loging.logfile == NULL)
		loging.logfile = fopen(logsetting.filepath,"a+");
	if(loging.logfile == NULL){
		return;
	}

	settime();
	fprintf(loging.logfile, "\r-------------Module restart -----------------Log begin at %s-----------------\n\r", loging.logtime);
	fflush(loging.logfile);
	//日志关闭
	
	fclose(loging.logfile);
	loging.logfile = NULL;
}

/*
 *获取日期
 * */
static char * getdate(char *date){
	time_t timer = time(NULL);
	strftime(date, 11, "%Y-%m-%d", localtime(&timer));
	return date;
}

/*
 *不定参打印
 * */
static void printlog(FILE *fp, char* file, int line, int level, int status, const char * format, va_list args){
	int d;
	char c,*s;

	time_t timer = time(NULL);
	char curtime[32];

	logtime(curtime); 
	fprintf(loging.logfile, "[%s] ", curtime);

#if defined(WIN32) || defined(_WINDOWS_)
	fprintf(loging.logfile, "[%d] ", GetCurrentThreadId());
#else
	fprintf(loging.logfile, "[%ld] ", pthread_self());
#endif
	fprintf(loging.logfile, "[%s] ", LogLevelText[level]);

    if (file) {	
        char *e = strrchr(file, '/');

        if (!e) {
            e = strrchr(file, '\\');
        }

		if(e != NULL)
			fprintf(loging.logfile, "%s(%d): ", &e[1], line);
		else
			fprintf(loging.logfile, "%s(%d): ", file, line);

    }	
	vfprintf(loging.logfile, format, args);

	fprintf(loging.logfile,"%s","\n");
}

int initlog() {
	mutex.init = &mutexinit;
	mutex.uninit = &mutexuninit;
	mutex.lock = &mutexlock;
	mutex.unlock = &mutexunlock;

	mutex.init(&mutex);

	getlogconfig();
	//logheader();//防止多次初始化引起的日志隔断

	return 0;
}

int exitlog() {

	mutex.uninit(&mutex);
	
	memset(&logsetting, 0x00, sizeof(LOGSET));
	if(loging.logfile != NULL){
		fclose(loging.logfile);
		loging.logfile = NULL;
	}
	memset(&loging, 0x00, sizeof(LOG));

	return 0;
}

/*
 *日志写入
 * */

int c_logger(char* file, int nLine, int level, int status, const char *fmt, ...)
{
	int rtv = -1;
	va_list args;
	long lenFile;
	
	do{
		if(level > logsetting.loglevel)
			break;
		//打开日志文件
		mutex.lock(&mutex);
		if(loging.logfile == NULL)
			loging.logfile = fopen(logsetting.filepath, "a+");
		if(loging.logfile == NULL){
			mutex.unlock(&mutex);
			rtv = -1;
			break;
		}
		//打印日志信息
		va_start(args, fmt);
		printlog(loging.logfile, file, nLine, level, status, fmt, args);
		va_end(args);
		//文件刷出
		fflush(loging.logfile);
		//日志关闭
		if (loging.logfile != stdout) {
			fseek(loging.logfile, 0L, SEEK_END);
			lenFile = ftell(loging.logfile);
			fclose(loging.logfile);
			// 如果日志大于10M,则重命名
			if (lenFile > logsetting.maxfilelen) {
				remove(logsetting.filepath);
			}
		}
		//if(loging.logfile!=NULL)
		//	fclose(loging.logfile);
		loging.logfile = NULL;
		mutex.unlock(&mutex);
		rtv = 0;
	}while(0);
	
	
	return rtv;
}
//
//int logfuncbegin()
//{
//	c_logger(__FILE__, __LINE__, LOG_DEBUG, 0, "[%s start]", __FUNCTION__);
//}
//int logfuncend()
//{
//	c_logger(__FILE__, __LINE__, LOG_DEBUG, 0, "[%s end]", __FUNCTION__);
//}