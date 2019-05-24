#pragma once
#include <string>
#include <list>
#include <map>
namespace _mxcomm
{
/**  
* @brief �ַ������б�Ĳ�����.
*  
* ��װ��һЩstring ,list map��ͨ�ú���
*  
*/  
using namespace std;
class CStrWork
{
public:
	CStrWork(void);
	~CStrWork(void);
public:
	static std::string Left(std::string szSrc,std::string::size_type nPos);
	static std::string Right(std::string szSrc,std::string::size_type nPos);
	static std::string GetMid(std::string szSrc,  std::string szBeginStr,std::string szEndStr);
	static std::string Mid(std::string szSrc,std::string::size_type nBegin,int nLen);
	static bool FormatString( std::string& szString, const char* pFormat, ... );
/**
* @brief                         �ַ����滻
*
*
*
* @param szData					 [in]szData �ַ���
* @param szSrc                   [in]���滻�ַ���
* @param szDes					 [in]�滻�ַ���
* @return                        ���滻�������ַ���
*/
	static std::string ReplaceAll( std::string szData,std::string szSrc,std::string szDes );
/**
* @brief                        �ַ���ת����map  
*
*								a=b\0c=d\0\0��ʽ���ַ���ת����map����β��\0\0����
*
* @param pData					 [in] �ַ���
* @param nDataLen                [in]�ַ�������
* @param mapStr					 [out]�γɵ�map
*/
    static std::string ReplaceJson( std::string szJsonData);
	static void MakeMapByZero(const char *pData ,int nDataLen, std::map<std::string,std::string>&mapStr);
public://list
/**
* @brief                        ���շָ������ַ���ת����List  
*								����ָ���Ϊ ';' �ַ���Ϊ"ab;cde;fgds;dddd" ���ָ�Ϊ "ab","cde","fgds","dddd"
*								
*
* @param strSource   			 [in] �ַ���
* @param lstStr                  [out]�γɵ�List
* @param strTok					 [in]�ָ��ַ�
*/

	static void MakeListByTok(std::string strSource, std::list<std::string>&lstStr,char strTok);
	static void MakeIntListByTok(std::string strSource, std::list<int>&lstStr,char strTok);

/**
* @brief                        ����\0���ַ���ת����List  
*							     �ַ���Ϊ"ab\0cde\0fgds\0dddd\0\0" ���ָ�Ϊ "ab","cde","fgds","dddd"
*								
*
* @param pSource   				[in] �ַ���
* @param lstStr                  [out]�γɵ�List

*/
	static void MakeListByReturn(const char * pSource,std::list<std::string>&lstStr);
/**
* @brief                        ����\0 List ��ת���� �ַ��� ���\0\0��β
*							     
*								
*
* @param lstStr   				[in] List
* @param pSource   				[out] �����ַ���
* @param nLen                  [out]pSource�ĳ���

*/
		static bool MakeReturnStringByList(std::list<std::string>lstStr,char * pSource,int &nLen);
/**
* @brief                        ����\0 List ��ת����  �ַ��� ���\0\0��β������һ�³���
*							     
*								
*
* @param lstStr   				[in] List
* @Return                       [out]���س���

*/

	static int  CalListSize(std::list<std::string>lstStr);

	static void MakeStringByList(std::string& strRet,const std::list<std::string>&lstStr,char strTok);
	static void MakeStringByIntList(std::string& strRet,std::list<int>&lstStr,char strTok);

	static  bool CompareNoCase(const string &strA,const string &strB);
	static  std::string StrToLower(const string &str);
public: //map
	//static bool LookUp(std::map<std::string ,std::string>mapA, )
/**
* @brief                         ģ��CMap��lookup���ܣ��� map�к�key��Ӧ��value�ҵ�
*
* @param mapA					 [in]map����
* @param firs                    [in]keyֵ
* @param sec					 [out]���ص�valueֵ
* @return                        true�����ҵ���false����û���ҵ�
*/

	template<class _INDEX, class _VAL>        
	static bool LookUp(typename std::map<_INDEX, _VAL> & mapA, _INDEX & firs, _VAL &sec)                
	{
		if (mapA.end() ==mapA.find(firs))
		{
			return false;
		}else
		{
			typename std::map<_INDEX, _VAL>::iterator it = mapA.find(firs);      
			sec =(*it).second;
			return true;
		}
		
	}
/**
* @brief                         map����
* @param mapSrc					 [in]Դmap
* @param mapDest                 [out]Ŀ��map
*/

