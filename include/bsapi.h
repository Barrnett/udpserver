#ifndef _BSAPI_BSAPI_H_
#define _BSAPI_BSAPI_H_
#include "VariableData.h"
#include "MemCache.h"
#include <iostream>
#include <string>
#include <list>
#include <map>
#include "tabledb_def.h"
#include "BSMQDefine.h"
#include <hash_map>

#ifdef WIN32
	#ifdef BSAPI_EXPORTS
		#define BSAPI_API __declspec(dllexport)
	#else
		#define BSAPI_API __declspec(dllimport)
	#endif
#else
	#define BSAPI_API __attribute__((visibility("default"))) 
#endif

typedef void* BSHANDLE;
#define BS_INVALID_HANDLE	(BSHANDLE)~0

////////////////////////////////////////////////////////////
typedef std::map<std::string ,chen::VariableData > BSPROPERTYSET;

/*
* 关闭BSHANDLE句柄
* 所有打开成功的BSHANDLE用完后必须调用此函数关闭
*/
BSAPI_API
bool bs_close_handle(BSHANDLE &hHandle);

//设置数据库的默认端口
BSAPI_API
void bs_set_default_port(unsigned short uPort);

/*
* 设置默认超时间
* -1为永不超时
*/
BSAPI_API
void bs_set_default_timeout(int uTimeOut);

#define BSAPI_NETIO_TYPE_NAMEPIPE	0x01
#define BSAPI_NETIO_TYPE_SOCKET		0x02
/*
* 设置网络通讯方式
* 可以通过命名管道和socket方式通讯
* nType：可以指定BSAPI_NETIO_TYPE_NAMEPIPE或BSAPI_NETIO_TYPE_SOCKET
*/
BSAPI_API
void bs_set_netio_type(int nType);


////////////////////////////////////////////////////////////

