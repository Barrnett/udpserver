#ifndef _MXHANDLEPOOL_H_
#define _MXHANDLEPOOL_H_

#include "MyMutex.h"
#include "MxHandle.h"
#include "MxVar.h"
#include "VariableData.h"
#include "DataDefine.h"
#include "DBServer.h"
#include "bserror.h"
#include "treedb_def.h"
#include "DbVar.h"
#define MAX_FILE_HANLE_POOL_SIZE 50

//#define INIKEY				"MX_INI"
//#define ENTITYKEY			"EntityTemplate"
//#define MONITORKEY			"PluginData"
//#define MXSEKEY				"MXSE"
//#define NNMKEY				"NNM"
#define  MXINFODB           "mxinfo"    //licence���ڵ����ݿ�

#define  MASTERTREE			"master"
#define  BASETREE			"base"
#define  CCUTREE			"ccubase"

#define COMPANYEKEY			"Co"
#define DELETEDCOMPANYKEY	"Co_delete"
#define DATADB				"mxdb"
#define MONITORALERTMQ		"BMP_Alert"
#define ALERTMQ				"alertmq"
#define INIKEY				"Config"
#define ENTITYKEY			"EntityTemplate"
#define MONITORKEY			"PluginData"
#define MXSEKEY				"MXSE"
#define NNMKEY				"NNM"
#define MDYN				"MDYN"
#define BSVIEW              "BSView"
#define MMSGRECV			"MicroMsgReceiver"

#define ONLY_OPEN_EXIST 1 //tabledb���ݿ����treedb���ļ�ֻ���Ѵ��ڵ�
#define CREATE_IF_NOT_EXIST 2 //�ȳ��Դ�tabledb���ݿ����treedb���ļ�������������򴴽�

//Treedb����OpenKey Flagֻ�����֣�����·����ʽ
//���Ѿ����ڵļ�����б��·����ʽ���Ѵ��ڵļ�
//Treedb handle�ز����������޸�����
#define ExistDotPath		(TRDB_OPKF_DOTPATH|TRDB_OPKF_OPENEXIST)
#define ExistNodePath		(TRDB_OPKF_OPENEXIST)

#define DefaultMQFlag (BSMQ_OT_COMMONMQ|BSMQ_OT_INDEXRECORD) //����mq��Ĭ�Ϸ��,�����ȼ�����������
//
static const char mxRId[]								= "_FIELD_AUTOID_";//"mxrid",��¼ID
static const char AlertVersionField[]					="edit_version";
#define DB_CHECKRESULT(NRET)  if(NRET!=BS_NOERROR) return NRET;
#define DELETEHANDLEOBJ(p) {CDealHandleObj::DeleteHandleObj(p); p=NULL;}

#define HANDLELOGFILE "handle.log"

#define INVALID_DBUSER "This is invalid db user: 7&3@d(421$dgwr[xz} *_*"
#define INVALID_DBPWD "This is invalid db password: &de05lc}21!d9x***~~~&%.dg?ds1"

#define SubKeyNotExist(nRet) (BS_ERROR_BPTREENOFIND==nRet||BS_ERROR_PARAMETER==nRet ||BS_TRDB_NOFINDITEM==nRet)
#define HandleFailed(nRet) (BSAPI_ERROR_TRANHANDLEABORTED==nRet||BSAPI_ERROR_RECVDATA==nRet ||BSAPI_ERROR_SENDDATA==nRet)

class CDealHandleObj
{
public:
	CDealHandleObj();
	virtual ~CDealHandleObj();

	void AddReference();//ָ��ָ��һ������������øú�����ָ��ſ���
	static void DeleteHandleObj(CDealHandleObj* pObj);
public:
	chen::Mutex m_mutexRefNum;
	int m_nRefNum;//�ö����������
};
class CHandlePool: public CDealHandleObj
{
public:
	CHandlePool();
	virtual ~CHandlePool();

public:
	CConfHandle* Alloc(int nHdType,bool bFromFront);//Ĭ�ϴ�frontȡһ����Ч��handle
	void Free(CConfHandle* phd);
private:
	int m_nMaxSize;	//handle�صĴ�С
	int m_nExistNum; //��ǰ�Ѿ�new������handle������������ʹ�õĺ���m_lstFreeHanldes�б���Ŀ��е�handle

