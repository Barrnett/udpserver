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
#define  MXINFODB           "mxinfo"    //licence所在的数据库

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

#define ONLY_OPEN_EXIST 1 //tabledb数据库或者treedb的文件只打开已存在的
#define CREATE_IF_NOT_EXIST 2 //先尝试打开tabledb数据库或者treedb的文件，如果不存在则创建

//Treedb定义OpenKey Flag只有两种，即点路径方式
//打开已经存在的键，或斜杠路径方式打开已存在的键
//Treedb handle池不允许创建和修改主键
#define ExistDotPath		(TRDB_OPKF_DOTPATH|TRDB_OPKF_OPENEXIST)
#define ExistNodePath		(TRDB_OPKF_OPENEXIST)

#define DefaultMQFlag (BSMQ_OT_COMMONMQ|BSMQ_OT_INDEXRECORD) //创建mq的默认风格,非优先级的索引队列
//
static const char mxRId[]								= "_FIELD_AUTOID_";//"mxrid",记录ID
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

	void AddReference();//指针指向一个对象后必须调用该函数后指针才可用
	static void DeleteHandleObj(CDealHandleObj* pObj);
public:
	chen::Mutex m_mutexRefNum;
	int m_nRefNum;//该对象的引用数
};
class CHandlePool: public CDealHandleObj
{
public:
	CHandlePool();
	virtual ~CHandlePool();

public:
	CConfHandle* Alloc(int nHdType,bool bFromFront);//默认从front取一个有效的handle
	void Free(CConfHandle* phd);
private:
	int m_nMaxSize;	//handle池的大小
	int m_nExistNum; //当前已经new出来的handle数，包括正在使用的和在m_lstFreeHanldes中保存的空闲的handle

	chen::Mutex m_mutex;
	list<CConfHandle*> m_lstFreeHanldes;
};

////treedb一个文件上的handle池
//class CTreedbFilePool: public CHandlePool
//{
//public:
//	CTreedbFilePool(string szFile);
//	virtual ~CTreedbFilePool();
//
//	CTreedbHd* Alloc(bool bFromFront=true);
//public:
//	string m_szTreedbFileName;//treedb文件名称
//};

//tabledb一个数据库中的handle池
//class CTabledbPool: public CHandlePool
//{
//public:
//	CTabledbPool(string szDbName,string szUser,string szPwd);
//	virtual ~CTabledbPool();
//
//	//打开handle的同时检查用户名和密码是否给对
//	int CheckAndOpenHandle(CTabledbHd* phd,string szHost,int nPort,
//		string szUser,string szPwd,int nOpFlag);
//	CTabledbHd* Alloc(bool bFromFront=true);
//public:
//	string m_szDbName;//treedb数据库名称
//	//保存数据库的正确的用户名和密码，用于验证给出的用户名密码
//	//目前认为一个数据库只有一个用户名和密码，只要同时符合这两样就可以打开数据库
//	string m_szUser;
//	string m_szPwd;
//	bool m_bIsValid;//保存的用户名和密码是否有效
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
	//treedb的handle池
	CHandlePool m_TreedbPool;//所有的treedb共享一个池

	//tabledb的handle池
	CHandlePool m_TabledbPool;//所有的tabledb共享一个池

	//MQ的handle池,CHandlePool中已经有锁
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
	map<CDBServer,CServerHandlePool*> m_mapGrossPool;//server -- server的handle池
};

class CMxHandleLoc
{
public:
	CMxHandleLoc();
	~CMxHandleLoc();

	CConfHandle* GetConfHandle();
	int GetHandleType();