	template<class _INDEX,class _VAL>
	static void SetSameMap(typename std::map<_INDEX ,_VAL>& mapSrc, typename std::map<_INDEX ,_VAL>& mapDest)
	{
		mapDest.clear();
		for(typename std::map<_INDEX ,_VAL>::iterator it=mapSrc.begin();it!=mapSrc.end();it++)
		{
			mapDest[(*it).first]=(*it).second;
		}
	}
/**
* @brief                         �ж�2��map�Ƿ���ͬ
* @param mapSrc					 [in]Դmap
* @param mapDest                 [in]Ŀ��map
* @return                        true������ͬ��false������ͬ
*/

	
	template<class _INDEX,class _VAL>
	static bool IsSameMap(typename std::map<_INDEX ,_VAL>& mapSrc, typename std::map<_INDEX ,_VAL>& mapDest)
	{
		int nSrcCount=mapSrc.size();
		int nDestCount=mapDest.size();
		if (nSrcCount!=nDestCount)
			return false;
		for(typename std::map<_INDEX ,_VAL>::iterator it=mapSrc.begin();it!=mapSrc.end();it++)
		{
			if (mapDest.find((*it).first)==mapDest.end())
				return false;
			if(mapDest[(*it).first]==(*it).second)
			{
			}else
				return false;
			
		}
		return true;
	}

	
/**
* @brief                         �Ƚ�2��map��
* @param mapNew					 [in]��map
* @param mapOld                  [in]��map
* @param mapAddEdit				 [out]���ӻ����޸ĵ�map����
* @param lstIndex                [out]��ɾ����map��key
* @return                        
*/

	template<class _INDEX,class _VAL>
	static void GetDiffMapInfo(class std::map<_INDEX ,_VAL>& mapNew, class std::map<_INDEX ,_VAL>& mapOld, class std::map<_INDEX ,_VAL>& mapAddEdit, class std::list<_INDEX>& lstIndex)
	{
		for(typename std::map<_INDEX ,_VAL>::iterator it=mapNew.begin();it!=mapNew.end();it++) 
		{
			if (mapOld.find((*it).first)==mapOld.end())
				mapAddEdit[(*it).first]=(*it).second;

			if(mapOld[(*it).first]==(*it).second)
			{
			}else
				mapAddEdit[(*it).first]=(*it).second;

		}
		for(typename std::map<_INDEX ,_VAL>::iterator it=mapOld.begin();it!=mapOld.end();it++)
		{
			if (mapNew.find((*it).first)==mapNew.end())
				lstIndex.push_back((*it).first);


		}
	}


/**
* @brief                         ��һ��list���ึ��Ŀ��List�������������ɵ�
* @param lstSrc					 [in]Դlist
* @param lstDest                  [in]Ŀ��list
* @return                        
*/
	template<class _INDEX>
	static void SetSamePointList(typename std::list<_INDEX*>& lstSrc, typename std::list<_INDEX* >& lstDest)
	{
		while(lstDest.size()>0)
		{
			_INDEX *pIndex= lstDest.front();
			delete pIndex;
			lstDest.pop_front();
		}
		for(typename std::list<_INDEX*>::iterator it=lstSrc.begin();it!=lstSrc.end();it++)
		{
			_INDEX *pIndex=*it;
			_INDEX  *pDesIn= new _INDEX();
			*pDesIn=*pIndex;
			lstDest.push_back(pDesIn);
			//mapDest[(*it).first]=(*it).second;
		}
	}
	template<class _INDEX>
	static void AddSamePointList(typename std::list<_INDEX*>& lstSrc, typename std::list<_INDEX* >& lstDest)
	{
		for(typename std::list<_INDEX*>::iterator it=lstSrc.begin();it!=lstSrc.end();it++)
		{
			_INDEX *pIndex=*it;
			_INDEX  *pDesIn= new _INDEX();
			*pDesIn=*pIndex;
			lstDest.push_back(pDesIn);
			//mapDest[(*it).first]=(*it).second;
		}
	}
	template<class _INDEX>
	static void ClearList(typename std::list<_INDEX*>& lstData)
	{
		while(lstData.size()>0)
		{
			_INDEX *pData=lstData.front();
			lstData.pop_front();
			delete pData;
		}
	}
	/**
* @brief                         ��map�γ��ַ���
*
*								 map ���� k1=v1\0k2=v2\0\0���ַ���
*
* @param mapStr					 [in]map
* @param ppData                  [out]�ַ���
* @param nDataLen				 [out]�ַ�������
*/
	static void MakeZeroCharByMap(std::map<std::string,std::string>mapStr, char **ppData ,int& nDataLen );
	static void MakeZeroCharByMap(std::map<std::string,std::string>mapStr, char * &ppData ,int& nDataLen );

	static std::string GeValFromMap(std::map<std::string,std::string> &mapParam, const std::string& szKey);
	static int SortString(string& szStrCompare1,string& szStrCompare2);
	static bool CCompareString(string& szStrCompare1,string& szStrCompare2);

	static bool mibcompare(string node1,string node2);
	static std::string Trim(std::string szSrc, const char *pStr );
	static std::string TrimLeft( std::string szSrc, const char *pStr );
	static std::string TrimLeft(std::string szSrc);
	static std::string TrimRight(std::string szSrc, const char *pStr);  
	static std::string TrimRight( std::string szSrc);
	static std::string Trim(std::string szSrc);
	static void AddList(std::list<std::string> & lstSrc,std::list<std::string> lstChild);

};
//std::string Left1(std::string szSrc,std::string::size_type nPos);
}