	chen::Mutex m_mutex;
	list<CConfHandle*> m_lstFreeHanldes;
};

////treedbһ���ļ��ϵ�handle��
//class CTreedbFilePool: public CHandlePool
//{
//public:
//	CTreedbFilePool(string szFile);
//	virtual ~CTreedbFilePool();
//
//	CTreedbHd* Alloc(bool bFromFront=true);
//public:
//	string m_szTreedbFileName;//treedb�ļ�����
//};

//tabledbһ�����ݿ��е�handle��
//class CTabledbPool: public CHandlePool
//{
//public:
//	CTabledbPool(string szDbName,string szUser,string szPwd);
//	virtual ~CTabledbPool();
//
//	//��handle��ͬʱ����û����������Ƿ����
//	int CheckAndOpenHandle(CTabledbHd* phd,string szHost,int nPort,
//		string szUser,string szPwd,int nOpFlag);
//	CTabledbHd* Alloc(bool bFromFront=true);
//public:
//	string m_szDbName;//treedb���ݿ�����
//	//�������ݿ����ȷ���û��������룬������֤�������û�������
//	//Ŀǰ��Ϊһ�����ݿ�ֻ��һ���û��������룬ֻҪͬʱ�����������Ϳ��Դ����ݿ�
//	string m_szUser;
//	string m_szPwd;
//	bool m_bIsValid;//������û����������Ƿ���Ч
//};
class CServerHandlePool: public CDealHandleObj
{
public:
	CServerHandlePool(string szHost,int nPort);
	~CServerHandlePool();

	CHandlePool* GetTreedbPool();
	CHandlePool* GetTabledbPool();
	CHandlePool* GetMQPool();
	CHandlePool* GetHandlePool(int nHdType);
public:
	CDBServer m_Server;
private:
	//treedb��handle��
	CHandlePool m_TreedbPool;//���е�treedb����һ����

	//tabledb��handle��
	CHandlePool m_TabledbPool;//���е�tabledb����һ����

	//MQ��handle��,CHandlePool���Ѿ�����
	CHandlePool m_MQPool;
};
class CGrossHanlePool: public CDealHandleObj
{
public:
	CGrossHanlePool();
	~CGrossHanlePool();

	CServerHandlePool* GetServerPool(CDBServer dbserver);
private:
	chen::Mutex mutex;
	map<CDBServer,CServerHandlePool*> m_mapGrossPool;//server -- server��handle��
};

class CMxHandleLoc
{
public:
	CMxHandleLoc();
	~CMxHandleLoc();

	CConfHandle* GetConfHandle();
	int GetHandleType();


