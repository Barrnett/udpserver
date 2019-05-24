#include "Tools.h"
using   namespace   std;

namespace Tools
{
	/*把一条gps数据放到了vector里*/
	vector<string> split(const char *str,const char *pattern)
	{
		//const char* convert to char*
		char * strc = new char[strlen(str)+1];
		strcpy(strc, str);
		vector<string> resultVec;
		char* tmpStr = strtok(strc, pattern);
		while (tmpStr != NULL)
		{
			resultVec.push_back(string(tmpStr));
			tmpStr = strtok(NULL, pattern);
		}

		delete[] strc;

		return resultVec;
	}

	/*把一条gps数据放到了vector里*/
	void Split(vector<string>& vec, const string& strSource, char cDelim)
	{
		string::size_type i = 0;
		string::size_type j = 0;
		for ( ; j = strSource.find(cDelim, i), j != string::npos; i = ++j)
		{
			string strTemp = strSource.substr(i, j-i);
			if (Trim(strTemp, " ").length() != 0)
				vec.push_back(Trim(strTemp, " "));
		}
		if (strSource.substr(i, strSource.length()).length() != 0)
		{
			string strTemp = strSource.substr(i, strSource.length());
			if (Trim(strTemp, " ").length() != 0)
				vec.push_back(Trim(strTemp, " "));
			
		}
	}
	/*把一个buf的数据分解成一条gps数据*/
	void Split(vector<string>& vec, const string& strSource, const string& strDelim)
	{
		string::size_type i = 0;
		string::size_type j = 0;
		for ( ; j = strSource.find(strDelim, i), j != string::npos; i = j + strDelim.length())
		{
			string strTemp = strSource.substr(i, j-i);
			if (Trim(strTemp, " ").length() != 0)
				vec.push_back(Trim(strTemp, " "));
		}
		if (strSource.substr(i, strSource.length()).length() != 0)
		{
			string strTemp = strSource.substr(i, strSource.length());
			if (Trim(strTemp, " ").length() != 0)
				vec.push_back(Trim(strTemp, " "));
		}	
	}
	/*截取字符串从begin到字符串end*/
	/*
	实例：string = nihao,sunshubao|sucuilan.
	      SplitStr(string, NULL, ",") 
		  输出 ：nihao
		  SplitStr(string, ",", ".") 
		  输出 ：sunshubao|sucuilan
		  SplitStr(string, "|", NULL) 
		  输出 ：sucuilan.
	【备注】：把申请出来的内存删除
	*/
	char* SplitStr(char *string, char *begin, char *end)
	{
		//判断条件
		if (NULL == string || (NULL == begin  && NULL == end))
		{
			return NULL; //错误的参数
		}

		char *result = NULL;
		int resultCount = 0;
		char *beginChar = NULL;
		char *endChar = NULL;

		//第一种情况：begin和end都不为空
		if (NULL != begin  && NULL != end)
		{
			beginChar = strstr(string, begin);
			endChar = strstr(string, end);
			if (!beginChar || !endChar || endChar - beginChar < 0)
			{
				return NULL;
			}

			beginChar += 1;
			resultCount = endChar - beginChar;
		}

		//第二种情况：begin不为空,end为空
		else if (NULL != begin && NULL == end)
		{
			beginChar = strstr(string, begin);
			if (!beginChar)
			{
				return NULL;
			}

			beginChar += 1;
			resultCount = strlen(beginChar);
		}

		//第三种情况：begin为空,end不为空
		else if (NULL == begin && NULL != end)
		{
			endChar = strstr(string, end);
			if (!endChar)
			{
				return NULL;
			}

			beginChar = string;
			resultCount = strlen(string) - strlen(endChar);
		}


		/* 多申请一个字节，用于存放结束句'\0' */
		result = (char *)malloc(sizeof(char) * resultCount + 1);
		memset(result, 0, sizeof(result));
		if(NULL == result)
		{
			printf("Error malloc memory in function of subStringByIndex.\n");
			return NULL;
		}

		strncpy(result, beginChar, resultCount);
		result[resultCount] = '\0';

		return result;
	}
	/*替换字符串从begin到字符串end*/
	/*
	实例：string = sunshubao+sucuilan=sunshiyu
	      SplitStr(string, NULL, +," ，"nihao") 
		  输出 ：string : nihao+sucuilan=sunshiyu
		  SplitStr(string, "=", NULL, "nihao") 
		  输出 ：string : sunshubao+sucuilan=nihao
		  SplitStr(string, "+", "=", "nihao") 
		  输出 ：string : sunshubao+nihao=sunshiyu.
	*/
	bool replaceStr(char *string, char *begin, char *end, char *replace)
	{
		//第一步:判断条件
		if (NULL == string || (NULL == begin  && NULL == end) || NULL == replace)
		{
			return false; //错误的参数
		}

		char *result = NULL;
		int resultCount = 0;
		char *beginChar = NULL;
		char *endChar = NULL;

		//第一种情况：begin和end都不为空
		if (NULL != begin  && NULL != end)
		{
			beginChar = strstr(string, begin) + 1;
			endChar = strstr(string, end);
			if (!beginChar || !endChar || endChar - beginChar < 0)
			{
				return false;
			}
			
			resultCount = (beginChar - string) + strlen(replace) + strlen(endChar);
		}
		//第二种情况：begin不为空,end为空
		else if (NULL != begin && NULL == end)
		{
			beginChar = strstr(string, begin) + 1;
			if (!beginChar)
			{
				return false;
			}
			resultCount = (beginChar -  string) + strlen(replace);
		}
		//第三种情况：begin为空,end不为空
		else if (NULL == begin && NULL != end)
		{
			endChar = strstr(string, end);
			if (!endChar)
			{
				return false;
			}
			resultCount = strlen(replace) + strlen(endChar) ;
		}

		/* 多申请一个字节，用于存放结束句'\0' */
		result = (char *)malloc(sizeof(char) * resultCount + 1);
		memset(result, 0, resultCount + 1);
		if(NULL == result)
		{
			printf("Error malloc memory in function of subStringByIndex.\n");
			return false;
		}

		if (beginChar)
			memcpy(result, string, beginChar-string);
		strcat(result, replace);
		if (endChar)
			strcat(result, endChar);
		result[resultCount] = '\0';

		//从新替换数据
		if ((int)strlen(string) < resultCount)
		{
			delete result;
			return false;
		}
		strcpy(string, result);
		delete result;
		return true;
	}

