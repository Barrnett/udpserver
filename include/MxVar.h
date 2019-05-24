#pragma once
//#include <windows.h>
#ifndef _MXVAR_FILE_
#define _MXVAR_FILE_


#include "BSMQDefine.h"
#include "bsapi.h"
#include "bserror.h"
#include "tabledb_def.h"
#include "treedb_def.h"
#include <string>
#include <list>
#include <map>
#include "StrWork.h"
#include "DbVar.h"
#include "WShowControl.h"
using namespace std;
class CDataRecord
{
public:
	FIELDVALUEMAP m_mapField ;
	void SetData(std::string sid,chen::VariableData rv);
	bool GetData(std::string sid,chen::VariableData& rv);
	//获取记录集中的Table数据，只允许存在一个
	bool GetTableVar(chen::VariableData& rv);   
};

class CListDataRecord
{
public:
	CListDataRecord();
	~CListDataRecord();
public:
	std::list<CDataRecord *> m_lstDataRecord;
};
class CMxVar
{
public:
	enum MQTYPE{ EXISTQ=1,NEWQ ,MUSTEXISTQ}; //EXISTQ 已存在queue，newq： new queue，mustexistq : 先按照已存在打开，如果失败，创建一个
	CMxVar(void);
	~CMxVar(void);
	//static int VariableDataToInt(chen::VariableData& rv);
	//static std::string VariableDataToString(chen::VariableData& rv);
	//static __int64 VariableDataToInt64(chen::VariableData& rv);
	static int WriteKeyString(string key,string str,BSHANDLE hd);
	static int WriteKeyInt(string key,int value,BSHANDLE hd);
	static int WriteKeyInt64(string key,__int64 value,BSHANDLE hd);
	static int WriteKeyProperty( BSHANDLE &hd,std::string PropertyName,chen::VariableData &rv);

	static int WriteKeyMap( BSHANDLE &hd,std::map<std::string ,chen::VariableData> mapVar);
	static int DeleteKeyList( BSHANDLE &hd,std::list<std::string > lstDel);

	static bool FindField(FIELDVALUEMAP &vm,std::string field,chen::VariableData &rv);
	static string GetKeyString(string key,string defaultret,BSHANDLE hd);
	static int GetKeyInt(string key,int defaultret,BSHANDLE hd);
	static int GetKeyMap( BSHANDLE &hd,std::map<std::string ,chen::VariableData>& mapVar );

	//static int GetIntFromProperty(string key,int defaultret, BSPROPERTYSET& PropList);
	//static string GetStringFromProperty(string key,string defaultret, BSPROPERTYSET& PropList);
	static int MapStringWrite(std::map<std::string,std::string>&mapA,BSHANDLE&hd );
	static int DeleteRealKey(BSHANDLE& hd,std::string szSubKey,int flag);
	static int ReopenNewKey(BSHANDLE &hd,std::string strSubKey,unsigned int flag);
	static int ReopenNewKey(BSHANDLE &hd,std::string strSubKey,std::string strSrcKey, unsigned int flag);
	static int ReopenSubKey(BSHANDLE &hd,std::string SubKey,unsigned int flag=TRDB_OPKF_OPENEXIST);

	static std::string GetParentEntityName(std::string szEn);
	
	//static int SerialString(char ** p,int nLen,std::string szData);
	//static int SerialInt(char ** p,int nLen,int nData);
	//static int SerialVar( char ** p,int nLen,chen::VariableData rv);
	//static int UnSerialVar( char **pSData,chen::VariableData&rv, int nDataLen);
	/**
	 * Method:    ReplaceStr
	 * FullName:  CMxVar::ReplaceStr
	 * Access:    public 
	 * @return:   std::string
	 * Qualifier:
	 * @Param: string & strSrc			源字符串
	 * @Param: const char * pOldStr		需要替换掉的字符
	 * @Param: const char * pNewStr		需替换后的字符
	*/
	static void FreeDataRecord( std::list<CDataRecord*>& lstData );
	static void DeleteAllProperty(BSHANDLE &hd);
	static int WriteKeyfloat(string key,float value,BSHANDLE hd);
	static int SelectKeyPropertys(BSHANDLE &hHandle,BSPROPERTYSET& promap);
	static int AppendSubKey(BSHANDLE &hd,std::string &subKey);
	static int SelectSubKeys(BSHANDLE &hd,std::list< std::string > &keylist);