	//��λ��һ������������Treedb_ReopenSubKey�ڸ��������ƶ�����ͬ�Ӽ����������������Ӽ��Ĳ��������øú���������������
	//Alloc�õ���һ����һ����Ч��handle
	int Treedb_Alloc(string szHost,string szFile,string szMainKey,string szMainKeyPwd="",int nOption=-1,int nPort=0);
	int Treedb_AllocEx(string szHost,string szFile,string szMainKey,string szSubKeyPath="",string szMainKeyPwd="",int nOption=-1,int nPort=0,
		bool bCreateMainKeyIfNotExist=false,bool bCreateFileIfNotExist=false,bool bCreateSubkeyIfNotExist=false,int nSubKeyFlag=BS_TRDB_FLAG_INDEXNODE);
	//���д��Ӽ��Ĳ��������øú���
	int Treedb_ReopenSubKey(string szSubKeyPath,int nOFlag=-1,string szMainKey="",string szMaiKeyPwd="",string szFile="");
	//�ڲ�����Treedb_ReopenSubKey���Ӽ�������򿪵��Ӽ���·������Щ�������ڣ���ᴴ�����Ӽ���·����������Ӽ���
	int Treedb_ReopenSubKeyEx(string szSubKeyPath,int nOFlag=-1,int nSubKeyFlag=BS_TRDB_FLAG_INDEXNODE,bool bCreateSubkeyIfNotExist=true,string szMainKey="",string szMainKeyPwd="",string szFile="");
	int Treedb_BeginTransaction();
	int Treedb_RollbackTransaction();
	int Treedb_CommitTransaction();
	static int Treedb_CreateFile(string szHost,string szNewFile,int nPort=0);
	static int Treedb_CreateMainKey(string szHost,string szFile,string szMainKey,string szMainKeyPwd="",int nPort=0);
	static int Treedb_RenameMainKey(string szHost,string szFile,string szOldMainKey,string szNewMainKey,string szMainKeyPwd="",int nPort=0);
	static int Treedb_DeleteMainKey(string szHost,string szFile,string szMainKey,string szMainKeyPwd="",int nPort=0);
	//��ȡ��������������Ҫhandle�����øú���֮ǰ����ҪAlloc
	static int Treedb_GetAllMainKeyNames(list<string>& lstMainKeyNames,string szHost,string szFile,int nPort=0);
	//int Treedb_OpenSubKey(string szSubKeyPath,int nFlag=TRDB_OPKF_OPENEXIST);
	//ֱ���ڵ�ǰhandleָ��ļ���������Ӽ�,��szSubKeyNameΪ�գ������ݿ������Ӽ���ͨ��szSubKeyName���أ������ø������Ӽ��������Ӽ�
	int Treedb_InsertSubKey(string& szSubKeyName,int nFlag=BS_TRDB_FLAG_INDEXNODE);
	//�ȴ򿪸������ٲ����Ӽ�,��szNewSubKeyΪ�գ������ݿ������Ӽ���ͨ��szNewSubKey���أ������ø������Ӽ��������Ӽ�
	int Treedb_AppendSubKey(string szParentKeyPath,string& szNewSubKey,int nFlag=BS_TRDB_FLAG_INDEXNODE);
	int Treedb_DeleteSubKey(string szSubKeyName);
	int Treedb_DeleteThisKey();
	int Treedb_QueryHandle(string& szMainKey,string& szSubKeyPath,string& szServer,
		unsigned int& uFlag,string& szDFBFile,unsigned int& uPort);
	int Treedb_RenameSubKey(string szOldSubKeyName,string szNewSubKeyName);
	//���ľ��ָ���������(ע�⣺���ܸ�������)
	int Treedb_RenameThisKey(string szNewKeyName);
	int Treedb_GetAllSubKeys(list<string>& lstSubKeys);
	int Treedb_InsertProperty(string szPropName,chen::VariableData& vData);
	int Treedb_GetProperty(string szPropName,chen::VariableData& vData);
	int Treedb_GetPropertyInt(string szPropName,int nDefault);//���ؽ��Ϊȡ�õ�int������
	string Treedb_GetPropertyString(string szPropName,string szDefault);
	__int64 Treedb_GetPropertyInt64(string szPropName,__int64 nDefault);
	float Treedb_GetPropertyFloat(string szPropName,float fDefault);
	double Treedb_GetPropertyDouble(string szPropName,double dDefault);
	int Treedb_EditProperty(string szPropName,chen::VariableData& vData);
	int Treedb_GetAllPropertyNames(list<string>& lstPropNames);
	int Treedb_DeleteProperty(string szPropName);
	int Treedb_DeleteAllProperty();
	int Treedb_RenameProperty(string szOldPropName,string szNewPropName);
	int Treedb_WriteKeyString(string szPropName,string szPropValue);
	int Treedb_WriteKeyInt(string szPropName,int nPropValue);
	int Treedb_WriteKeyInt64(string szPropName,__int64 n64PropValue);
	int Treedb_WriteKeyDouble(string szPropName,double nPropValue);
	int Treedb_WriteKeyVariable(string szPropName,const chen::VariableData& vData);
	int Treedb_SelectKeyPropertys(BSPROPERTYSET& lstProp);
    int Treedb_WriteMap(const map<string,chen::VariableData>& mapWrite);

