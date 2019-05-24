#pragma once
#include <string>
#include <list>
#include <map>
namespace _mxcomm
{
/**  
* @brief 字符串和列表的操作类.
*  
* 封装了一些string ,list map的通用函数
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
* @brief                         字符串替换
*
*
*
* @param szData					 [in]szData 字符串
* @param szSrc                   [in]被替换字符串
* @param szDes					 [in]替换字符串
* @return                        被替换过的新字符串
*/
	static std::string ReplaceAll( std::string szData,std::string szSrc,std::string szDes );
/**
* @brief                        字符串转换成map  
*
*								a=b\0c=d\0\0格式的字符串转换成map，结尾以\0\0结束
*
* @param pData					 [in] 字符串
* @param nDataLen                [in]字符串长度
* @param mapStr					 [out]形成的map
*/
    static std::string ReplaceJson( std::string szJsonData);
	static void MakeMapByZero(const char *pData ,int nDataLen, std::map<std::string,std::string>&mapStr);
public://list
/**
* @brief                        按照分隔符将字符串转换成List  
*								假设分隔符为 ';' 字符串为"ab;cde;fgds;dddd" 被分割为 "ab","cde","fgds","dddd"
*								
*
* @param strSource   			 [in] 字符串
* @param lstStr                  [out]形成的List
* @param strTok					 [in]分割字符
*/

	static void MakeListByTok(std::string strSource, std::list<std::string>&lstStr,char strTok);
	static void MakeIntListByTok(std::string strSource, std::list<int>&lstStr,char strTok);

/**
* @brief                        按照\0将字符串转换成List  
*							     字符串为"ab\0cde\0fgds\0dddd\0\0" 被分割为 "ab","cde","fgds","dddd"
*								
*
* @param pSource   				[in] 字符串
* @param lstStr                  [out]形成的List

*/
	static void MakeListByReturn(const char * pSource,std::list<std::string>&lstStr);
/**
* @brief                        按照\0 List 将转换成 字符串 最后\0\0结尾
*							     
*								
*
* @param lstStr   				[in] List
* @param pSource   				[out] 返回字符串
* @param nLen                  [out]pSource的长度

*/
		static bool MakeReturnStringByList(std::list<std::string>lstStr,char * pSource,int &nLen);
/**
* @brief                        按照\0 List 将转换成  字符串 最后\0\0结尾，计算一下长度
*							     
*								
*
* @param lstStr   				[in] List
* @Return                       [out]返回长度

*/

	static int  CalListSize(std::list<std::string>lstStr);

	static void MakeStringByList(std::string& strRet,const std::list<std::string>&lstStr,char strTok);
	static void MakeStringByIntList(std::string& strRet,std::list<int>&lstStr,char strTok);

	static  bool CompareNoCase(const string &strA,const string &strB);
	static  std::string StrToLower(const string &str);
public: //map
	//static bool LookUp(std::map<std::string ,std::string>mapA, )
/**
* @brief                         模拟CMap的lookup功能，将 map中和key对应的value找到
*
* @param mapA					 [in]map变量
* @param firs                    [in]key值
* @param sec					 [out]返回的value值
* @return                        true代表找到，false代表没有找到
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
* @brief                         map复制
* @param mapSrc					 [in]源map
* @param mapDest                 [out]目标map
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
* @brief                         判断2个map是否相同
* @param mapSrc					 [in]源map
* @param mapDest                 [in]目标map
* @return                        true代表相同，false代表不相同
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
* @brief                         比较2个map，
* @param mapNew					 [in]新map
* @param mapOld                  [in]老map
* @param mapAddEdit				 [out]增加或者修改的map数据
* @param lstIndex                [out]被删除的map的key
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
* @brief                         将一个list的类付给目标List，类是重新生成的
* @param lstSrc					 [in]源list
* @param lstDest                  [in]目的list
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
* @brief                         将map形成字符串
*
*								 map 生成 k1=v1\0k2=v2\0\0的字符串
*
* @param mapStr					 [in]map
* @param ppData                  [out]字符串
* @param nDataLen				 [out]字符串长度
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