/*
* 保持句柄有效
* hHandle：已经打开过的句柄，必须是通过..open打开的句柄
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_handle_keep_live(BSHANDLE &hHandle);


///////////////////////////////////////////////////////////////

/*
* 获取数据库中所有MQ的名称
* nameList：返回MQ名称列表
* sHostName：MQ服务器地址
* uPort：如果用Socket方式通讯用于指定要连接的数据库服务器所监听TCP端口，
		 为0则使用默认端口
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_mq_query_all_names(std::list<std::string >&nameList,const char *sHostName,unsigned short uPort=0);

/*
* 打开或创建一个MQ并返回其句柄
* hHandle：如果成功传出句柄，其初始值应该为BS_INVALID_HANDLE，此句柄用完后须通过bs_close_handle函数关闭
* sName：MQ名称
* sPWD：打开此MQ所需密码
* uOpenFlag：打开MQ的操作方式
* uMQFlag：如果是创建，此参数指定所创建MQ的风格
* sHostName：MQ服务器地址
* uPort：如果用Socket方式通讯用于指定要连接的数据库服务器所监听TCP端口，
		 为0则使用默认端口
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_mq_open(BSHANDLE &hHandle,const char *sName,const char *sPWD,unsigned int uOpenFlag,
			   unsigned int uMQFlag,const char *sHostName,unsigned short uPort=0);

/* 
* 利用同一个句柄打开一个新的MQ
* 其它参数参考bs_mq_open
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_mq_reopen(BSHANDLE &hHandle,const char *sName,const char *sPWD,unsigned int uOpenFlag,
			   unsigned int uMQFlag);

/* 
* 向一个打开的MQ中发送一条包含二进制数据的消息
* hHandle：已经打开的MQ句柄，此句柄必须通过bs_mq_open打开
* pData：指向存放要发送消息的缓冲区
* uDLen：消自息内容的长度
* sLable：要发送消息的Label字符串以'\0'结尾
* uID：返回此消在MQ队列中的ID
* nLevel：此变量用来指定发送消息到队列的级别，仅对优先队列有效
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_mq_push(BSHANDLE &hHandle,const char *pData,unsigned int uDLen,const char *sLabel,
			   unsigned __int64 &uID,unsigned char nLevel=BS_MQ_COMMONPRIORITY);

#define BS_TIMER_INFINITE	0xFFFFFFFF

/*
* 从一个打开的MQ中接收一条包含二进制数据的消息
* hHandle：已经打开的MQ句柄，此句柄必须通过bs_mq_open打开
* pBuf：返回接收到消息的缓冲区
* uDataLen：返回缓冲区的大小
* sLabel：返回接收到消息的Label
* cTime：返回此条MQ的创建时间
* uID：此值为零则接收队列中最后一条消息，否则接收指定ID的消息，并通过此变量返回接收到消息的ID
* nTimer：如果所接受的队列为空，此值表明要等待的时间，为零马上返回，为BS_TIMER_INFINITE则无限等待
*		  直到有消息返回或出错
* isPeek：如果此变量不为真则把接收到的消息从队列中清除，否则只取回消息的拷贝
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/

BSAPI_API
int bs_mq_pop(BSHANDLE &hHandle,chen::MemCache &pBuf,unsigned int &uDataLen,std::string &sLabel,time_t &cTime,
			  unsigned __int64 &uID,unsigned int nTimer=0,bool isPeek=false);


/* 
* 向一个打开的MQ中发送一条chen::VariableData类型的消息
* hHandle：已经打开的MQ句柄，此句柄必须通过bs_mq_open打开
* vData：存放要发送消息的变量
* sLable：要发送消息的Label字符串以'\0'结尾
* uID：返回此消在MQ队列中的ID
* nLevel：此变量用来指定发送消息到队列的级别，仅对优先队列有效
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_mq_push(BSHANDLE &hHandle,const chen::VariableData &vData,const char *sLabel,unsigned __int64 &uID,
			   unsigned char nLevel=BS_MQ_COMMONPRIORITY);


/*
* 从一个打开的MQ中接收一条chen::VariableData类型的消息，此函数只能接收发送的消息体为chen::VariableData类型的消息
* hHandle：已经打开的MQ句柄，此句柄必须通过bs_mq_open打开
* vData：返回接收到的消息
* sLabel：返回接收到消息的Label
* cTime：返回此条MQ的创建时间
* uID：此值为零则接收队列中最后一条消息，否则接收指定ID的消息，并通过此变量返回接收到消息的ID
* nTimer：如果所接受的队列为空，此值表明要等待的时间，为零马上返回，为BS_TIMER_INFINITE则无限等待
*		  直到有消息返回或出错
* isPeek：如果此函数不为真则把接收到的消息从队列中清除，否则只取回消息的拷贝
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_mq_pop(BSHANDLE &hHandle,chen::VariableData &vData,std::string &sLabel,time_t &cTime,
			  unsigned __int64 &uID,unsigned int nTimer=0,bool isPeek=false);

/*
* 向一个打开的MQ中发送一条字符类型的消息
* sData：要发送的字符串消息
* 其它参数参考上面同类函数
*/
BSAPI_API
int bs_mq_push(BSHANDLE &hHandle,const std::string &sData,const char *sLabel,unsigned __int64 &uID,unsigned char nLevel=BS_MQ_COMMONPRIORITY);

/*
* 从一个打开的MQ中接收一条字符串类型的消息，此函数只能接收发送的消息体为字符类型的消息
* sData：返回接收到的字符串消息
* 其它参数参考上面同类函数
*/
BSAPI_API
int bs_mq_pop(BSHANDLE &hHandle,std::string &sData,std::string &sLabel,time_t &cTime,
			  unsigned __int64 &uID,unsigned int nTimer=0,bool isPeek=false);