	//Tabledb
	//�����ݿ���Ҫ��֤�û���������,Alloc�õ���handle��������Ч��
	int Tabledb_Alloc(string szHost,string szDbName,string szUser,string szPwd,int nPort=0,int nOpFlag=ONLY_OPEN_EXIST,unsigned int uDBFlag=0);
	//Alloc���ٴ�һ���µ�TableDb
	int Tabledb_ReopenDb(string szDbName,string szUser,string szPwd,int nOpFlag=ONLY_OPEN_EXIST,unsigned int uDBFlag=0);
	int Tabledb_BeginTransaction();
	int Tabledb_RollbackTransaction();
	int Tabledb_CommitTransaction();
	//��ȡ�������ݿ�������Ҫhandle�����øú���֮ǰ����ҪAlloc
	static int Tabledb_GetAllDBNames(list<string>& lstDBNames,string szHost,int nPort=0);
	//��ȡrid��һ����¼������vm��
	int Tabledb_GetOneRecord(string szTableName,i64 rid,FIELDVALUEMAP& vm,time_t& ctm);
	//��ȡ�����uLimitCount����¼����uLimitCountΪ0�����ȡ���м�¼������ȡ��¼��Ϊ0���򷵻�ֵ����BS_NOERROR
	int Tabledb_SelectRecords(BSHANDLE& hRS,string szTableName,ui32& uLimitCount);
	//��ȡszFieldֵΪvValue��uLimitCount����¼
	int Tabledb_SelectRecordsByField(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		string szField,chen::VariableData& vValue);
	//ȡ����ʱ����vFValue��vEndValue֮��ļ�¼
	int Tabledb_SelectRecordsByCTime(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		time_t tBegin,time_t tEnd,unsigned char ucCondition=0);//ucConditionĬ��Ϊ0�������½���
	//ȡĳ���ֶ���vFValue��vEndValue֮��ļ�¼
	int Tabledb_SelectRecordsByField(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		string szField,chen::VariableData& vFValue,chen::VariableData& vEndValue,
		unsigned char uCondition=0);//uConditionĬ��Ϊ0�������½���
	//ȡ����ʱ����ڻ�С��vValue�ļ�¼
	int Tabledb_SelectRecordsByCTime(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		time_t tTime,unsigned char ucCondition);
	//ȡĳ���ֶδ��ڻ�С��vValue�ļ�¼
	int Tabledb_SelectRecordsByField(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		string szFiled,chen::VariableData& vValue,unsigned char uCondition);
	//ԭʼAPIֱ�ӷ�װ,�����ֶι���
	int Tabledb_SelectRecordsByField(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		string szField,chen::VariableData& vFValue,chen::VariableData* pEndValue,
		unsigned char ucCondition);
	//ԭʼAPIֱ�ӷ�װ�����ݴ���ʱ�����
	int Tabledb_SelectRecordsByCTime(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		time_t tBegin,time_t* ptEnd,unsigned char ucCondition);
	int Tabledb_GetRecordsNumByField(int& nNum,string szTableName,string szField,chen::VariableData& vValue);
	//����һ����Ĭ�ϸ���һ���Զ�������ID���в���������Ϊ���������м�Ϊrid
	int Tabledb_CreateTable(string szTableName,BSFIELDINFOLIST& lFieldList,ui32 uFlag=BS_TDTF_FLAG_AUTOID|BS_TDTF_FLAG_INDEXID|BS_TDTF_FLAG_PMKI_DESCORDER);
	//Ĭ�Ͽ���ɾ��һ����
	int Tabledb_DeleteTable(string szTableName,unsigned short uType=TBDB_DT_QUICK);
	//��������,û��������������ݿ�API
	//int Tabledb_RenameTable(string szOldTableName,string szNewTableName);
	int Tabledb_GetAllTableNames(list<string>& lstTableNames);
	//�رռ�¼�����hRecordSet���α���hCursor
	int Tabledb_CloseHandle(BSHANDLE hOtherHd);
	int Tabledb_GetCursor(BSHANDLE& hRS,BSHANDLE& hCR);
	int Tabledb_CursorNext(BSHANDLE& hCR,FIELDVALUEMAP& vm,time_t& tCTime);
	//����һ��������¼
	int Tabledb_InsertRecord(string szTableName,BSFIELDVALUELIST& lRecordValueList);
	//����һ�������ļ�¼���������ݿ�Ϊ��¼�����rid������û��_FIELD_AUTOID_�ֶΣ�������BS_FAILED��
	int Tabledb_InsertRecord(string szTableName,BSFIELDVALUELIST& lRecordValueList,i64& rid);
	//���´���ʱ����vFValue��vEndValue֮������м�¼
	int Tabledb_UpdateRecordsByCTime(string szTableName,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList,
		time_t tBegin,time_t tEnd,ui64& uUpdateCount,
		unsigned char ucCondition=0);//uConditionĬ��Ϊ0���·�Χ�������½���
	//����ĳ���ֶ�ֵ��vFValue��vEndValue֮������м�¼
	int Tabledb_UpdateRecordsByField(string szTableName,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList,
		string szField,chen::VariableData& vFValue,chen::VariableData& vEndValue,ui64& uUpdateCount,
		unsigned char ucCondition=0);//uConditionĬ��Ϊ0���·�Χ�������½���
	//���´���ʱ����ڻ�С��vValue�����м�¼
	int Tabledb_UpdateRecordsByCTime(string szTableName,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList,
		time_t tTime,ui64& uUpdateCount,unsigned char ucCondition);
	//����ĳ���ֶ�ֵ���ڻ�С��vValue�����м�¼
	int Tabledb_UpdateRecordsByField(string szTableName,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList,
		string szField,chen::VariableData& vValue,ui64& uUpdateCount,unsigned char ucCondition);
	//����_FIELD_AUTOID_�ֶ�ֵΪrid��һ����¼,��������rid�ļ�¼����������BS_TBDB_NOFINDRECORD
	int Tabledb_UpdateOneRecord(string szTableName,i64 rid,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList);
	//ɾ��һ������������м�¼
	int Tabledb_DeleteAllRecord(string szTableName,ui64& uDeleteCount);
	//ɾ��һ��������_FIELD_AUTOID_�ֶ�ֵΪrid��һ����¼����������rid�ļ�¼����������BS_TBDB_NOFINDRECORD
	int Tabledb_DeleteOneRecord(string szTableName,i64 rid);
	//ɾ��ĳ���ֶ���vFValue��vEndValue֮������м�¼
	int Tabledb_DeleteRecordsByField(string szTableName,string szField,
		chen::VariableData& vFValue,chen::VariableData& vEndValue,ui64& uDeleteCount,
		unsigned char ucCondition=0);//uConditionĬ��Ϊ0ɾ����Χ�������½���
	//ɾ������ʱ����vFValue��vEndValue֮������м�¼
	int Tabledb_DeleteRecordsByCTime(string szTableName,
		time_t tBegin,time_t tEnd,ui64& uDeleteCount,
		unsigned char ucCondition=0);//uConditionĬ��Ϊ0ɾ����Χ�������½���
	//ֱ�Ӷ�ԭʼAPI�ķ�װ
	int Tabledb_DeleteRecordsByField(string szTableName,string szField,
		chen::VariableData& vFValue,chen::VariableData* pEndValue,
		ui64& uDeleteCount,unsigned char ucCondition);
	int Tabledb_DeleteRecordsByCTime(string szTableName,time_t tBegin,
		time_t* ptEnd,ui64& uDeleteCount,unsigned char ucCondition);
	////����һ�����ݿ�
	//int Tabledb_CreateDb(unsigned int uiDbFlag=0);
	//ɾ����ǰ���ݿ�,��ǰ������ɾ�����ݿ�
	//int Tabledb_DeleteDb();
	int Tabledb_GetFields(string szTableName,BSFIELDINFOLIST& lstFieldsInfo);
	static int Tabledb_GetDYNValue(string szHost,string szCCUFile,string szMonitorId,td_dyn& dyn,FIELDVALUEMAP& vmap,int nPort=0);
	static int Tabledb_GetSimpleDYN(string szHost,string szCCUFile,string szMonitorId,td_dyn& dyn,int nPort=0);
	int Tabledb_GetLastRDs(list<string>& lstTableNames,stdext::hash_map<string,bs_fieldmap>& mapRds);


