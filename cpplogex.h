/*************************************************
 * 文件名:  cpplogex.h
 * 作者: 赵子轩
 * 日期: 2021-05
 * 内容描述: c++扩展clog（引入原日志库的stack_trace,支持函数监控)。

*************************************************/

extern "C"{
	#include "clog.h"
};

/*用于记录函数开始和结束,此部分为C++代码，可以放到引用到工程头文件中声明使用*/
#define  stack_trace	XTXStackTrace __xtx_stack_trace(__FILE__, __FUNCTION__, __LINE__)

class XTXStackTrace
{
public:
	explicit XTXStackTrace(const char *filename, const char *fun_name, const long& lineNum)
	{
		if(fun_name)
		{
			const char *p = strrchr(filename, '\\');
			if(p == NULL)p = filename;
			else p += 1;
			if(!p)return;

			memset(m_filename, 0, sizeof(m_filename));
			memcpy(m_filename, p, strlen(p));
			memset(m_fun_name, 0, sizeof(m_fun_name));
			memcpy(m_fun_name, fun_name, strlen(fun_name));
			m_lineNum = lineNum;
			m_start = GetTickCount();
			c_logger(m_filename,m_lineNum, LOG_DEBUG, 0, "[%s start]", m_fun_name);
		}
	}

	~XTXStackTrace()
	{
		if(m_fun_name)
		{
			int timeElp = GetTickCount() - m_start;
			c_logger(m_filename,m_lineNum, LOG_DEBUG, 0, "[%s end][Time consume %d ms]", m_fun_name, timeElp);
		}
	}

	int GetTickCount()
	{
		return int(double(clock()) * 1000 / CLOCKS_PER_SEC);
	};
private:
	char				m_filename[256];
	char				m_fun_name[256];
	long				m_lineNum;
	int 				m_start;
};