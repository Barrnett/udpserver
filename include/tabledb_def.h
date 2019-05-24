#ifndef _BS_TABLEDBDEF_H_
#define _BS_TABLEDBDEF_H_
#include "bluesky_int.h"

#define TBDB_TABLE_MAXFIELDCOUNT		4096

#define BS_TD_MAXUSERNAME	20
#define BS_TD_MAXPWD		30
#define BS_TD_DBNAME	30


#define BS_TD_MAXTALBENAME		100
	//���ڴ�������������ʱ��ʡ�ռ�(ȡ��λ���ĳߴ�)
#define BS_TD_MEDIANTABLENAMELEN	40




/////////////Open DB flag////////////////////////

#define TBDB_OPDBF_CREATENEW		0x00000001
#define TBDB_OPDBF_OPENEXIST		0x00000002
#define TBDB_OPDBF_DELETEDB			0x00000004

/////////////////////////////////////////////////

//////////Create table flag//////////////////////

	//�д˱�־������һ���Զ�������ID����
#define BS_TDTF_FLAG_AUTOID			0x00000001
	//�̶����ȼ�¼ģʽ���ڴ�ģʽ�·ǹ̶��ߴ�����ڴ���ʱ����ָ�����ߴ����ַ����Ͷ��������ݵ�
#define BS_TDTF_FLAG_FIXEDRECORD	0x00000002
	//�����Զ�����iDʱ������ô˱�־����Զ�����ID����������Ϊ����
#define BS_TDTF_FLAG_INDEXID		0x00000004
	//���ô˱�־λ���ʾ�������ݿ��Լ�����������ID�⣬������һ������Ҫ�Զ�����(�ڴ˱����Ϊ�����Ӳ����ٶ�)
#define BS_TDTF_FLAG_AUTOINCREASE	0x00000008
	//���ô˱�־λ��ͳ�Ʊ�ռ�õ�ҳ����
#define BS_TDTF_FLAG_COUNTPAGE		0x00000020
	//���ô˱�־λ��ʾ��������г���������������һ��������������(�ڴ˱����Ϊ�������ٶ�)
#define BS_TDTF_FLAG_INDEXFIELD		0x00000040
	//���ô˱�־λ��ʾ���Ǹ��������͵ı���ʱ�����ȡ����������ȥȷ���е�λ�ö�����������ʽ
#define BS_TDTF_FLAG_PRIMARYTABLE	0x00000080
	//���ô˱�־λ��ʾ����������һ���ֶ�ΪVDT_STR����VDT_BIN��������(Ϊ�˼ӿ�����ٶ�)
#define BS_TDTF_FLAG_VARFIELD		0x00000100
	//ָ������������Ϊ����ģʽ����Ϊ����
#define BS_TDTF_FLAG_PMKI_DESCORDER 0x00000200
	//���ô˱�־λ�������Ǹ�DYN��
#define BS_TDTF_FLAG_DYNTABLE		0x00040000


////////////condition/////////////////////////////
#define TBDB_FMC_GREATERTHAN		0x01
#define TBDB_FMC_LESSTHAN			0x02
#define TBDB_FMC_EQUAL				0x03
#define TBDB_FMC_EQUALORGREATERTHAN 0x04
#define TBDB_FMC_EQUALORLESSTHAN	0x05
#define TBDB_FMC_UNEQUAL			0x06
//////////////////////////////////////////////////

/////////////////////condition flag////////////////////////////

#define BS_FIELD_COND_USE_END	0x01
//�˱��Ϊ��ʱ����������ʱ�����
#define BS_FIELD_COND_USE_CTIME 0x02
#define BS_FIELD_COND_USE_DELETEALL		0x04

////////////////////////////////////////////////////

//////////////delete table type//////////////////
//����ɾ��һ����������������ͷſռ�
#define TBDB_DT_QUICK				0x01
//����ɾ��һ�������ͷű���ռ�Ŀռ�
#define TBDB_DT_RS					0x02
////////////////////////////////////////////////

////////////////////�ڴ��м�¼����ǰ׺////////////////////
struct bs_tdrdsetprex
{
	ui16 uCBLen;
	ui32 uRecordCount;
	ui32 uDataLen;
	ui32 uTableFlag;
};
#define BS_TDRDSETPREXLEN	14