	/*将字符串s中所有f的内容替换成r的内容*/
	char * str_replace(const char * s, const char * f, const char * r)
	{
		int inum = 1;
		int if_len, ir_len;
		char * ps;
		char * pt;
		char * pn;
		char * pp;

		if(s==0 || f==0 || r==0) return 0;
		if(s[0]==0 || f[0]==0) return 0;

		if_len = strlen(f);
		ir_len = strlen(r);

		for(ps=(char *)s;;)
		{
			pt = strstr(ps, f);
			if(pt == 0)
			{
				inum += strlen(ps);
				break;
			}
			inum += (pt - ps) + ir_len;
			ps = pt + if_len;
		}

		pn = pp = (char *)malloc( sizeof(char) * inum );

		if(pn == 0) return 0;
		pn[inum-1] = '\0';

		for(ps=(char *)s;;)
		{
			pt = strstr(ps, f);
			if(pt == 0)
			{
				memcpy(pp, ps, strlen(ps));
				break;
			}

			memcpy(pp, ps, pt - ps);
			pp += pt - ps;
			memcpy(pp, r, ir_len);
			pp += ir_len;
			ps = pt + if_len;
		}

		return pn;
	}

	/*去前后空格,注意：必须unicode*/
	/*inline string& trim(string &st) 
	{ 
		ltrim(rtrim(st)); 
		return   st; 
	}*/
	/*去掉前后空格*/
	string Trim_(string &str,const string strToken)
	{
		str.erase(0,str.find_first_not_of(strToken));
		str.erase(str.find_last_not_of(strToken)+1);
		return str;
	}
	//去除首上的指定字符
	string TrimLeft(string &strValue, string strToken)
	{
		if(strValue.size() != 0)
			strValue.erase(0,strValue.find_first_not_of(strToken));
		return strValue;
	}
	//去除尾上的指定字符
	string TrimRight(string &strValue, string strToken)
	{
		if(strValue.size() != 0)
			strValue.erase(strValue.find_last_not_of(strToken)+1,strValue.size()-strValue.find_last_not_of(strToken)-1);
		return strValue;
	}
	//去除首尾上的指定字符
	string Trim(string &strValue, string strToken)
	{
		TrimLeft(strValue,strToken);	
		return TrimRight(strValue,strToken);
	}
	/*去除指定字符成功返回true，失败返回false*/
	bool EraseAll(string& strSource, char cDelim)
	{
		bool    bIsFind = false;

		if (strSource.empty())
			return bIsFind;

		string::iterator iter;
		for (iter = strSource.begin(); iter != strSource.end(); ++iter)
		{
			if ((*iter) == cDelim)
			{
				iter = strSource.erase(iter);
				iter--;
				bIsFind = true;
			}
		}
		return bIsFind;
	}