	//定位到一个主键，再用Treedb_ReopenSubKey在该主键下移动到不同子键，对其他主键上子键的操作得先用该函数跳到其他主键
	//Alloc得到的一定是一个有效的handle
	int Treedb_Alloc(string szHost,string szFile,string szMainKey,string szMainKeyPwd="",int nOption=-1,int nPort=0);
	int Treedb_AllocEx(string szHost,string szFile,string szMainKey,string szSubKeyPath="",string szMainKeyPwd="",int nOption=-1,int nPort=0,
		bool bCreateMainKeyIfNotExist=false,bool bCreateFileIfNotExist=false,bool bCreateSubkeyIfNotExist=false,int nSubKeyFlag=BS_TRDB_FLAG_INDEXNODE);
	//所有打开子键的操作都是用该函数
	int Treedb_ReopenSubKey(string szSubKeyPath,int nOFlag=-1,string szMainKey="",string szMaiKeyPwd="",string szFile="");
	//内部调用Treedb_ReopenSubKey打开子键，如果打开到子键的路径上有些键不存在，则会创建到子键的路径，并最后将子键打开
	int Treedb_ReopenSubKeyEx(string szSubKeyPath,int nOFlag=-1,int nSubKeyFlag=BS_TRDB_FLAG_INDEXNODE,bool bCreateSubkeyIfNotExist=true,string szMainKey="",string szMainKeyPwd="",string szFile="");
	int Treedb_BeginTransaction();
	int Treedb_RollbackTransaction();
	int Treedb_CommitTransaction();
	static int Treedb_CreateFile(string szHost,string szNewFile,int nPort=0);
	static int Treedb_CreateMainKey(string szHost,string szFile,string szMainKey,string szMainKeyPwd="",int nPort=0);
	static int Treedb_RenameMainKey(string szHost,string szFile,string szOldMainKey,string szNewMainKey,string szMainKeyPwd="",int nPort=0);
	static int Treedb_DeleteMainKey(string szHost,string szFile,string szMainKey,string szMainKeyPwd="",int nPort=0);
	//获取所有主键名不需要handle，调用该函数之前不需要Alloc
	static int Treedb_GetAllMainKeyNames(list<string>& lstMainKeyNames,string szHost,string szFile,int nPort=0);
	//int Treedb_OpenSubKey(string szSubKeyPath,int nFlag=TRDB_OPKF_OPENEXIST);
	//直接在当前handle指向的键下面添加子键,若szSubKeyName为空，则数据库生成子键名通过szSubKeyName返回，否则用给出的子键名创建子键
	int Treedb_InsertSubKey(string& szSubKeyName,int nFlag=BS_TRDB_FLAG_INDEXNODE);
	//先打开父键，再插入子键,若szNewSubKey为空，则数据库生成子键名通过szNewSubKey返回，否则用给出的子键名创建子键
	int Treedb_AppendSubKey(string szParentKeyPath,string& szNewSubKey,int nFlag=BS_TRDB_FLAG_INDEXNODE);
	int Treedb_DeleteSubKey(string szSubKeyName);
	int Treedb_DeleteThisKey();
	int Treedb_QueryHandle(string& szMainKey,string& szSubKeyPath,string& szServer,
		unsigned int& uFlag,string& szDFBFile,unsigned int& uPort);
	int Treedb_RenameSubKey(string szOldSubKeyName,string szNewSubKeyName);
	//更改句柄指向键的名称(注意：不能更改主键)
	int Treedb_RenameThisKey(string szNewKeyName);
	int Treedb_GetAllSubKeys(list<string>& lstSubKeys);
	int Treedb_InsertProperty(string szPropName,chen::VariableData& vData);
	int Treedb_GetProperty(string szPropName,chen::VariableData& vData);
	int Treedb_GetPropertyInt(string szPropName,int nDefault);//返回结果为取得的int型属性
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
	//打开数据库需要验证用户名和密码,Alloc得到的handle必须是有效的
	int Tabledb_Alloc(string szHost,string szDbName,string szUser,string szPwd,int nPort=0,int nOpFlag=ONLY_OPEN_EXIST,unsigned int uDBFlag=0);
	//Alloc后再打开一个新的TableDb
	int Tabledb_ReopenDb(string szDbName,string szUser,string szPwd,int nOpFlag=ONLY_OPEN_EXIST,unsigned int uDBFlag=0);
	int Tabledb_BeginTransaction();
	int Tabledb_RollbackTransaction();
	int Tabledb_CommitTransaction();
	//获取所有数据库名不需要handle，调用该函数之前不需要Alloc
	static int Tabledb_GetAllDBNames(list<string>& lstDBNames,string szHost,int nPort=0);
	//获取rid的一条记录，放入vm中
	int Tabledb_GetOneRecord(string szTableName,i64 rid,FIELDVALUEMAP& vm,time_t& ctm);
	//获取最近的uLimitCount条记录，若uLimitCount为0，则获取所有记录，若获取记录数为0，则返回值还是BS_NOERROR
	int Tabledb_SelectRecords(BSHANDLE& hRS,string szTableName,ui32& uLimitCount);
	//获取szField值为vValue的uLimitCount条记录
	int Tabledb_SelectRecordsByField(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		string szField,chen::VariableData& vValue);
	//取创建时间在vFValue和vEndValue之间的记录
	int Tabledb_SelectRecordsByCTime(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		time_t tBegin,time_t tEnd,unsigned char ucCondition=0);//ucCondition默认为0包括上下界限
	//取某个字段在vFValue和vEndValue之间的记录
	int Tabledb_SelectRecordsByField(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		string szField,chen::VariableData& vFValue,chen::VariableData& vEndValue,
		unsigned char uCondition=0);//uCondition默认为0包括上下界限
	//取创建时间大于或小于vValue的记录
	int Tabledb_SelectRecordsByCTime(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		time_t tTime,unsigned char ucCondition);
	//取某个字段大于或小于vValue的记录
	int Tabledb_SelectRecordsByField(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		string szFiled,chen::VariableData& vValue,unsigned char uCondition);
	//原始API直接封装,根据字段过滤
	int Tabledb_SelectRecordsByField(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		string szField,chen::VariableData& vFValue,chen::VariableData* pEndValue,
		unsigned char ucCondition);
	//原始API直接封装，根据创建时间过滤
	int Tabledb_SelectRecordsByCTime(BSHANDLE& hRS,string szTableName,ui32& uLimitCount,
		time_t tBegin,time_t* ptEnd,unsigned char ucCondition);
	int Tabledb_GetRecordsNumByField(int& nNum,string szTableName,string szField,chen::VariableData& vValue);
	//创建一个表，默认附加一个自动增长的ID的列并将该列设为主键，该列即为rid
	int Tabledb_CreateTable(string szTableName,BSFIELDINFOLIST& lFieldList,ui32 uFlag=BS_TDTF_FLAG_AUTOID|BS_TDTF_FLAG_INDEXID|BS_TDTF_FLAG_PMKI_DESCORDER);
	//默认快速删除一个表
	int Tabledb_DeleteTable(string szTableName,unsigned short uType=TBDB_DT_QUICK);
	//重命名表,没有重命名表的数据库API
	//int Tabledb_RenameTable(string szOldTableName,string szNewTableName);
	int Tabledb_GetAllTableNames(list<string>& lstTableNames);
	//关闭记录集句柄hRecordSet或游标句柄hCursor
	int Tabledb_CloseHandle(BSHANDLE hOtherHd);
	int Tabledb_GetCursor(BSHANDLE& hRS,BSHANDLE& hCR);
	int Tabledb_CursorNext(BSHANDLE& hCR,FIELDVALUEMAP& vm,time_t& tCTime);
	//插入一条完整记录
	int Tabledb_InsertRecord(string szTableName,BSFIELDVALUELIST& lRecordValueList);
	//插入一条完整的记录，返回数据库为记录分配的rid（若表没有_FIELD_AUTOID_字段，将返回BS_FAILED）
	int Tabledb_InsertRecord(string szTableName,BSFIELDVALUELIST& lRecordValueList,i64& rid);
	//更新创建时间在vFValue和vEndValue之间的所有记录
	int Tabledb_UpdateRecordsByCTime(string szTableName,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList,
		time_t tBegin,time_t tEnd,ui64& uUpdateCount,
		unsigned char ucCondition=0);//uCondition默认为0更新范围包括上下界限
	//更新某个字段值在vFValue和vEndValue之间的所有记录
	int Tabledb_UpdateRecordsByField(string szTableName,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList,
		string szField,chen::VariableData& vFValue,chen::VariableData& vEndValue,ui64& uUpdateCount,
		unsigned char ucCondition=0);//uCondition默认为0更新范围包括上下界限
	//更新创建时间大于或小于vValue的所有记录
	int Tabledb_UpdateRecordsByCTime(string szTableName,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList,
		time_t tTime,ui64& uUpdateCount,unsigned char ucCondition);
	//更新某个字段值大于或小于vValue的所有记录
	int Tabledb_UpdateRecordsByField(string szTableName,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList,
		string szField,chen::VariableData& vValue,ui64& uUpdateCount,unsigned char ucCondition);
	//更新_FIELD_AUTOID_字段值为rid的一条记录,若不存在rid的记录，则函数返回BS_TBDB_NOFINDRECORD
	int Tabledb_UpdateOneRecord(string szTableName,i64 rid,list<string>& lFieldList,BSFIELDVALUELIST& lRecordValueList);
	//删除一个表里面的所有记录
	int Tabledb_DeleteAllRecord(string szTableName,ui64& uDeleteCount);
	//删除一个表里面_FIELD_AUTOID_字段值为rid的一条记录，若不存在rid的记录，则函数返回BS_TBDB_NOFINDRECORD
	int Tabledb_DeleteOneRecord(string szTableName,i64 rid);
	//删除某个字段在vFValue和vEndValue之间的所有记录
	int Tabledb_DeleteRecordsByField(string szTableName,string szField,
		chen::VariableData& vFValue,chen::VariableData& vEndValue,ui64& uDeleteCount,
		unsigned char ucCondition=0);//uCondition默认为0删除范围包括上下界限
	//删除创建时间在vFValue和vEndValue之间的所有记录
	int Tabledb_DeleteRecordsByCTime(string szTableName,
		time_t tBegin,time_t tEnd,ui64& uDeleteCount,
		unsigned char ucCondition=0);//uCondition默认为0删除范围包括上下界限
	//直接对原始API的封装
	int Tabledb_DeleteRecordsByField(string szTableName,string szField,
		chen::VariableData& vFValue,chen::VariableData* pEndValue,
		ui64& uDeleteCount,unsigned char ucCondition);
	int Tabledb_DeleteRecordsByCTime(string szTableName,time_t tBegin,
		time_t* ptEnd,ui64& uDeleteCount,unsigned char ucCondition);
	////创建一个数据库
	//int Tabledb_CreateDb(unsigned int uiDbFlag=0);
	//删除当前数据库,当前不允许删除数据库
	//int Tabledb_DeleteDb();
	int Tabledb_GetFields(string szTableName,BSFIELDINFOLIST& lstFieldsInfo);
	static int Tabledb_GetDYNValue(string szHost,string szCCUFile,string szMonitorId,td_dyn& dyn,FIELDVALUEMAP& vmap,int nPort=0);
	static int Tabledb_GetSimpleDYN(string szHost,string szCCUFile,string szMonitorId,td_dyn& dyn,int nPort=0);
	int Tabledb_GetLastRDs(list<string>& lstTableNames,stdext::hash_map<string,bs_fieldmap>& mapRds);


