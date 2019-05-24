#ifndef __FILEREAD__H__
#define __FILEREAD__H__

#include<iostream>
#include<string>
#include<map>
#include<fstream>

using namespace std;

typedef map<string,string> MapStr2Str;
class ReadConfig
{
public:
	ReadConfig(string fileName)
	{
		m_strFileName = fileName;
	}
	bool getContent();//读取配置文件内容
	void formatContent(string Str);//把字段名和值拆分为键值对放入map。
	
	MapStr2Str m_mapConfig;
private:
	string m_strFileName;
};

class NetConfig
{
public:
	NetConfig();
	~NetConfig();
	
	void loadConfig(ReadConfig & config);
public:
	string m_strServIp;//服务器ip
	unsigned int m_nServPort;//服务器端口
	string m_strDBIp;//数据库ip
	unsigned int m_nDBPort;//数据库端口
	unsigned int m_nClientPort;//客户端端口
};

#endif