	int GetCurTimeString(char *lpszTime)
	{
		struct tm *lt;
		time_t    nNow;

		time(&nNow);   
		lt = localtime(&nNow);
		sprintf(lpszTime, "%04d%02d%02d%02d%02d%02d", lt->tm_year+1900,
			lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
		return 0;
	}

    void GetCurTimeString(char* lpszTime, char* pFormat, int charCount)
	{
		struct tm *lt = NULL;
		time_t    nNow;
		char      szTime[32] = {0};

		time(&nNow);   
		lt = localtime(&nNow);

		sprintf(szTime,
				pFormat,
				lt->tm_year+1900,
				lt->tm_mon+1,
				lt->tm_mday,
				lt->tm_hour,
				lt->tm_min,
				lt->tm_sec);
		if (charCount == 0)
		{
			strcpy(lpszTime, szTime);
		}
		else
		{
			strncpy(lpszTime, szTime, charCount);
		}
	}

	int GetCurDataString(char *lpszTime)
	{
		struct tm *lt;
		time_t   nNow;

		time(&nNow);   
		lt = localtime(&nNow);
		sprintf(lpszTime, "%04d%02d%02d", lt->tm_year+1900,
			    lt->tm_mon+1, lt->tm_mday);
		return 0;
	}

	int GetFormatTimeString(char *lpszTime)
	{
		struct tm *lt;
		time_t   nNow;

		time(&nNow);   
		lt = localtime(&nNow);
		sprintf(lpszTime, "%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year+1900,
			lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min,lt->tm_sec);
		return 0;
	}

	/*去掉字符，tab键和0xff*/
	char* Del_char(char* szBuffer, char del_char)
	{
		if ( (szBuffer[0] == '\r') || (szBuffer[0]) == '\n' || (strlen(szBuffer) == 0) )
		{
			return szBuffer;
		}

		char szChar = (char)0xff;
		/*删除前面的空格*/
		for (unsigned int i = 0; ; ++i)
		{
			if ( (szBuffer[i] == '\t') || (szBuffer[i] == del_char) || (szBuffer[i] == szChar) )
			{
				continue;
			}
			else
			{
				szBuffer = &szBuffer[i];
				break;
			}
		}
		/*删除后面的空格*/
		for (size_t j = strlen(szBuffer); ; --j)
		{
			if ( (szBuffer[j-1] == '\t') || (szBuffer[j-1] == del_char) || (szBuffer[j-1] == szChar) )
			{
				continue;
			}
			else
			{
				szBuffer[j] = '\0';
				break;
			}
		}
		return szBuffer;
	}

	void RemoveBlank(char* szBuffer)
	{
		if (strcmp(szBuffer, "") ==0)
		{
			return;
		}

		char szChar = (char)0xff;
		for (unsigned int i = 0;; ++i)
		{
			if (szBuffer[i] == '\n' || i > strlen(szBuffer))
			{
				return;
			}
			else if (szBuffer[i] == '\t' || szBuffer[i] == ' '|| szBuffer[i] == szChar)
			{
				continue;
			}
			else
			{
				szBuffer = &szBuffer[i];
				return;
			}
		}

	}

	void RemoveBlank(char* szBuffer, char*& szReturnPoint)
	{
		if (strcmp(szBuffer, "") ==0)
		{
			return;
		}
		char szChar = (char)0xff;
		for (unsigned int i = 0;; ++i)
		{
			if (szBuffer[i] == '\n' || i > strlen(szBuffer))
			{
				return;
			}
			else if (szBuffer[i] == '\t' || szBuffer[i] == ' ' || szBuffer[i] == szChar)
			{
				continue;
			}
			else
			{
				szReturnPoint = &szBuffer[i];
				return;
			}
		}
	}


	bool isnum(const char* str) 
	{
		size_t strLength;
		strLength = strlen(str); //取得字符串长度
		for (size_t i=1;i<=strLength;i++) //循环判断每个字符是否为数字
		{
			if (str[i]<'0' && str[i]>'9') 
				return false;
		}
		return true;
	}
	/*
	string to time_t   
	时间格式 2009-3-24 0:00:08 或 2009-3-24
	*/
	int API_StringToTimeEX(const string &strDateStr,time_t &timeData)
	{
		char *pBeginPos = (char*) strDateStr.c_str();
		char *pPos = strstr(pBeginPos,"-");
		if(pPos == NULL)
		{
			printf("strDateStr[%s] err \n", strDateStr.c_str());
			return -1;
		}
		int iYear = atoi(pBeginPos);
		int iMonth = atoi(pPos + 1);
		pPos = strstr(pPos + 1,"-");
		if(pPos == NULL)
		{
			printf("strDateStr[%s] err \n", strDateStr.c_str());
			return -1;
		}
		int iDay = atoi(pPos + 1);
		int iHour=0;
		int iMin=0;
		int iSec=0;
		pPos = strstr(pPos + 1," ");
		//为了兼容有些没精确到时分秒的
		if(pPos != NULL)
		{
			iHour=atoi(pPos + 1);
			pPos = strstr(pPos + 1,":");
			if(pPos != NULL)
			{
				iMin=atoi(pPos + 1);
				pPos = strstr(pPos + 1,":");
				if(pPos != NULL)
				{
					iSec=atoi(pPos + 1);
				}
			}
		}

		struct tm sourcedate;
		sourcedate.tm_sec = iSec;
		sourcedate.tm_min = iMin; 
		sourcedate.tm_hour = iHour;
		sourcedate.tm_mday = iDay;
		sourcedate.tm_mon = iMonth - 1; 
		sourcedate.tm_year = iYear - 1900;
		timeData = mktime(&sourcedate);  
		return 0;
	}

	/*提取文件的名称*/
	bool FetchFileName(string strFileName, char cDelim, int OrderNum, string &strFetchName)
	{
		if (strFileName.empty())
			return false;

		vector<string>  vec;
		Split(vec, strFileName, '_');
		string::size_type pos = vec.at(OrderNum-1).find(".txt");
		if ( pos!= string::npos)
		{
			strFetchName = vec.at(OrderNum-1).substr(0, pos);
			return true;
		}
		return false;
	}
	/*计算下一个timeid*/
	int GetNextIntTimeId(int nCurTimeId,  int nOutputInterval, int nTimeIdInterval)
	{
		/*时间ID间隔（单位是：秒）*/
		//int nTimeIdInterval = 30;

		int iStep((int)(nOutputInterval/nTimeIdInterval));
		if (iStep == 0 || nCurTimeId == 0)
		{
			return -1;
		}

		int nRet(1);
		if ((nCurTimeId - 1) % iStep != 0)
		{
			nRet = ((nCurTimeId - 1)/iStep + 1) * iStep + 1;
		}
		else
		{
			nRet = nCurTimeId + iStep;
		}
		// 下一天
// 		if (nRet > 24*3600/nTimeIdInterval)
// 		{
// 			nRet = iStep;
// 		}
		return nRet;
	}

	/*计算生成路况的当前timeid*/
	int GetIntPathTimeId(int nNextTimeId,  int nOutputInterval, int nTimeIdInterval)
	{
		/*时间ID间隔（单位是：秒）*/
		int iStep((int)(nOutputInterval/nTimeIdInterval));
		if (iStep == 0 || nNextTimeId == 0)
		{
			return -1;
		}
		int nRet(1);
		if ((nNextTimeId - 1) % iStep != 0)
		{
			nRet = ((nNextTimeId - 1)/iStep) * iStep;
		}
		else
		{
			nRet = nNextTimeId -1;
		}
		return nRet;
	}
	/*把int转换成string类型*/
	string int2str(int n) 
	{
		bool bRet = false;
		char t[24];
		memset(t, 0, sizeof(t));
		int i = 0;

		while (n) 
		{
			t[i++] = (n % 10) + '0';
			n /= 10;
			bRet = true;
		}
		t[i] = 0;
		if (bRet)
			return string(strrev(t));
		else
			return string("0");
	}
	string byte2str(byte n)
	{
		char t[24];
		memset(t, 0, sizeof(t));
		int i = 0;

		while (n) 
		{
			t[i++] = (n % 10) + '0';
			n /= 10;
		}
		t[i] = 0;
		return string(strrev(t));
	}

	/*double转换成string*/
	string doubleToString(double num)
	{
		char str[256];
		sprintf(str, "%lf", num);
		string result = str;
		return result;
	}

	/*得到本地的ip地址*/
	bool GetHostAddress(char*  szhostaddr)
	{
		int sockfd;
		struct ifconf Ifconf;
		struct ifreq *pfreq;
		char buf[512];
		//初始化Ifconf
		Ifconf.ifc_len =512;
		Ifconf.ifc_buf = buf;
		
		if ((sockfd =socket(AF_INET,SOCK_DGRAM,0))<0)
		{
			return false;
		}
		ioctl(sockfd, SIOCGIFCONF, &Ifconf); //获取所有接口信息
		close(sockfd);
		
		pfreq = (struct ifreq*)Ifconf.ifc_buf;
		//获取机器ip地址
		char strIP[64];
		for(int i=0;i<(Ifconf.ifc_len/sizeof (struct ifreq));i++)
		{
			//for ipv4
			if((pfreq+i)->ifr_flags == AF_INET)
			{
				memset(strIP,0,64);
				strcpy(strIP,inet_ntoa(((struct sockaddr_in *)&((pfreq+i)->ifr_addr))->sin_addr));
				if(strcmp(strIP,"127.0.0.1") == 0)
				{
					continue;
				}
				else
				{
					strcpy(szhostaddr,strIP);
					return true;
				}
			}
        }
		return false;
    }
	/*判断ip地址的合法性*/
	bool bCheckIP(char* szip)
	{
		int   nRet = 0;
		if (inet_addr(szip) == INADDR_NONE)
		{
			return false;
		}
		for (size_t i = 0; i < strlen(szip); ++i)
		{
			if (szip[i] == '.')
				nRet ++;
		}
		if (nRet != 3)
		{
			return false;
		}
		return true;
	}
	/*获得程序的当前目录*/
	string getMyDirectory() 
	{ 
		string str; 
		char currentPath[1024];
		if(NULL == realpath("./",currentPath))
		{
			return NULL;
		}
		strcat(currentPath,"/");
		str = currentPath;
		return str; 
	}

	/*char 转换成double*/
	void charToDouble(const char* ch, double& res )
	{
		sscanf(ch,"%lf",&res);
	}

	//字符编码转换
	bool code_convert(const char *from_charset, const char *to_charset, char *inbuf, size_t inlen,char *outbuf, size_t outlen)
	{
		iconv_t cd;
		char **pin = &inbuf;    //需要转换的原字符集
		char **pout = &outbuf;  //转换为目标字符集
		
		cd = iconv_open(to_charset, from_charset);
		if (cd == 0)
		{
			return false;
		}
		memset(outbuf, 0, outlen);
		if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
		{
			return false;
		}
		iconv_close(cd);
		*pout = '\0';
		return true;
	}
	//UTF8格式转换为GBK
	void UTF8ToGBK(char *&szOut)
	{
		int lenth = strlen(szOut);
		if(lenth == 0)
		{
			return;
		}
		char strIn[4096] = {'\0'};
		strcpy(strIn,szOut);
		memset(szOut,'\0',lenth);
		
		code_convert("UTF-8", "GBK", strIn, lenth, szOut, lenth);
		return;
	}

	//GBK格式转换为UTF格式
	void GBKToUTF8(char* &szOut)
	{
		int lenth = strlen(szOut);
		if(lenth == 0)
		{
			return;
		}
		char strIn[4096] = {'\0'};
		strcpy(strIn,szOut);
		memset(szOut,'\0',lenth);
		
		code_convert("GBK", "UTF-8", strIn, lenth, szOut, lenth);
		return;
	}
	
	//提取begin和end之间的数据
	std::string Extract(const char *string, char *begin, char *end)
	{
		std::string temp = "";
		const char *beginChar = NULL;
		const char *endChar = NULL;
		const char *tempChar = NULL;
		int length = 0;

		//第一步:判断条件
		if (NULL == string || (NULL == begin  && NULL == end))
		{
			return temp; //错误的参数
		}

		beginChar = strstr(string, begin);
		if (!beginChar)
		{
			return temp;
		}
		else
		{
			beginChar += strlen(begin);
		}

		//去掉前面的空格
		while (*beginChar == ' ')
		{
			beginChar++;
		}

		endChar = strstr(beginChar, end);
		if (!endChar)
		{
			return false;
		}

		//去掉后面空格
		tempChar = endChar;
		while(*(--tempChar) == ' ')
		{

		}

		endChar = tempChar + 1;
		length = endChar - beginChar;
		if (!length)
		{
			return false;
		}

		for (int i = 0; i < length; i++)
		{
			temp += *beginChar++;
		}

		return temp;
	}

	//实现string到unsigned int的转换
	unsigned int string_to_unsigned_int(string str)
	{
		unsigned int result(0);//最大可表示值为4294967296（=2‘32-1）
		//从字符串首位读取到末位（下标由0到str.size() - 1）
		for (int i = str.size()-1;i >= 0;i--)
		{
			unsigned int temp(0),k = str.size() - i - 1;
			//判断是否为数字
			if (isdigit(str[i]))
			{
				//求出数字与零相对位置
				temp = str[i] - '0';
				while (k--)
					temp *= 10;
				result += temp;
			}
			else
				//exit(-1);
				break;
		}
		return result;
	}
	string strrev(const char * s)
	{
		string strIn = s;
		char temp;
		int len = strIn.length();
		for(int i=0;i<len/2;i++)
		{
			temp = strIn[i];
			strIn[i] = strIn[len-1-i];
			strIn[len-1-i] = temp;
		}
		return strIn;
	}
};