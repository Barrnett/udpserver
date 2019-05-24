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
* �ر�BSHANDLE���
* ���д򿪳ɹ���BSHANDLE����������ô˺����ر�
*/
BSAPI_API
bool bs_close_handle(BSHANDLE &hHandle);

//�������ݿ��Ĭ�϶˿�
BSAPI_API
void bs_set_default_port(unsigned short uPort);

/*
* ����Ĭ�ϳ�ʱ��
* -1Ϊ������ʱ
*/
BSAPI_API
void bs_set_default_timeout(int uTimeOut);

#define BSAPI_NETIO_TYPE_NAMEPIPE	0x01
#define BSAPI_NETIO_TYPE_SOCKET		0x02
/*
* ��������ͨѶ��ʽ
* ����ͨ�������ܵ���socket��ʽͨѶ
* nType������ָ��BSAPI_NETIO_TYPE_NAMEPIPE��BSAPI_NETIO_TYPE_SOCKET
*/
BSAPI_API
void bs_set_netio_type(int nType);


////////////////////////////////////////////////////////////

/*
* ���־����Ч
* hHandle���Ѿ��򿪹��ľ����������ͨ��..open�򿪵ľ��
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_handle_keep_live(BSHANDLE &hHandle);


///////////////////////////////////////////////////////////////

/*
* ��ȡ���ݿ�������MQ������
* nameList������MQ�����б�
* sHostName��MQ��������ַ
* uPort�������Socket��ʽͨѶ����ָ��Ҫ���ӵ����ݿ������������TCP�˿ڣ�
		 Ϊ0��ʹ��Ĭ�϶˿�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_mq_query_all_names(std::list<std::string >&nameList,const char *sHostName,unsigned short uPort=0);

/*
* �򿪻򴴽�һ��MQ����������
* hHandle������ɹ�������������ʼֵӦ��ΪBS_INVALID_HANDLE���˾���������ͨ��bs_close_handle�����ر�
* sName��MQ����
* sPWD���򿪴�MQ��������
* uOpenFlag����MQ�Ĳ�����ʽ
* uMQFlag������Ǵ������˲���ָ��������MQ�ķ��
* sHostName��MQ��������ַ
* uPort�������Socket��ʽͨѶ����ָ��Ҫ���ӵ����ݿ������������TCP�˿ڣ�
		 Ϊ0��ʹ��Ĭ�϶˿�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_mq_open(BSHANDLE &hHandle,const char *sName,const char *sPWD,unsigned int uOpenFlag,
			   unsigned int uMQFlag,const char *sHostName,unsigned short uPort=0);

/* 
* ����ͬһ�������һ���µ�MQ
* ���������ο�bs_mq_open
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_mq_reopen(BSHANDLE &hHandle,const char *sName,const char *sPWD,unsigned int uOpenFlag,
			   unsigned int uMQFlag);

/* 
* ��һ���򿪵�MQ�з���һ���������������ݵ���Ϣ
* hHandle���Ѿ��򿪵�MQ������˾������ͨ��bs_mq_open��
* pData��ָ����Ҫ������Ϣ�Ļ�����
* uDLen������Ϣ���ݵĳ���
* sLable��Ҫ������Ϣ��Label�ַ�����'\0'��β
* uID�����ش�����MQ�����е�ID
* nLevel���˱�������ָ��������Ϣ�����еļ��𣬽������ȶ�����Ч
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_mq_push(BSHANDLE &hHandle,const char *pData,unsigned int uDLen,const char *sLabel,
			   unsigned __int64 &uID,unsigned char nLevel=BS_MQ_COMMONPRIORITY);

#define BS_TIMER_INFINITE	0xFFFFFFFF

/*
* ��һ���򿪵�MQ�н���һ���������������ݵ���Ϣ
* hHandle���Ѿ��򿪵�MQ������˾������ͨ��bs_mq_open��
* pBuf�����ؽ��յ���Ϣ�Ļ�����
* uDataLen�����ػ������Ĵ�С
* sLabel�����ؽ��յ���Ϣ��Label
* cTime�����ش���MQ�Ĵ���ʱ��
* uID����ֵΪ������ն��������һ����Ϣ���������ָ��ID����Ϣ����ͨ���˱������ؽ��յ���Ϣ��ID
* nTimer����������ܵĶ���Ϊ�գ���ֵ����Ҫ�ȴ���ʱ�䣬Ϊ�����Ϸ��أ�ΪBS_TIMER_INFINITE�����޵ȴ�
*		  ֱ������Ϣ���ػ����
* isPeek������˱�����Ϊ����ѽ��յ�����Ϣ�Ӷ��������������ֻȡ����Ϣ�Ŀ���
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/

BSAPI_API
int bs_mq_pop(BSHANDLE &hHandle,chen::MemCache &pBuf,unsigned int &uDataLen,std::string &sLabel,time_t &cTime,
			  unsigned __int64 &uID,unsigned int nTimer=0,bool isPeek=false);


/* 
* ��һ���򿪵�MQ�з���һ��chen::VariableData���͵���Ϣ
* hHandle���Ѿ��򿪵�MQ������˾������ͨ��bs_mq_open��
* vData�����Ҫ������Ϣ�ı���
* sLable��Ҫ������Ϣ��Label�ַ�����'\0'��β
* uID�����ش�����MQ�����е�ID
* nLevel���˱�������ָ��������Ϣ�����еļ��𣬽������ȶ�����Ч
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_mq_push(BSHANDLE &hHandle,const chen::VariableData &vData,const char *sLabel,unsigned __int64 &uID,
			   unsigned char nLevel=BS_MQ_COMMONPRIORITY);


/*
* ��һ���򿪵�MQ�н���һ��chen::VariableData���͵���Ϣ���˺���ֻ�ܽ��շ��͵���Ϣ��Ϊchen::VariableData���͵���Ϣ
* hHandle���Ѿ��򿪵�MQ������˾������ͨ��bs_mq_open��
* vData�����ؽ��յ�����Ϣ
* sLabel�����ؽ��յ���Ϣ��Label
* cTime�����ش���MQ�Ĵ���ʱ��
* uID����ֵΪ������ն��������һ����Ϣ���������ָ��ID����Ϣ����ͨ���˱������ؽ��յ���Ϣ��ID
* nTimer����������ܵĶ���Ϊ�գ���ֵ����Ҫ�ȴ���ʱ�䣬Ϊ�����Ϸ��أ�ΪBS_TIMER_INFINITE�����޵ȴ�
*		  ֱ������Ϣ���ػ����
* isPeek������˺�����Ϊ����ѽ��յ�����Ϣ�Ӷ��������������ֻȡ����Ϣ�Ŀ���
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_mq_pop(BSHANDLE &hHandle,chen::VariableData &vData,std::string &sLabel,time_t &cTime,
			  unsigned __int64 &uID,unsigned int nTimer=0,bool isPeek=false);

/*
* ��һ���򿪵�MQ�з���һ���ַ����͵���Ϣ
* sData��Ҫ���͵��ַ�����Ϣ
* ���������ο�����ͬ�ຯ��
*/
BSAPI_API
int bs_mq_push(BSHANDLE &hHandle,const std::string &sData,const char *sLabel,unsigned __int64 &uID,unsigned char nLevel=BS_MQ_COMMONPRIORITY);

