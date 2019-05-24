#ifndef __LIBCHEN_CONFIG_H__
#define __LIBCHEN_CONFIG_H__

#ifndef WIN32
	#include "Linux_Common.h"     
#endif

#include <string>
#include <map>
#include <list>

using namespace std;

namespace chen{


	class CIniFile
	{
	public:
		CIniFile(void);
		CIniFile(const char * chFileName);
		CIniFile(const string &strFileName);
		~CIniFile(void);


		int Init();
		int Init(const char* chFileName);
		int Init(const string& strFileName);
		int Dump();


		int ReadItem(const string& strSection, const string& strKey, string& strValue);
		int WriteItem(const string& strSection, const string& strKey, const string& strValue);

		//////////////////////by chen///////////////////
		int SaveIni();
		void PutIniSetting(const char *pszSection, const char *pszKey=NULL, const char *pszValue="");
		std::string GetIniSetting(const char *pszSection, const char *pszKey, const char *pszDefaultVal="");
		void RemoveIniSetting(const char *pszSection, const char *pszKey);
		bool GetSection(const char *pszSection,map<string, string >&keymap);
		bool GetSectionNames(std::list<std::string > &namelist);

		/////////////////////////////////////

	private:
		int LoadFile();
		int WriteFile();

		int ReadLine(FILE* pFile, string& strLine);
		int TrimString(string& strToken);

	private:
		static const int BUFFER_LEN = 1024;
		string m_strFileName;
		typedef map<string, string> ConfigType;
		map<string, ConfigType> m_mSec2Config;

		bool m_isChange;   //by chen


	};
}
#endif /* __LIBCHEN_CONFIG_H__ */