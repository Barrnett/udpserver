#ifndef TOOLS_H_
#define TOOLS_H_

#include  <stdio.h>
#include  <string.h>
#include  <vector>
#include  <string>
#include  <algorithm> 
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <iconv.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;
typedef unsigned char byte;

namespace Tools
{
	vector<string> split(const char *str,const char *pattern);
	void   Split(vector<string>& vec, const string& strSource, const string& strDelim);
	void   Split(vector<string>& vec, const string& strSource, char cDelim);
	char*  SplitStr(char *string, char *begin, char *end);
	bool   replaceStr(char *string, char *begin, char *end, char *replace);
	char*  str_replace(const char *s, const char * f, const char * r);
//	inline string&   ltrim(string &ss);
//	inline string&   rtrim(string &ss) ;
	inline string&   trim(string &st);
	string Trim_(string &str,const string strToken);
	string TrimLeft(string &strValue, string strToken);
	string TrimRight(string &strValue, string strToken);
	string Trim(string &strValue, string strToken);
	int    GetCurTimeString(char *lpszTime);
	void   GetCurTimeString(char* lpszTime, char* pFormat, int charCount);
	int    GetCurDataString(char *lpszTime);
	int    GetFormatTimeString(char *lpszTime);
	char*  Del_char(char* szBuffer, char del_char);
	void   RemoveBlank(char* szBuffer);
	void   RemoveBlank(char* szBuffer, char*& szReturnPoint);
	bool   isnum(const char* str);
	bool   EraseAll(string& strSource, char cDelim);
	int    API_StringToTimeEX(const string &strDateStr,time_t &timeData);
	/*提取路段名称*/
	bool   FetchFileName(string strFileName, char cDelim, int OrderNum, string &strFetchName);
	/*提取文件的名称*/
	int    GetNextIntTimeId(int nCurTimeId,  int nOutputInterval, int nTimeIdInterval);
	/*计算生成路况的当前timeid*/
	int    GetIntPathTimeId(int nNextTimeId,  int nOutputInterval, int nTimeIdInterval);
	/*把int转换成string类型*/
	string int2str(int n);
	string byte2str(byte n);
	/*double转换成string*/
	string doubleToString(double num);
	/*得到本地的ip地址*/
	bool GetHostAddress(char*  szhostaddr);
	/*判断ip地址的合法性*/
	bool bCheckIP(char* szip);
	/*获得程序的当前目录*/
	string getMyDirectory();
	/*char转换成double*/
	void charToDouble(const char* ch, double& res );
	//字符编码转换
	bool code_convert(const char *from_charset, const char *to_charset, char *inbuf, size_t inlen,char *outbuf, size_t outlen);
	//UTF8格式转换为GBK
	void UTF8ToGBK(char *&szOut);
	//GBK格式转换为UTF格式
	void GBKToUTF8(char* &szOut);
	string Extract(const char *string, char *begin, char *end);
	unsigned int string_to_unsigned_int(string str);
	string strrev(const char * strSource);
}
#endif