/*
* ��һ���򿪵�MQ�н���һ���ַ������͵���Ϣ���˺���ֻ�ܽ��շ��͵���Ϣ��Ϊ�ַ����͵���Ϣ
* sData�����ؽ��յ����ַ�����Ϣ
* ���������ο�����ͬ�ຯ��
*/
BSAPI_API
int bs_mq_pop(BSHANDLE &hHandle,std::string &sData,std::string &sLabel,time_t &cTime,
			  unsigned __int64 &uID,unsigned int nTimer=0,bool isPeek=false);

/*
* ��ȡһ����MQ�е���Ϣ����
* hHandle���Ѿ��򿪵�MQ������˾������ͨ��bs_mq_open��
* uCount������MQ����Ϣ����
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_mq_length(BSHANDLE &hHandle,unsigned __int64 &uCount);

/*
* ���һ���򿪵�MQ
* hHandle���Ѿ��򿪵�MQ������˾������ͨ��bs_mq_open��
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_mq_clear(BSHANDLE &hHandle);

/*
* ɾ��һ���򿪵�MQ
* hHandle���Ѿ��򿪵�MQ������˾������ͨ��bs_mq_open��
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_mq_delete(BSHANDLE &hHandle);

/////////////////////////////////////////////////////////////////

/*
* ����һ���µ�treeDB�ļ�
* sDBFile�����ݿ��ļ���������Ϊ��
* sHostName�����ݿ��������ַ
* uPort�������Socket��ʽͨѶ����ָ��Ҫ���ӵ����ݿ������������TCP�˿ڣ�
		 Ϊ0��ʹ��Ĭ�϶˿�.
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_create_newfile(const char *sDBFile,const char *sHostName,unsigned short uPort);


/*
* ��ѯ����������
* mainKeyNameList������ָ�����ݿ����������������б�
* sDBFile��ָ����ѯ�����ݿ��ļ����������ֵΪNULL��ʹ��Ĭ�����ݿ��ļ�
* sHostName�����ݿ��������ַ
* uPort�������Socket��ʽͨѶ����ָ��Ҫ���ӵ����ݿ������������TCP�˿ڣ�
		 Ϊ0��ʹ��Ĭ�϶˿�.
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_query_mainkeys(std::list< std::string > &mainKeyNameList,const char *sDBFile,const char *sHostName,
							 unsigned short uPort=0);

/*
* �򿪻򴴽�һ��������������
* hHandle������ɹ�������������ʼֵӦ��ΪBS_INVALID_HANDLE���˾���������ͨ��bs_close_handle�����ر�
* sMainKey����������
* sSubKeyPath���Ӽ�·��
* sPWD������������
* uFlag���򿪼��ķ�ʽ
* sDBFile��ָ�����ݿ��ļ�������˲�ΪNULL���Ĭ���ļ�
* sHostName�����ݿ��������ַ
* uPort�������Socket��ʽͨѶ����ָ��Ҫ���ӵ����ݿ������������TCP�˿ڣ�
		 Ϊ0��ʹ��Ĭ�϶˿�.
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_open(BSHANDLE &hHandle,const char *sMainKey,const char *sSubKeyPath,const char *sPWD,unsigned int uFlag,
				const char *sDBFile,const char *sHostName,unsigned short uPort=0);

/*
* �������о����һ���¼�
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sMainKey����������
* sSubKeyPath���Ӽ�·��
* sPWD������������
* uFlag���򿪼��ķ�ʽ
* sDBFile��ָ�����ݿ��ļ�������˲�ΪNULL���Ĭ���ļ�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_reopen(BSHANDLE &hHandle,const char *sMainKey,const char *sSubKeyPath,const char *sPWD,unsigned int uFlag,
					 const char *sDBFile);
/*
* ����һ���Ѵ��������Ӽ�·��
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sSubKeyPath���µ��Ӽ�·���������ֵΪNULL���������Ŀ¼
* uFlag���򿪼��ķ�ʽ
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_open_subkey(BSHANDLE &hHandle,const char *sSubKeyPath,unsigned int uFlag);

/*
* ��ʼһ�����˾������������
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* ����һ�����óɹ���ͨ���˾�������в�����������һ�����񣬱���ͨ����bs_treedb_commit_transaction����
* ʹ������Ч�������bs_treedb_rollback_transaction�ع������Ĳ��������û�е���bs_treedb_commit_transaction
* �����ʱ��ر�ʱ�����Զ����ûع��������в���������˾������bs_treedb_reopenʱ�ı�������
* ���ļ���������ᱻ�ع������Ҿ������û�������״̬������þ���Ѿ��ɹ����ù��˺�������û�е���
* bs_treedb_commit_transaction����ύ��������ٴε�����������������¿�ʼһ����
* ��������������ع���
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_begin_transaction(BSHANDLE &hHandle);

/*
* �ύ���˾������������
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* �ɹ����ô˺������ύ��hHandle����ɹ���bs_treedb_begin_transaction����֮������в���
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_commit_transaction(BSHANDLE &hHandle);

/*
* �ع����˾������������
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* �ɹ����ô˺������ع�����hHandle����ɹ���bs_treedb_begin_transaction����֮������в���
* ����ع����п��ܻ�������ָ���ڲ���ȷ��״̬���������bs_treedb_reopen���´�һ��������رվ�����´򿪡�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_rollback_transaction(BSHANDLE &hHandle);



/*
* ��ѯһ��treedb�������Ϣ
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sMainKey������������Ϣ
* sSubKeyPath�������Ӽ�·����Ϣ
* sHostName�����ط�������ַ��Ϣ
* sDFBFile������treedb���ݿ��ļ���Ϣ
* uFlag�����ؼ��Ĵ򿪷�ʽ��Ϣ
* uPort�����ط������˿���Ϣ
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_query_handle(BSHANDLE &hHandle,std::string &sMainKey,std::string &sSubKeyPath,std::string &sHostName,
						   unsigned int &uFlag,std::string &sDFBFile,unsigned int &uPort);

/*
* ͨ�������һ���Ѵ򿪵ļ��в���һ���Ӽ�
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sSubKey�����Ӽ�
* uFlag���򿪼��ķ�ʽ
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_insert_subkey(BSHANDLE &hHandle,const char *sSubKey,unsigned int uFlag);

/*
* ͨ�������һ���Ѵ򿪵ļ��в���һ���Ӽ�
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sSubKey�����Ӽ�����Ϊ�ջ�ΪNULL���Զ���˳�������¼���
* sKeyName�������������Ӽ�������
* uFlag���򿪼��ķ�ʽ
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_insert_subkey(BSHANDLE &hHandle,const char *sSubKey,std::string &sKeyName,unsigned int uFlag);

/*
* ͨ�����ɾ��ָ�������Ӽ�
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sSubKey��Ҫɾ�����Ӽ�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_delete_subkey(BSHANDLE &hHandle,const char *sSubKey);

/*
* ͨ�����ɾ��ָ���ļ�(ע�⣺�˺�������ɾ����������Ҫɾ�����Ե���OpenKey����)
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
* ע�⣺ɾ���ɹ��󣬾����ָ������λ��
*/
BSAPI_API
int bs_treedb_delete_key(BSHANDLE &hHandle);