/*
* 获取一个打开MQ中的消息个数
* hHandle：已经打开的MQ句柄，此句柄必须通过bs_mq_open打开
* uCount：返回MQ的消息个数
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_mq_length(BSHANDLE &hHandle,unsigned __int64 &uCount);

/*
* 清空一个打开的MQ
* hHandle：已经打开的MQ句柄，此句柄必须通过bs_mq_open打开
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_mq_clear(BSHANDLE &hHandle);

/*
* 删除一个打开的MQ
* hHandle：已经打开的MQ句柄，此句柄必须通过bs_mq_open打开
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_mq_delete(BSHANDLE &hHandle);

/////////////////////////////////////////////////////////////////

/*
* 创建一个新的treeDB文件
* sDBFile：数据库文件名，不能为空
* sHostName：数据库服务器地址
* uPort：如果用Socket方式通讯用于指定要连接的数据库服务器所监听TCP端口，
		 为0则使用默认端口.
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_create_newfile(const char *sDBFile,const char *sHostName,unsigned short uPort);


/*
* 查询所有主键名
* mainKeyNameList：返回指定数据库中所有主键名称列表
* sDBFile：指定查询的数据库文件名，如果此值为NULL则使用默认数据库文件
* sHostName：数据库服务器地址
* uPort：如果用Socket方式通讯用于指定要连接的数据库服务器所监听TCP端口，
		 为0则使用默认端口.
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_query_mainkeys(std::list< std::string > &mainKeyNameList,const char *sDBFile,const char *sHostName,
							 unsigned short uPort=0);

/*
* 打开或创建一个键并返回其句柄
* hHandle：如果成功传出句柄，其初始值应该为BS_INVALID_HANDLE，此句柄用完后须通过bs_close_handle函数关闭
* sMainKey：主键名称
* sSubKeyPath：子键路径
* sPWD：主键的密码
* uFlag：打开键的方式
* sDBFile：指定数据库文件，如果此参为NULL则打开默认文件
* sHostName：数据库服务器地址
* uPort：如果用Socket方式通讯用于指定要连接的数据库服务器所监听TCP端口，
		 为0则使用默认端口.
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_open(BSHANDLE &hHandle,const char *sMainKey,const char *sSubKeyPath,const char *sPWD,unsigned int uFlag,
				const char *sDBFile,const char *sHostName,unsigned short uPort=0);

/*
* 利用现有句柄打开一个新键
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sMainKey：主键名称
* sSubKeyPath：子键路径
* sPWD：主键的密码
* uFlag：打开键的方式
* sDBFile：指定数据库文件，如果此参为NULL则打开默认文件
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_reopen(BSHANDLE &hHandle,const char *sMainKey,const char *sSubKeyPath,const char *sPWD,unsigned int uFlag,
					 const char *sDBFile);
/*
* 更改一个已打开主键的子键路径
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sSubKeyPath：新的子键路径，如果此值为NULL则打开主键根目录
* uFlag：打开键的方式
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_open_subkey(BSHANDLE &hHandle,const char *sSubKeyPath,unsigned int uFlag);

/*
* 开始一个跟此句柄关联的事务
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* 函数一旦调用成功，通过此句柄的所有操作将被看成一次事务，必须通调用bs_treedb_commit_transaction才能
* 使操作生效，或调用bs_treedb_rollback_transaction回滚所做的操作，如果没有调用bs_treedb_commit_transaction
* 句柄超时或关闭时将会自动调用回滚撤消所有操作。如果此句柄调用bs_treedb_reopen时改变了数据
* 库文件，则事务会被回滚，并且句柄处于没有事务的状态。如果该句柄已经成功调用过此函数，在没有调用
* bs_treedb_commit_transaction完成提交的情况下再次调用这个函数，将重新开始一个新
* 事务并引起老事务回滚。
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_begin_transaction(BSHANDLE &hHandle);

/*
* 提交跟此句柄关联的事务
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* 成功调用此函数将提交用hHandle句柄成功调bs_treedb_begin_transaction函数之后的所有操作
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_commit_transaction(BSHANDLE &hHandle);

/*
* 回滚跟此句柄关联的事务
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* 成功调用此函数将回滚掉用hHandle句柄成功调bs_treedb_begin_transaction函数之后的所有操作
* 事务回滚后有可能会引起句柄指向处于不正确的状态，建议调用bs_treedb_reopen重新打开一个键，或关闭句柄重新打开。
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_rollback_transaction(BSHANDLE &hHandle);



/*
* 查询一个treedb句柄的信息
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sMainKey：返回主键信息
* sSubKeyPath：返回子键路径信息
* sHostName：返回服务器地址信息
* sDFBFile：返回treedb数据库文件信息
* uFlag：返回键的打开方式信息
* uPort：返回服务器端口信息
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_query_handle(BSHANDLE &hHandle,std::string &sMainKey,std::string &sSubKeyPath,std::string &sHostName,
						   unsigned int &uFlag,std::string &sDFBFile,unsigned int &uPort);

/*
* 通过句柄向一个已打开的键中插入一个子键
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sSubKey：新子键
* uFlag：打开键的方式
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_insert_subkey(BSHANDLE &hHandle,const char *sSubKey,unsigned int uFlag);

/*
* 通过句柄向一个已打开的键中插入一个子键
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sSubKey：新子键，如为空或为NULL则自动按顺序生成新键名
* sKeyName：返回新增加子键的名称
* uFlag：打开键的方式
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_insert_subkey(BSHANDLE &hHandle,const char *sSubKey,std::string &sKeyName,unsigned int uFlag);

/*
* 通过句柄删除指定键的子键
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sSubKey：要删除的子键
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_delete_subkey(BSHANDLE &hHandle,const char *sSubKey);

/*
* 通过句柄删除指定的键(注意：此函数不能删除主键，如要删除可以调用OpenKey函数)
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* 返回值：如果成功返回BS_NOERROR否则返回错误码
* 注意：删除成功后，句柄将指向主键位置
*/
BSAPI_API
int bs_treedb_delete_key(BSHANDLE &hHandle);