	//MQdb
//	int MQdb_Alloc(string szHost,int nPort=0);
	//Alloc�õ���handle��������Ч��
	int MQdb_Alloc(string szHost,std::string sName,std::string sPWD,
		unsigned int uOpType=CMxVar::EXISTQ, 
		unsigned int uMQFlag=DefaultMQFlag,//��������˲���ָ��������mq�ķ��
		int nPort=0);
//	int MQdb_ReopenMQ(string szMQName,string szPwd,int nOFlag,int nMQFlag);
	//��ȡ����MQ������Ҫhandle�����øú���֮ǰ����ҪAlloc
	static int MQdb_GetAllMQNames(list<string>& lstMQNames,string szHost,int nPort=0);
	//ɾ����ǰhandleָ���mq
	int MQdb_DeleteMQ();
	int MQdb_ClearMQ();
	//�����Ϣ����Ϊ���ȶ�����nLevel����Ч����ʾ������Ϣ�����ȼ�
	int MQdb_PushMessage(const chen::VariableData& vData,string szLabel,ui64 &uID,
		unsigned char nLevel=BS_MQ_COMMONPRIORITY);
	//uIDĬ��Ϊ����ն��������һ����Ϣ����Ϊ0�����ָ��ID����Ϣ����ͨ���˱������ؽ��յ���Ϣ��ʵ��ID
	//����Ϣ����Ϊ��ʱ��nWaitTimeΪ0�򲻵ȴ����Ϸ��أ�Ĭ��ΪBS_TIMER_INFINITE��ʾ���޵ȴ�
	//isPeekΪfalse��ѽ��յ�����Ϣ�Ӷ����������Ϊtrue��ֻȡ����Ϣ�Ŀ�������Ϣ���ڶ�����
	int MQdb_PopMessage(chen::VariableData& vData,string& szLabel,time_t& ctm,
		ui64& uID,ui32  nWaitTime=BS_TIMER_INFINITE,bool bIsPeek=false);
	//int MQdb_PopMessage(chen::MemCache& bufData,string& szLabel,time_t& ctm,
	//	ui64& uID,ui32 nWaitTime=BS_TIMER_INFINITE,bool bIsPeek=false);
	int MQdb_PopMessage(chen::MemCache& bufData,ui32& nDataLen,string& szLabel,time_t& ctm, ui64& uID,ui32 nWaitTime=BS_TIMER_INFINITE,bool bIsPeek=false);
	int MQdb_GetLength(ui64& uCount);