/*
* ���ľ��ָ�����ĳ���Ӽ�������
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sSubKey��Ҫ���ĵ��Ӽ�����
* sNewKeyName�����ĺ��������
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_rename_subkey(BSHANDLE &hHandle,const char *sOldKeyName,const char *sNewKeyName);

/*
* ���ľ��ָ���������(ע�⣺���ܸ�������������������Ҫ��OpenKey)
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sNewKeyName�����ĺ��������
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_rename_key(BSHANDLE &hHandle,const char *sNewKeyName);



/*
* ��ȡ�����ָ���Ӽ��������Ӽ����б�
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* subKeys�������Ӽ��б�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_get_subkeys(BSHANDLE &hHandle,std::list< std::string > &subKeys);

/*
* ����ָ����Ӽ��²���һ������
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sPropertyName����������
* pBuf������������ֵ������
* uBufLen������ֵ��������С
* bOverWrite���������ֵ�Ѿ����ڣ��˲���Ϊ���򸲸�ԭ����ֵ�����򲻸��Ƿ��ش�����
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_insert_property(BSHANDLE &hHandle,const char *sPropertyName,const char *pBuf,unsigned int uBufLen,bool bOverWrite=false);

/*
* ��ȡ���ָ����Ӽ���ĳ������ֵ
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sPropertyName����������
* pBuf����ŷ��صĶ���������ֵ�Ļ�����
* uBufLen������������ֵ�Ĵ�С
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_get_property(BSHANDLE &hHandle,const char *sPropertyName,chen::MemCache &pBuf,unsigned int &uBufLen);

/*
* ����ָ����Ӽ��²���һ��VariableData���͵�����
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sPropertyName����������
* vData������ֵ����
* bOverWrite���������ֵ�Ѿ����ڣ��˲���Ϊ���򸲸�ԭ����ֵ�����򲻸��Ƿ��ش�����
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_insert_property(BSHANDLE &hHandle,const char *sPropertyName,const chen::VariableData &vData,bool bOverWrite=false);

/*
* ��ȡ���ָ����Ӽ���ĳ��VariableData���͵�����ֵ
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sPropertyName����������
* vData���洢����VariableData��������ֵ���ݵı���
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_get_property(BSHANDLE &hHandle,const char *sPropertyName,chen::VariableData &vData);

/*
* �༭���ָ����Ӽ��µ�ĳ������ֵ
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sPropertyName����������
* pBuf������������ֵ������
* uBufLen������ֵ��������С
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_edit_property(BSHANDLE &hHandle,const char *sPropertyName,const char *pBuf,unsigned int uBufLen);

/*
* �༭���ָ����Ӽ��µ�ĳ������ֵ
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* vData������ֵ����
* uBufLen������ֵ��������С
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_edit_property(BSHANDLE &hHandle,const char *sPropertyName,const chen::VariableData &vData);


/*
* ��ȡ���ָ������������Ե������б�
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* subKeys����ŷ������������б�ı���
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_get_all_property_names(BSHANDLE &hHandle,std::list< std::string > &propertyNames);

/*
* ��������ɾ�����ָ�����ĳ������
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sPropertyName����������
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_delete_property(BSHANDLE &hHandle,const char *sPropertyName);

/*
* ��������ɾ�����ָ�������������
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_delete_all_property(BSHANDLE &hHandle);


/*
* ���ľ��ָ���ĳ�����Ե�����
* hHandle���Ѿ��򿪵ļ�������˾������ͨ��bs_treedb_open��
* sOldPropertyName������������
* sNewPropertyName������������
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_treedb_rename_property(BSHANDLE &hHandle,const char *sOldPropertyName,const char *sNewPropertyName);

//////////////////////////////////////////////////////////////////////

/*
* ��ȡ�������ݿ������б�
* DBNameList�������������ݿ������б�
* sHostName�����ݿ����ڵķ�������ַ
* uPort�������Socket��ʽͨѶ����ָ��Ҫ���ӵ����ݿ������������TCP�˿ڣ�
		 Ϊ0��ʹ��Ĭ�϶˿�.
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_get_all_dbnames(std::list< std::string > &DBNameList,const char *sHostName,unsigned short uPort=0);

/*
* �򿪡�������ɾ��һ�����ݿⲢ��������(ɾ������)
* hHandle������ɹ�������������ʼֵӦ��ΪBS_INVALID_HANDLE���˾���������ͨ��bs_close_handle�����ر�
* sDBName��Ҫ�򿪻򴴽������ݿ�����
* sUser���򿪻򴴽����ݿ���û���
* sPWD���û�����
* uOFlag�������ݿ�ķ����ο�tabledb_def.h�еĶ���
* uDBFlag�����������ֵָ�����ݿ�ķ��
* sHostName�����ݿ��������ַ
* uPort�������Socket��ʽͨѶ����ָ��Ҫ���ӵ����ݿ������������TCP�˿ڣ�
		 Ϊ0��ʹ��Ĭ�϶˿�.
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_open(BSHANDLE &hHandle,const char *sDBName,const char *sUser,const char *sPWD,unsigned int uOFlag,unsigned int uDBFlag,
					const char *sHostName,unsigned short uPort=0);

/*
* ����������
* sDBName��Ҫ�����������ݿ�����
* sNewDBName�����ݿ��������
* sUser���򿪻򴴽����ݿ���û���
* sPWD���û�����
* sHostName�����ݿ��������ַ
* uPort�������Socket��ʽͨѶ����ָ��Ҫ���ӵ����ݿ������������TCP�˿ڣ�
		 Ϊ0��ʹ��Ĭ�϶˿�.
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_rename(const char *sDBName,const char *sNewDBName,const char *sUser,const char *sPWD,
					const char *sHostName,unsigned short uPort=0);


/*
* ��ѯ���ݿ�������
* hHandle�����ݿ�������bs_tabledb_open��bs_tabledb_repoen�򿪹��ģ����򷵼Ӵ���
* sDBName���������ݿ�����
* sUser�������û�����
* uOFlag�����ش����ݿ����õķ���
* uDBFlag����ʱ����Ǵ�������˲������ش������ݿ�ķ��
* sHostName���������ݿ�������ĵ�ַ
* uPort�������Socket��ʽͨѶ�򷵻�Ҫ���ӵ����ݿ��������ʹ�õĶ˿�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_qurey_handle(BSHANDLE &hHandle,std::string &sDBName,std::string &sUser,std::string &sPWD,unsigned int &uOFlag,unsigned int &uDBFlag,
							std::string &sHostName,unsigned short &uPort);

/*
* �������о����һ�������ݿ�
* hHandle���Ѿ�ͨ��bs_tabledb_open�򿪹��ľ��
* sDBName��Ҫ�򿪵������ݿ�����
* sUser��Ҫ�����ݿ���û���
* sPWD���û�����
* uOFlag�������ݿ�ķ����ο�tabledb_def.h�еĶ���
* uDBFlag�����������ֵָ�����ݿ�ķ��
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_reopen(BSHANDLE &hHandle,const char *sDBName,const char *sUser,const char *sPWD,unsigned int uOFlag,unsigned int uDBFlag);

/*
* ��ʼһ�����˾������������
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* ����һ�����óɹ���ͨ���˾�������в�����������һ�����񣬱���ͨ����bs_tabledb_commit_transaction����
* ʹ������Ч�������bs_tabledb_rollback_transaction�ع������Ĳ��������û�е���bs_tabledb_commit_transaction
* �����ʱ��ر�ʱ�����Զ����ûع��������в���������˾������bs_treedb_reopenʱ�ı�������
* ���ļ���������ᱻ�ع������Ҿ������û�������״̬������þ���Ѿ��ɹ����ù��˺�������û�е���
* bs_tabledb_commit_transaction����ύ��������ٴε�����������������¿�ʼһ����
* ��������������ع���
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_begin_transaction(BSHANDLE &hHandle);

/*
* �ύ���˾������������
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* �ɹ����ô˺������ύ��hHandle����ɹ���bs_tabledb_begin_transaction����֮������в���
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_commit_transaction(BSHANDLE &hHandle);

/*
* �ع����˾������������
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* �ɹ����ô˺������ع�����hHandle����ɹ���bs_tabledb_begin_transaction����֮������в���
* ����ع����п��ܻ�������ָ���ڲ���ȷ��״̬���������bs_tabledb_reopen���´�һ��������رվ�����´򿪡�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_rollback_transaction(BSHANDLE &hHandle);


typedef std::list< bs_tdfield * > BSFIELDINFOLIST;

/*
* ������
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* lFieldList���ֶ���Ϣ�б�
* uFlag�����񣬲ο�tabledb_def.h
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_create_table(BSHANDLE &hHandle,const char *sTableName,BSFIELDINFOLIST &lFieldList,unsigned int uFlag);

/*
* ��ȡ���ݿ������еı���
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* tableNameList������ɹ�ͨ�����б������б�����
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_get_all_tablenames(BSHANDLE &hHandle,std::list< std::string > &tableNameList);

/*
* ɾ����
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��Ҫɾ���ı�����
* uType��ɾ����ķ����μ�tabledb_def.h
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_delete_table(BSHANDLE &hHandle,const char *sTableName,unsigned short uType);

/*
* ��������
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName����Ҫ�����ı�����
* sNewName��������
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_rename_table(BSHANDLE &hHandle,const char *sTableName,const char *sNewName);

/*
* ��ȡ����ֶ���Ϣ
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��Ҫɾ���ı�����
* lFieldList�����ص��ֶ���Ϣ�б��������Ҫ�û�ɾ���б��е�bs_tdfield����ָ��
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_get_fields(BSHANDLE &hHandle,const char *sTableName,BSFIELDINFOLIST &lFieldList);

typedef std::list < chen::VariableData > BSFIELDVALUELIST;

/*
* ����в�������
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��������
* lRecordValueList��Ҫ������ֶ������б�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_insert_record(BSHANDLE &hHandle,const char *sTableName,BSFIELDVALUELIST &lRecordValueList);

/*
* ���ݵ����������¼�¼
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��������
* lFieldList��Ҫ���¼�¼���ֶ��б�������б�Ϊ������Ϊ����������¼�������ֶε�ֵ
* lRecordValueList��Ҫ���µ��ֶ������б���lFieldList��Ϊ�գ����б�Ҫ��lFieldList�������Ӧ��������б�ΪҪ����
					��¼�����ֶε�ֵ��
* sCondField����Ϊ����������������
* vFValue�����������еĶԱ�ֵ
* pEndValue�����������еĶԱ�ֵ�������ֵ��Ϊ�գ�������vFValueΪ��ʼֵ��pEndValueΪ����ֵ��Ϊ�������������������ļ�¼
			��������,�����ֵΪ�գ�����vFValue��uCondition��Ϊ���������������ֵΪ��,��uCondition���λ0x1Ϊ����������������ʼֵ(vFValue)�����λ0x2Ϊ����
			�������������βֵ(pEndValue)��������������λȫΪ����ʼֵ�ͽ��ֵȫ���ԡ�
* uCondition�����������ο�tabledb_def.h�е�condition
* uOpFlag�����������ο�tabledb_def.h�е�condition flag������˱�־ΪBS_FIELD_COND_USE_CTIME���򰴴���ʱ����˺���sCondField
* uUpdateCount������ɹ����ظ��µļ�¼����
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_update_record_by_field(BSHANDLE &hHandle,const char *sTableName,std::list< std::string > &lFieldList,
									BSFIELDVALUELIST &lRecordValueList,const char *sCondField,chen::VariableData &vFValue,
									chen::VariableData *pEndValue,unsigned char uCondition,unsigned char uOpFlag,unsigned __int64 &uUpdateCount);

/*
* ���ݵ�������ɾ����¼
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��������
* sCondField����Ϊ����������������
* vFValue�����������еĶԱ�ֵ
* pEndValue�����������еĶԱ�ֵ�������ֵ��Ϊ�գ�������vFValueΪ��ʼֵ��pEndValueΪ����ֵ��Ϊ�������������������ļ�¼
			����ɾ��,�����ֵΪ�գ�����vFValue��uCondition��Ϊ���������������ֵΪ��,��uCondition���λ0x1Ϊ����������������ʼֵ(vFValue)�����λ0x2Ϊ����
			�������������βֵ(pEndValue)��������������λȫΪ����ʼֵ�ͽ��ֵȫ���ԡ�
* uCondition�����������ο�tabledb_def.h�е�condition
* uOpFlag�����������ο�tabledb_def.h�е�condition flag������˱�־ΪBS_FIELD_COND_USE_CTIME���򰴴���ʱ����˺���sCondField
* uDeleteCount������ɹ����ر�ɾ���ļ�¼����
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_delete_record_by_field(BSHANDLE &hHandle,const char *sTableName,const char *sCondField,chen::VariableData &vFValue,
									chen::VariableData *pEndValue,unsigned char uCondition,unsigned char uOpFlag,unsigned __int64 &uDeleteCount);
/*
* ���ݴ���ʱ��ɾ����¼
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��������
* sCondField����Ϊ����������������
* tBeginTime��ʱ��Ա�ֵ
* pEndTime��ʱ��ĶԱ�ֵ�������ֵ��Ϊ�գ�������tBeginTimeΪ��ʼֵ��pEndTimeΪ����ֵ��Ϊ�������������������ļ�¼
			����ɾ��,�����ֵΪ�գ�����tBeginTime��uCondition��Ϊ��������
* uCondition�����������ο�tabledb_def.h�е�condition
* uOpFlag�����������ο�tabledb_def.h
* uDeleteCount������ɹ����ر�ɾ���ļ�¼����
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_delete_record_by_ctime(BSHANDLE &hHandle,const char *sTableName,time_t tBeginTime,time_t *pEndTime,
									  unsigned char uCondition,unsigned char uOpFlag,unsigned __int64 &uDeleteCount);

/*
* ����¼����ʱ���ȡ��������ļ�¼
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��������
* uCount���޶�Ҫ��ȡ�ļ�¼�������ɹ��󴫳�ʵ�ʻ�ȡ�ļ�¼��
* hRecordSet������ɹ��򷵻�RecordSet�������ָ������ȡ�ļ�¼�����˾���������ͨ��bs_close_handle�����ر�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_get_record(BSHANDLE &hHandle,const char *sTableName,ui32 &uCount,BSHANDLE &hRecordSet);

/*
* ���ݵ���������ȡ��¼
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��������
* sCondField����Ϊ����������������
* vFValue�����������еĶԱ�ֵ
* pEndValue�����������еĶԱ�ֵ�������ֵ��Ϊ�գ�������vFValueΪ��ʼֵ��pEndValueΪ����ֵ��Ϊ�������������������ļ�¼
			������ȡ,�����ֵΪ�գ�����vFValue��uCondition��Ϊ���������������ֵΪ��,��uCondition���λ0x1Ϊ����������������ʼֵ(vFValue)�����λ0x2Ϊ����
			�������������βֵ(pEndValue)��������������λȫΪ����ʼֵ�ͽ��ֵȫ���ԡ�
* uCondition�����������ο�tabledb_def.h�е�condition
* uOpFlag�����������ο�tabledb_def.h�е�condition flag������˱�־ΪBS_FIELD_COND_USE_CTIME���򰴴���ʱ����˺���sCondField
* uLimitCount�� �޶�Ҫ��ȡ�ļ�¼�����������ֵΪ0���޶����ɹ���˱�������ʵ�ʻ�ȡ�ļ�¼��
* hRecordSet������ɹ��򷵻�RecordSet�������ָ������ȡ�ļ�¼�����˾���������ͨ��bs_close_handle�����ر�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_get_record_by_field(BSHANDLE &hHandle,const char *sTableName,const char *sCondField,chen::VariableData &vFValue,
									chen::VariableData *pEndValue,unsigned char uCondition,unsigned char uOpFlag,unsigned int &uLimitCount,BSHANDLE &hRecordSet);

/*
* ������¼����������Ϣ
* hRecordSet����¼��������˾������ͨ��bs_tabledb_get_record_by_field��bs_tabledb_get_record����
* uRecordCount�����ؼ�¼���еļ�¼����
* lFieldList�����ؼ�¼�����ֶ���Ϣ
* uTableFlag�����ر���
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_recordset_info(BSHANDLE &hRecordSet,ui32 &uRecordCount,BSFIELDINFOLIST &lFieldList,ui32 &uTableFlag);

/*
* ��ȡ��¼��������α���
* hRecordSet����¼��������˾������ͨ��bs_tabledb_get_record_by_field��bs_tabledb_get_record����
* hCursor������ɹ���ͨ���˱��������α������˾���������ͨ��bs_close_handle�����ر�
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_get_cursor(BSHANDLE &hRecordSet,BSHANDLE &hCursor);

typedef stdext::hash_map<std::string ,chen::VariableData> FIELDVALUEMAP;

/*
* �����α������α���ָ��ĵ�ǰ��¼��Ϣ�����ƶ��α�ָ����һ����¼
* hCursor���α������˾������ͨ��bs_tabledb_get_cursor����
* fvMap������ɹ����˱������ص�ǰ��¼����
* ctm������ɹ����˱������ص�ǰ��¼�Ĵ���ʱ��
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����,����BSAPI_CURSOR_END�����Ѿ�����¼����β
*/
BSAPI_API
int bs_tabledb_cursor_next(BSHANDLE &hCursor,FIELDVALUEMAP &fvMap,time_t &ctm);