/*
* 更改句柄指向键的某个子键的名称
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sSubKey：要更改的子键名称
* sNewKeyName：更改后的新名称
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_rename_subkey(BSHANDLE &hHandle,const char *sOldKeyName,const char *sNewKeyName);

/*
* 更改句柄指向键的名称(注意：不能更改主键，主键改名需要用OpenKey)
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sNewKeyName：更改后的新名称
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_rename_key(BSHANDLE &hHandle,const char *sNewKeyName);



/*
* 获取句柄所指向子键的所有子键名列表
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* subKeys：返回子键列表
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_get_subkeys(BSHANDLE &hHandle,std::list< std::string > &subKeys);

/*
* 向句柄指向的子键下插入一个属性
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sPropertyName：属性名称
* pBuf：二进制属性值缓冲区
* uBufLen：属性值缓冲区大小
* bOverWrite：如果属性值已经存在，此参数为真则覆盖原属性值，否则不覆盖返回错误码
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_insert_property(BSHANDLE &hHandle,const char *sPropertyName,const char *pBuf,unsigned int uBufLen,bool bOverWrite=false);

/*
* 获取句柄指向的子键下某个属性值
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sPropertyName：属性名称
* pBuf：存放返回的二进制属性值的缓冲区
* uBufLen：二进制属性值的大小
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_get_property(BSHANDLE &hHandle,const char *sPropertyName,chen::MemCache &pBuf,unsigned int &uBufLen);

/*
* 向句柄指向的子键下插入一个VariableData类型的属性
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sPropertyName：属性名称
* vData：属性值数据
* bOverWrite：如果属性值已经存在，此参数为真则覆盖原属性值，否则不覆盖返回错误码
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_insert_property(BSHANDLE &hHandle,const char *sPropertyName,const chen::VariableData &vData,bool bOverWrite=false);

/*
* 获取句柄指向的子键下某个VariableData类型的属性值
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sPropertyName：属性名称
* vData：存储返回VariableData类型属性值数据的变量
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_get_property(BSHANDLE &hHandle,const char *sPropertyName,chen::VariableData &vData);

/*
* 编辑句柄指向的子键下的某个属性值
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sPropertyName：属性名称
* pBuf：二进制属性值缓冲区
* uBufLen：属性值缓冲区大小
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_edit_property(BSHANDLE &hHandle,const char *sPropertyName,const char *pBuf,unsigned int uBufLen);

/*
* 编辑句柄指向的子键下的某个属性值
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* vData：属性值数据
* uBufLen：属性值缓冲区大小
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_edit_property(BSHANDLE &hHandle,const char *sPropertyName,const chen::VariableData &vData);


/*
* 获取句柄指向键的所有属性的名称列表
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* subKeys：存放返回属性名称列表的变量
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_get_all_property_names(BSHANDLE &hHandle,std::list< std::string > &propertyNames);

/*
* 根据名称删除句柄指向键的某个属性
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sPropertyName：属性名称
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_delete_property(BSHANDLE &hHandle,const char *sPropertyName);

/*
* 根据名称删除句柄指向键的所有属性
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_delete_all_property(BSHANDLE &hHandle);


/*
* 更改句柄指向键某个属性的名称
* hHandle：已经打开的键句柄，此句柄必须通过bs_treedb_open打开
* sOldPropertyName：旧属性名称
* sNewPropertyName：新属性名称
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_treedb_rename_property(BSHANDLE &hHandle,const char *sOldPropertyName,const char *sNewPropertyName);

//////////////////////////////////////////////////////////////////////

/*
* 获取所有数据库名称列表
* DBNameList：返回所有数据库名称列表
* sHostName：数据库所在的服务器地址
* uPort：如果用Socket方式通讯用于指定要连接的数据库服务器所监听TCP端口，
		 为0则使用默认端口.
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_get_all_dbnames(std::list< std::string > &DBNameList,const char *sHostName,unsigned short uPort=0);

/*
* 打开、创建或删除一个数据库并返回其句柄(删除除外)
* hHandle：如果成功传出句柄，其初始值应该为BS_INVALID_HANDLE，此句柄用完后须通过bs_close_handle函数关闭
* sDBName：要打开或创建的数据库名称
* sUser：打开或创建数据库的用户名
* sPWD：用户密码
* uOFlag：打开数据库的方法参考tabledb_def.h中的定义
* uDBFlag：如果创建此值指定数据库的风格
* sHostName：数据库服务器地址
* uPort：如果用Socket方式通讯用于指定要连接的数据库服务器所监听TCP端口，
		 为0则使用默认端口.
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_open(BSHANDLE &hHandle,const char *sDBName,const char *sUser,const char *sPWD,unsigned int uOFlag,unsigned int uDBFlag,
					const char *sHostName,unsigned short uPort=0);

/*
* 重命名数据
* sDBName：要重命名的数据库名称
* sNewDBName：数据库的新名称
* sUser：打开或创建数据库的用户名
* sPWD：用户密码
* sHostName：数据库服务器地址
* uPort：如果用Socket方式通讯用于指定要连接的数据库服务器所监听TCP端口，
		 为0则使用默认端口.
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_rename(const char *sDBName,const char *sNewDBName,const char *sUser,const char *sPWD,
					const char *sHostName,unsigned short uPort=0);


/*
* 查询数据库句柄内容
* hHandle：数据库句柄，被bs_tabledb_open或bs_tabledb_repoen打开过的，否则返加错误
* sDBName：返回数据库名称
* sUser：返回用户名称
* uOFlag：返回打开数据库所用的方法
* uDBFlag：打开时如果是创建，则此参数返回创建数据库的风格
* sHostName：返回数据库服务器的地址
* uPort：如果用Socket方式通讯则返回要连接的数据库服务器所使用的端口
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_qurey_handle(BSHANDLE &hHandle,std::string &sDBName,std::string &sUser,std::string &sPWD,unsigned int &uOFlag,unsigned int &uDBFlag,
							std::string &sHostName,unsigned short &uPort);

/*
* 利用现有句柄打开一个新数据库
* hHandle：已经通过bs_tabledb_open打开过的句柄
* sDBName：要打开的新数据库名称
* sUser：要打开数据库的用户名
* sPWD：用户密码
* uOFlag：打开数据库的方法参考tabledb_def.h中的定义
* uDBFlag：如果创建此值指定数据库的风格
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_reopen(BSHANDLE &hHandle,const char *sDBName,const char *sUser,const char *sPWD,unsigned int uOFlag,unsigned int uDBFlag);

/*
* 开始一个跟此句柄关联的事务
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* 函数一旦调用成功，通过此句柄的所有操作将被看成一次事务，必须通调用bs_tabledb_commit_transaction才能
* 使操作生效，或调用bs_tabledb_rollback_transaction回滚所做的操作，如果没有调用bs_tabledb_commit_transaction
* 句柄超时或关闭时将会自动调用回滚撤消所有操作。如果此句柄调用bs_treedb_reopen时改变了数据
* 库文件，则事务会被回滚，并且句柄处于没有事务的状态。如果该句柄已经成功调用过此函数，在没有调用
* bs_tabledb_commit_transaction完成提交的情况下再次调用这个函数，将重新开始一个新
* 事务并引起老事务回滚。
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_begin_transaction(BSHANDLE &hHandle);

/*
* 提交跟此句柄关联的事务
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* 成功调用此函数将提交用hHandle句柄成功调bs_tabledb_begin_transaction函数之后的所有操作
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_commit_transaction(BSHANDLE &hHandle);

/*
* 回滚跟此句柄关联的事务
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* 成功调用此函数将回滚掉用hHandle句柄成功调bs_tabledb_begin_transaction函数之后的所有操作
* 事务回滚后有可能会引起句柄指向处于不正确的状态，建议调用bs_tabledb_reopen重新打开一个键，或关闭句柄重新打开。
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_rollback_transaction(BSHANDLE &hHandle);


typedef std::list< bs_tdfield * > BSFIELDINFOLIST;

/*
* 创建表
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* lFieldList：字段信息列表
* uFlag：表风格，参考tabledb_def.h
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_create_table(BSHANDLE &hHandle,const char *sTableName,BSFIELDINFOLIST &lFieldList,unsigned int uFlag);

/*
* 获取数据库中所有的表名
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* tableNameList：如果成功通过此列表返回所有表名称
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_get_all_tablenames(BSHANDLE &hHandle,std::list< std::string > &tableNameList);

/*
* 删除表
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：要删除的表名称
* uType：删除表的方法参见tabledb_def.h
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_delete_table(BSHANDLE &hHandle,const char *sTableName,unsigned short uType);

/*
* 重命名表
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：需要改名的表名称
* sNewName：新名称
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_rename_table(BSHANDLE &hHandle,const char *sTableName,const char *sNewName);

/*
* 获取表的字段信息
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：要删除的表名称
* lFieldList：返回的字段信息列表，用完后需要用户删除列表中的bs_tdfield对像指针
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_get_fields(BSHANDLE &hHandle,const char *sTableName,BSFIELDINFOLIST &lFieldList);

typedef std::list < chen::VariableData > BSFIELDVALUELIST;

/*
* 向表中插入数据
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：表名称
* lRecordValueList：要插入的字段数据列表
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_insert_record(BSHANDLE &hHandle,const char *sTableName,BSFIELDVALUELIST &lRecordValueList);

/*
* 根据单列条件更新记录
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：表名称
* lFieldList：要更新记录的字段列表，如果此列表为空则认为更新整条记录中所有字段的值
* lRecordValueList：要更新的字段数据列表，如lFieldList不为空，此列表要跟lFieldList的项相对应，否则此列表为要包含
					记录所有字段的值。
* sCondField：作为过滤条件的列名称
* vFValue：过滤条件列的对比值
* pEndValue：过滤条件列的对比值，如果此值不为空，则会把以vFValue为开始值，pEndValue为结束值作为过滤条件，满足条件的记录
			将被更新,如果此值为空，会以vFValue和uCondition作为过滤条件，如果此值为真,则uCondition标记位0x1为真则结果集不包括开始值(vFValue)，标记位0x2为真则
			结果集不包括结尾值(pEndValue)，如果这两个标记位全为真则开始值和结果值全忽略。
* uCondition：过滤条件参考tabledb_def.h中的condition
* uOpFlag：操作方法参考tabledb_def.h中的condition flag，如果此标志为BS_FIELD_COND_USE_CTIME，则按创建时间过滤忽略sCondField
* uUpdateCount：如果成功返回更新的记录条数
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_update_record_by_field(BSHANDLE &hHandle,const char *sTableName,std::list< std::string > &lFieldList,
									BSFIELDVALUELIST &lRecordValueList,const char *sCondField,chen::VariableData &vFValue,
									chen::VariableData *pEndValue,unsigned char uCondition,unsigned char uOpFlag,unsigned __int64 &uUpdateCount);

/*
* 根据单列条件删除记录
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：表名称
* sCondField：作为过滤条件的列名称
* vFValue：过滤条件列的对比值
* pEndValue：过滤条件列的对比值，如果此值不为空，则会把以vFValue为开始值，pEndValue为结束值作为过滤条件，满足条件的记录
			将被删除,如果此值为空，会以vFValue和uCondition作为过滤条件，如果此值为真,则uCondition标记位0x1为真则结果集不包括开始值(vFValue)，标记位0x2为真则
			结果集不包括结尾值(pEndValue)，如果这两个标记位全为真则开始值和结果值全忽略。
* uCondition：过滤条件参考tabledb_def.h中的condition
* uOpFlag：操作方法参考tabledb_def.h中的condition flag，如果此标志为BS_FIELD_COND_USE_CTIME，则按创建时间过滤忽略sCondField
* uDeleteCount：如果成功返回被删除的记录条数
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_delete_record_by_field(BSHANDLE &hHandle,const char *sTableName,const char *sCondField,chen::VariableData &vFValue,
									chen::VariableData *pEndValue,unsigned char uCondition,unsigned char uOpFlag,unsigned __int64 &uDeleteCount);
/*
* 根据创建时间删除记录
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：表名称
* sCondField：作为过滤条件的列名称
* tBeginTime：时间对比值
* pEndTime：时间的对比值，如果此值不为空，则会把以tBeginTime为开始值，pEndTime为结束值作为过滤条件，满足条件的记录
			将被删除,如果此值为空，会以tBeginTime和uCondition作为过滤条件
* uCondition：过滤条件参考tabledb_def.h中的condition
* uOpFlag：操作方法参考tabledb_def.h
* uDeleteCount：如果成功返回被删除的记录条数
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_delete_record_by_ctime(BSHANDLE &hHandle,const char *sTableName,time_t tBeginTime,time_t *pEndTime,
									  unsigned char uCondition,unsigned char uOpFlag,unsigned __int64 &uDeleteCount);

/*
* 按记录生成时间获取表中最近的记录
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：表名称
* uCount：限定要获取的记录条数，成功后传出实际获取的记录数
* hRecordSet：如果成功则返回RecordSet句柄，它指向所获取的记录集，此句柄用完后须通过bs_close_handle函数关闭
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_get_record(BSHANDLE &hHandle,const char *sTableName,ui32 &uCount,BSHANDLE &hRecordSet);

/*
* 根据单列条件获取记录
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：表名称
* sCondField：作为过滤条件的列名称
* vFValue：过滤条件列的对比值
* pEndValue：过滤条件列的对比值，如果此值不为空，则会把以vFValue为开始值，pEndValue为结束值作为过滤条件，满足条件的记录
			将被获取,如果此值为空，会以vFValue和uCondition作为过滤条件。如果此值为真,则uCondition标记位0x1为真则结果集不包括开始值(vFValue)，标记位0x2为真则
			结果集不包括结尾值(pEndValue)，如果这两个标记位全为真则开始值和结果值全忽略。
* uCondition：过滤条件参考tabledb_def.h中的condition
* uOpFlag：操作方法参考tabledb_def.h中的condition flag，如果此标志为BS_FIELD_COND_USE_CTIME，则按创建时间过滤忽略sCondField
* uLimitCount： 限定要获取的记录条数，如果此值为0则不限定，成功后此变量传出实际获取的记录数
* hRecordSet：如果成功则返回RecordSet句柄，它指向所获取的记录集，此句柄用完后须通过bs_close_handle函数关闭
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_get_record_by_field(BSHANDLE &hHandle,const char *sTableName,const char *sCondField,chen::VariableData &vFValue,
									chen::VariableData *pEndValue,unsigned char uCondition,unsigned char uOpFlag,unsigned int &uLimitCount,BSHANDLE &hRecordSet);

/*
* 检索记录集句柄里的信息
* hRecordSet：记录集句柄，此句柄必须通过bs_tabledb_get_record_by_field或bs_tabledb_get_record创建
* uRecordCount：返回记录集中的记录条数
* lFieldList：返回记录集的字段信息
* uTableFlag：返回表风格
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_recordset_info(BSHANDLE &hRecordSet,ui32 &uRecordCount,BSFIELDINFOLIST &lFieldList,ui32 &uTableFlag);

/*
* 获取记录集句柄的游标句柄
* hRecordSet：记录集句柄，此句柄必须通过bs_tabledb_get_record_by_field或bs_tabledb_get_record创建
* hCursor：如果成功，通过此变量返回游标句柄，此句柄用完后须通过bs_close_handle函数关闭
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_get_cursor(BSHANDLE &hRecordSet,BSHANDLE &hCursor);

typedef stdext::hash_map<std::string ,chen::VariableData> FIELDVALUEMAP;

/*
* 返回游标句柄中游标所指向的当前记录信息，并移动游标指向向一条记录
* hCursor：游标句柄，此句柄必须通过bs_tabledb_get_cursor创建
* fvMap：如果成功，此变量返回当前记录内容
* ctm：如果成功，此变量返回当前记录的创建时间
* 返回值：如果成功返回BS_NOERROR否则返回错误码,返回BSAPI_CURSOR_END代表已经到记录集结尾
*/
BSAPI_API
int bs_tabledb_cursor_next(BSHANDLE &hCursor,FIELDVALUEMAP &fvMap,time_t &ctm);