	//--------------------------------MQ-----------------------------
	static int PushMQMessage(BSHANDLE &hd,const chen::VariableData &rv,std::string Label,int level=BS_MQ_COMMONPRIORITY);
	//static int PopMQMessage(BSHANDLE &hd,std::string &Message,std::string &Label,time_t &createTime,unsigned int timer=0);
	static int PopMQMessage( BSHANDLE &hd,chen::VariableData &rv,std::string &Label,time_t &cTime,unsigned __int64& uID,unsigned __int64 timer=BS_TIMER_INFINITE );
	static int PopMQMessage( BSHANDLE &hd,chen::VariableData &rv,std::string &Label,time_t &cTime,unsigned __int64 timer=BS_TIMER_INFINITE );
	
	static int DeleteMQ(BSHANDLE &hd);
	static int ClearMQ(BSHANDLE &hd);
	static int OpenMQ(std::string addr,std::string mqName,std::string PassWord,BSHANDLE &hd,unsigned int uOpflag=EXISTQ, unsigned  int uMQFlag=BSMQ_OT_COMMONMQ,int nPort=0);
	static int MQ_PushOneMsg(std::string szServer, std::string szQueue,std::string szLabel,chen::VariableData rv,int nType );

	//static int ReopenMQ( std::string mqName,std::string PassWord,BSHANDLE &hd,unsigned int flag/*=BSMQ_OF_OPENEXIST*/,int type/*=BSMQ_OT_COMMONMQ*/ );
	static std::string GetParentDataId(std::string szId);
	static int MQ_PopOneMsg(std::string szServer,std::string szRetQueue, int nRetType, int nWaitTimeOut,chen::VariableData& rvRet, std::string szReturnLabel,time_t& tmCreateTime,bool bDelAfterUsed);
public:
	// 
};

class CSaveShowCtlInfo
{
public:
	string		m_strName;
	string		m_strLabel;
	string		m_strHelp;
	string		m_strCtlType;
	string		m_strValue;
	string		m_strFollowName;
	int			m_nFollow;
	int			m_nMust;
	int			m_nReadOnly;
	int			m_nOnlyNum;
	int			m_nParamCount;
	int			m_nSort;
	int         m_nDisable;            //0表示可用，1表示不可用
	string      m_strDropDownName;     //下拉列表所使用的结构的名称，如果有多个以‘，’号分割
	string      m_strDisableName;      //例如此控件是checkbox，则此checkbox的勾选可能会影响的控件的列表,如果有多个以‘，’区分
	std::list<std::pair<std::string,std::string>> m_lstDropContent;
	int			m_nrows;
	int         m_ndefualtIndex;//对于
	CWShowControl*	m_pShowCtrl;
public:
	CSaveShowCtlInfo()
	{
		m_pShowCtrl=NULL;
		m_strName = "";
		m_strLabel = "";
		m_strHelp = "";
		m_strCtlType = "";
		m_strValue = "";
		m_strFollowName = "";
		m_nFollow = 0;
		m_nMust = 0;
		m_nReadOnly = 0;
		m_nOnlyNum = 0;
		m_nParamCount = 0;
		m_nSort = 0;
		m_lstDropContent.clear();
		m_nrows = 0;
		m_nDisable=0;
		m_strDropDownName="";
		m_strDisableName="";
		m_ndefualtIndex=0;
	}
	~CSaveShowCtlInfo()
	{
		m_lstDropContent.clear();
	}
	CSaveShowCtlInfo(const CSaveShowCtlInfo& info){
		m_pShowCtrl=info.m_pShowCtrl;
		m_strName = info.m_strName;
		m_strLabel = info.m_strLabel;
		m_strHelp = info.m_strHelp;
		m_strCtlType = info.m_strCtlType;
		m_strValue = info.m_strValue;
		m_strFollowName = info.m_strFollowName;
		m_nFollow = info.m_nFollow;
		m_nMust = info.m_nMust;
		m_nReadOnly = info.m_nReadOnly;
		m_nOnlyNum = info.m_nOnlyNum;
		m_nParamCount = info.m_nParamCount;
		m_nSort = info.m_nSort;
		m_lstDropContent.assign(info.m_lstDropContent.begin(),info.m_lstDropContent.end());
		m_nrows = info.m_nrows;
		m_nDisable= info.m_nDisable;
		m_strDropDownName=info.m_strDropDownName;
		m_strDisableName=info.m_strDisableName;
		m_ndefualtIndex=info.m_ndefualtIndex;
	}
};



#endif