/*
* �ƶ��α����е��α�ָ����һ����¼���������ƶ���ָ���¼����Ϣ
* hCursor���α������˾������ͨ��bs_tabledb_get_cursor����
* fvMap������ɹ����˱������ؼ�¼����
* ctm������ɹ����˱������ؼ�¼����ʱ��
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش����룬����BSAPI_CURSOR_BEGIN�����Ѿ�����¼��ͷλ��
*/
BSAPI_API
int bs_tabledb_cursor_pre(BSHANDLE &hCursor,FIELDVALUEMAP &fvMap,time_t &ctm);

/*
* �����α����е��α꣬ʹ��ָ���¼���Ŀ�ʼλ��
* hCursor���α������˾������ͨ��bs_tabledb_get_cursor����
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_cursor_reset(BSHANDLE &hCursor);

///////////////////dyn/////////////////////

/*
* ���dyn���ͱ��dyn��Ϣ
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��dyn������
* dyn������ɹ����˱�������dyn��Ϣ
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_get_dyn(BSHANDLE &hHandle,const char *sTableName,td_dyn &dyn);

/*
* ���dyn���ͱ��dyn��Ϣ�����һ����¼
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sTableName��dyn������
* dyn������ɹ����˱�������dyn��Ϣ
* fvMap�����һ����¼��Ϣ
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/
BSAPI_API
int bs_tabledb_get_dyn_and_lastrd(BSHANDLE &hHandle,const char *sTableName,td_dyn &dyn,FIELDVALUEMAP &fvMap);

/*
* ͨ������������ת����dyn��Ϣ��һ����¼
* pData�������ƻ�����ָ��
* nDLen����������С
* dyn������ɹ����˱�������dyn��Ϣ
* fvMap�����һ����¼��Ϣ
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
*/