/*
* 移动游标句柄中的游标指向上一条记录，并返回移动后指向记录的信息
* hCursor：游标句柄，此句柄必须通过bs_tabledb_get_cursor创建
* fvMap：如果成功，此变量返回记录内容
* ctm：如果成功，此变量返回记录创建时间
* 返回值：如果成功返回BS_NOERROR否则返回错误码，返回BSAPI_CURSOR_BEGIN代表已经到记录集头位置
*/
BSAPI_API
int bs_tabledb_cursor_pre(BSHANDLE &hCursor,FIELDVALUEMAP &fvMap,time_t &ctm);

/*
* 重置游标句柄中的游标，使其指向记录集的开始位置
* hCursor：游标句柄，此句柄必须通过bs_tabledb_get_cursor创建
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_cursor_reset(BSHANDLE &hCursor);

///////////////////dyn/////////////////////

/*
* 获得dyn类型表的dyn信息
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：dyn表名称
* dyn：如果成功，此变量返回dyn信息
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_get_dyn(BSHANDLE &hHandle,const char *sTableName,td_dyn &dyn);

/*
* 获得dyn类型表的dyn信息和最后一条记录
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sTableName：dyn表名称
* dyn：如果成功，此变量返回dyn信息
* fvMap：最后一条记录信息
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_get_dyn_and_lastrd(BSHANDLE &hHandle,const char *sTableName,td_dyn &dyn,FIELDVALUEMAP &fvMap);

/*
* 通过二进制数据转化出dyn信息和一条记录
* pData：二进制缓冲区指针
* nDLen：缓冲区大小
* dyn：如果成功，此变量返回dyn信息
* fvMap：最后一条记录信息
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/

BSAPI_API
int bs_parse_alert_data(const char *pData ,int nDLen,td_dyn &dyn,FIELDVALUEMAP &fvMap);


/*
* 获取数据库中所有dyn表的状态信息
* hHandle：数据库句柄，此句柄必须通过bs_tabledb_open创建
* sGroupName：如果此变量的值不为空，则获取所有表名称前面部分与此字符串相同的表的状态信息，为空则获取反有表的状态信息
* nStateMap：如果isQuickMode变量为真，获取成功后此变量返回以unsigned int为key，以unsigned char(状态)为value的hashmap，此map的key是表名称通过特定算法
			 生成的unsigned int型数据，value为状态值。如果isQuickMode不为真则此变量不起作用。
* sStateMap：如果获取成功，此变量返回以表名为key以状态为value的hashmap，如果isQuickMode为真，则此变量只包含nStateMap变量中key值换算有重复的表的状态
			 信息(此变量只包含发现重复后的表状态信息，第一个生成相同unsigned int Key的表信息仍包含在nStateMap中)。如果isQuickMode不为真则此变量包含
			 全部dny表的完整名称和状态信息。
* isQuickMode：控制获取表状态信息的方式，参考上面两个参数
* 返回值：如果成功返回BS_NOERROR否则返回错误码
*/
BSAPI_API
int bs_tabledb_get_all_dyn_table_state(BSHANDLE &hHandle,const char *sGroupName,stdext::hash_map<unsigned int,unsigned char > &nStateMap,
									   stdext::hash_map<std::string,unsigned char > &sStateMap,bool isQuickMode=true);