	//MQdb
//	int MQdb_Alloc(string szHost,int nPort=0);
	//Alloc得到的handle必须是有效的
	int MQdb_Alloc(string szHost,std::string sName,std::string sPWD,
		unsigned int uOpType=CMxVar::EXISTQ, 
		unsigned int uMQFlag=DefaultMQFlag,//如果创建此参数指明创建的mq的风格
		int nPort=0);
//	int MQdb_ReopenMQ(string szMQName,string szPwd,int nOFlag,int nMQFlag);
	//获取所有MQ名不需要handle，调用该函数之前不需要Alloc
	static int MQdb_GetAllMQNames(list<string>& lstMQNames,string szHost,int nPort=0);
	//删除当前handle指向的mq
	int MQdb_DeleteMQ();
	int MQdb_ClearMQ();
	//如果消息队列为优先队列则nLevel才有效，表示发送消息的优先级
	int MQdb_PushMessage(const chen::VariableData& vData,string szLabel,ui64 &uID,
		unsigned char nLevel=BS_MQ_COMMONPRIORITY);
	//uID默认为零接收队列中最后一条消息，不为0则接收指定ID的消息，并通过此变量返回接收到消息的实际ID
	//当消息队列为空时，nWaitTime为0则不等待马上返回，默认为BS_TIMER_INFINITE表示无限等待
	//isPeek为false则把接收到的消息从队列中清除，为true则只取回消息的拷贝，消息还在队列中
	int MQdb_PopMessage(chen::VariableData& vData,string& szLabel,time_t& ctm,
		ui64& uID,ui32  nWaitTime=BS_TIMER_INFINITE,bool bIsPeek=false);
	//int MQdb_PopMessage(chen::MemCache& bufData,string& szLabel,time_t& ctm,
	//	ui64& uID,ui32 nWaitTime=BS_TIMER_INFINITE,bool bIsPeek=false);
	int MQdb_PopMessage(chen::MemCache& bufData,ui32& nDataLen,string& szLabel,time_t& ctm, ui64& uID,ui32 nWaitTime=BS_TIMER_INFINITE,bool bIsPeek=false);
	int MQdb_GetLength(ui64& uCount);