BSAPI_API
int bs_parse_alert_data(const char *pData ,int nDLen,td_dyn &dyn,FIELDVALUEMAP &fvMap);


/*
* ��ȡ���ݿ�������dyn���״̬��Ϣ
* hHandle�����ݿ������˾������ͨ��bs_tabledb_open����
* sGroupName������˱�����ֵ��Ϊ�գ����ȡ���б�����ǰ�沿������ַ�����ͬ�ı��״̬��Ϣ��Ϊ�����ȡ���б��״̬��Ϣ
* nStateMap�����isQuickMode����Ϊ�棬��ȡ�ɹ���˱���������unsigned intΪkey����unsigned char(״̬)Ϊvalue��hashmap����map��key�Ǳ�����ͨ���ض��㷨
			 ���ɵ�unsigned int�����ݣ�valueΪ״ֵ̬�����isQuickMode��Ϊ����˱����������á�
* sStateMap�������ȡ�ɹ����˱��������Ա���Ϊkey��״̬Ϊvalue��hashmap�����isQuickModeΪ�棬��˱���ֻ����nStateMap������keyֵ�������ظ��ı��״̬
			 ��Ϣ(�˱���ֻ���������ظ���ı�״̬��Ϣ����һ��������ͬunsigned int Key�ı���Ϣ�԰�����nStateMap��)�����isQuickMode��Ϊ����˱�������
			 ȫ��dny����������ƺ�״̬��Ϣ��
* isQuickMode�����ƻ�ȡ��״̬��Ϣ�ķ�ʽ���ο�������������
* ����ֵ������ɹ�����BS_NOERROR���򷵻ش�����
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