BSAPI_API
int bs_tabledb_covert_recordvalue_to_buffer(BSFIELDVALUELIST &lRecordValueList,char *pBuf,unsigned int &nBufLen);

BSAPI_API
int bs_tabledb_insert_record_by_buffer(BSHANDLE &hHandle,const char *sTableName,char *pBuf,unsigned int nBufLen);

BSAPI_API
int bs_get_state_from_rdbuffer(const char *pBuf,unsigned int nBufLen,unsigned char &uState);

//struct st_ftlist
//{
//	const char *name;
//	unsigned char type;
//};

BSAPI_API
int bs_covert_rdbuffer_to_rdmap(const char *pBuf,unsigned int nBufLen,std::list<std::pair<std::string,unsigned char > > &ftlist,FIELDVALUEMAP &rdMap);



class bs_fieldmap
{
public:
	bs_fieldmap()
	{
		m_pFieldMap=NULL;
	}
	bs_fieldmap(FIELDVALUEMAP *pFieldMap)
	{
		m_pFieldMap=pFieldMap;

	}
	~bs_fieldmap()
	{
		if(m_pFieldMap)
			delete m_pFieldMap;
	}

	bs_fieldmap& operator=(FIELDVALUEMAP *pFieldMap)
	{
		this->m_pFieldMap=pFieldMap;

		return *this;
	}

	FIELDVALUEMAP *m_pFieldMap;
};

BSAPI_API
int bs_get_tables_last_rd(BSHANDLE &hHandle,const char *sTableNames,unsigned int uNameBufLen,stdext::hash_map<std::string,bs_fieldmap > &lastRDMaps);




////////////////////////////////////////////////////////////////////////////////////////////













#endif