#define BS_TBDB_PTORDSETPREX(h,p) { \
	h.uCBLen=bo_bufuint2(p);	\
	h.uRecordCount=bo_bufuint4(p+2); \
	h.uDataLen=bo_bufuint4(p+6);	\
	h.uTableFlag=bo_bufuint4(p+10);	\
}

/////////////�ڴ��¼����ÿ����¼��ǰ׺//////////////
struct bs_tdrdprex
{
	ui16 uCBLen;
	ui32 uCheckSum;
	ui32 uDataLen;
	ui64 uCreateTime;
};
#define BS_TDRDPREX_LEN		18

#define BS_TBDB_PTORDPREX(h,p) { \
	h.uCBLen=bo_bufuint2(p);		\
	h.uCheckSum=bo_bufuint4(p+2);	\
	h.uDataLen=bo_bufuint4(p+6);	\
	h.uCreateTime=bo_bufuint8(p+10); \
}

///////////////////�ڴ��б��ֶνṹ/////////////////////
struct bs_tdfield
{
#define BS_TD_MAXFIELDNAME		32
	i8 name[BS_TD_MAXFIELDNAME];
	i8 uType;
	//���ô˱�־����д�������
#define BS_TDFF_CREATEINDEX			0x0001
	//���ô˱�־���ʾ����ֵ�������ظ���
#define BS_TDFF_UNIQUE				0x0002
	//���ô˱�־��ʾ���ֶ�Ϊ����
#define BS_TDFF_PRIMARYKEY			0x0004
	//ָδ���ֶ�Ϊ����id�Զ�,��ʱ�������ͱ���Ϊ������
#define BS_TDFF_AUTOINCREASE		0x0008
	//���ô˱�־ָʾ��������������Ϊ���򣬷���Ϊ����
#define BS_TDFF_INDEX_DESC			0x0020
	ui16 uFlag;
	//����ǹ̶����ȼ�¼ģʽ�����ֵ����ǹ̶����ȵ��������͵��޶����ȣ����ַ����Ͷ��������ݵ�
	ui32 uDataLen;

};
#define BS_TDFIELD_LEN		(7+BS_TD_MAXFIELDNAME)

#define BS_TDFIELDTOP(h,p) {			\
	memcpy(p,h.name,BS_TD_MAXFIELDNAME);		\
	*(p+BS_TD_MAXFIELDNAME)=h.uType;		\
	bo_int2buf(h.uFlag,p+BS_TD_MAXFIELDNAME+1);		\
	bo_int4buf(h.uDataLen,p+BS_TD_MAXFIELDNAME+3);	\
}

#define BS_PTOTDFIELD(h,p) {		\
	memcpy(h.name,p,BS_TD_MAXFIELDNAME);	\
	h.uType=(i8)(*(p+BS_TD_MAXFIELDNAME));	\
	h.uFlag=bo_bufuint2(p+BS_TD_MAXFIELDNAME+1); \
	h.uDataLen=bo_bufuint4(p+BS_TD_MAXFIELDNAME+3); \
}


/////////////////////////////DYN////////////////////////////////////////////////

#define	MSTATUS_OK			1
#define	MSTATUS_WARNING		2
#define	MSTATUS_ERROR		3
#define	MSTATUS_BAD			4
#define	MSTATUS_DISABLE		5
#define	MSTATUS_NULL		0

struct td_dyn
{
	ui16 uCBLen;
	ui64 tLastChangeTime;
	ui8	uState;
	ui8 uPreState;
	ui64 tLastStateChangeTime;
	ui64 uLastStateKeepTimes;
};
#define BS_TBDB_DYNLEN	28

#define BS_PTOTDDYN(h,p) {		\
	h.uCBLen=bo_bufuint2(p);		\
	h.tLastChangeTime=bo_bufuint8(p+2); \
	h.uState=(ui8)(*(p+10));	\
	h.uPreState=(ui8)(*(p+11));		\
	h.tLastStateChangeTime=bo_bufuint8(p+12); \
	h.uLastStateKeepTimes=bo_bufuint8(p+20); \
}
///////////////////////////////////////////////////////////////////////////////



#endif