	//当handle失效时将handle置为有效，但新数据库不允许handle失效，所以该函数不需要用
	int ReplaceHandle(); 
	//分配handle
	bool AllocHandleFromPool(CDBServer& dbserver,int nType);
	//将handle主动放回handle池,free以后该对象无效，需要重新Alloc一下才能用
	void FreeHandle();
	int RollbackTransaction();
	
	std::string CCUFile();
	int  OpFlag();
//public:
//	//A=B,将B对象的所有参数传给A对象,使A对象可以替换B对象使用，并将B对象清空
//	CMxHandleLoc& operator=(CMxHandleLoc& hdloc);
//
//	void GetContent(string& szMainKey,string& szMainKeyPwd,
//		CConfHandle* & phd,CHandlePool* & pHandlePool,
//		CServerHandlePool* & pServerPool,
//		int& nHandleType,bool& bHaveTrans);

private:
	string m_szFile;
	string m_szMainKey;//该字段完全是为了和老版handle池兼容设定
	string m_szMainKeyPwd;

	CConfHandle* m_phd;//句柄
	CHandlePool* m_pHandlePool;//所属handle池
	CServerHandlePool* m_pServerPool;//所属的server池
	int m_nHandleType;

	bool m_bHaveTransaction;//是否开启事务
};

extern CGrossHanlePool g_GrossHandlePool;

#endif