	//��handleʧЧʱ��handle��Ϊ��Ч���������ݿⲻ����handleʧЧ�����Ըú�������Ҫ��
	int ReplaceHandle(); 
	//����handle
	bool AllocHandleFromPool(CDBServer& dbserver,int nType);
	//��handle�����Ż�handle��,free�Ժ�ö�����Ч����Ҫ����Allocһ�²�����
	void FreeHandle();
	int RollbackTransaction();
	
	std::string CCUFile();
	int  OpFlag();
//public:
//	//A=B,��B��������в�������A����,ʹA��������滻B����ʹ�ã�����B�������
//	CMxHandleLoc& operator=(CMxHandleLoc& hdloc);
//
//	void GetContent(string& szMainKey,string& szMainKeyPwd,
//		CConfHandle* & phd,CHandlePool* & pHandlePool,
//		CServerHandlePool* & pServerPool,
//		int& nHandleType,bool& bHaveTrans);

private:
	string m_szFile;
	string m_szMainKey;//���ֶ���ȫ��Ϊ�˺��ϰ�handle�ؼ����趨
	string m_szMainKeyPwd;

	CConfHandle* m_phd;//���
	CHandlePool* m_pHandlePool;//����handle��
	CServerHandlePool* m_pServerPool;//������server��
	int m_nHandleType;

	bool m_bHaveTransaction;//�Ƿ�������
};

extern CGrossHanlePool g_GrossHandlePool;

#endif
