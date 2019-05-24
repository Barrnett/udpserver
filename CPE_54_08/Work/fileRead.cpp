#include "fileRead.h"

bool ReadConfig::getContent()
{
	ifstream iConfig(m_strFileName.c_str());
	if( !iConfig.is_open() )
	{
		cout <<"config file open failed!"<<endl;
		return false;
	}
	
	string strLine;
	while( getline(iConfig,strLine) )
	{
		formatContent(strLine);
	}
	return true;
}

void ReadConfig::formatContent(string Str)
{
	string::size_type position = Str.find_first_of(":");
	string strKey = Str.substr(0,position);
	string strValue = Str.substr(position+1);
	m_mapConfig.insert(make_pair(strKey,strValue));
	return;
}

NetConfig::NetConfig()
{
	m_strServIp = "";
	m_nServPort = 0;
	m_strDBIp = "";
	m_nDBPort = 0;
	m_nClientPort = 0;
}

NetConfig::~NetConfig()
{
}

void NetConfig::loadConfig(ReadConfig & config)
{
	MapStr2Str & confMap = config.m_mapConfig;
	for(MapStr2Str::iterator iter=confMap.begin();iter!=confMap.end();iter++)
	{
		string strKey = iter->first;
		string strValue = iter->second;
		if(strKey == "serverIP")
		{
			m_strServIp = strValue;
		}
		else if(strKey == "serverPort")
		{
			m_nServPort = atoi(strValue.c_str());
		}
		else if(strKey == "DbIP")
		{
			m_strDBIp = strValue;
		}
		else if(strKey == "DbPort")
		{
			m_nDBPort = atoi(strValue.c_str());
		}
		else if(strKey == "clientPort")
		{
			m_nClientPort = atoi(strValue.c_str());
		}
	}
	return;
}
