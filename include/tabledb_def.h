#ifndef _BS_TABLEDBDEF_H_
#define _BS_TABLEDBDEF_H_
#include "bluesky_int.h"

#define TBDB_TABLE_MAXFIELDCOUNT		4096

#define BS_TD_MAXUSERNAME	20
#define BS_TD_MAXPWD		30
#define BS_TD_DBNAME	30


#define BS_TD_MAXTALBENAME		100
	//用于创建表名称索引时节省空间(取中位数的尺寸)
#define BS_TD_MEDIANTABLENAMELEN	40




/////////////Open DB flag////////////////////////

#define TBDB_OPDBF_CREATENEW		0x00000001
#define TBDB_OPDBF_OPENEXIST		0x00000002
#define TBDB_OPDBF_DELETEDB			0x00000004

/////////////////////////////////////////////////

//////////Create table flag//////////////////////

	//有此标志将增加一个自动增长的ID的列
#define BS_TDTF_FLAG_AUTOID			0x00000001
	//固定长度记录模式：在此模式下非固定尺寸的列在创建时必须指定最大尺寸如字符串和二进制数据等
#define BS_TDTF_FLAG_FIXEDRECORD	0x00000002
	//当有自动增长iD时如果设置此标志则对自动增长ID建索引并设为主键
#define BS_TDTF_FLAG_INDEXID		0x00000004
	//设置此标志位则表示除了数据库自己增加自增加ID外，最少有一个列需要自动增长(在此标记是为了增加插入速度)
#define BS_TDTF_FLAG_AUTOINCREASE	0x00000008
	//设置此标志位则统计表占用的页面数
#define BS_TDTF_FLAG_COUNTPAGE		0x00000020
	//设置此标志位表示在这个表中除了主键外最少有一个列是有索引的(在此标记是为了增加速度)
#define BS_TDTF_FLAG_INDEXFIELD		0x00000040
	//设置此标志位表示这是个主键类型的表，此时将会采取主键加索引去确定行的位置而不用链表形式
#define BS_TDTF_FLAG_PRIMARYTABLE	0x00000080
	//设置此标志位表示表中最少有一个字段为VDT_STR或者VDT_BIN类型数据(为了加快插入速度)
#define BS_TDTF_FLAG_VARFIELD		0x00000100
	//指明此主键索引为降序模式否则为升序
#define BS_TDTF_FLAG_PMKI_DESCORDER 0x00000200
	//设置此标志位表明这是个DYN表
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
//此标记为真时表明按创建时间过滤
#define BS_FIELD_COND_USE_CTIME 0x02
#define BS_FIELD_COND_USE_DELETEALL		0x04

////////////////////////////////////////////////////

//////////////delete table type//////////////////
//快速删除一个表，这种情况不会释放空间
#define TBDB_DT_QUICK				0x01
//完整删除一个表，会释放表所占的空间
#define TBDB_DT_RS					0x02
////////////////////////////////////////////////

////////////////////内存中记录集的前缀////////////////////
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

/////////////内存记录集中每条记录的前缀//////////////
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

///////////////////内存中表字段结构/////////////////////
struct bs_tdfield
{
#define BS_TD_MAXFIELDNAME		32
	i8 name[BS_TD_MAXFIELDNAME];
	i8 uType;
	//设置此标志则对列创建索引
#define BS_TDFF_CREATEINDEX			0x0001
	//设置此标志则表示此列值不能有重复的
#define BS_TDFF_UNIQUE				0x0002
	//设置此标志表示此字段为主键
#define BS_TDFF_PRIMARYKEY			0x0004
	//指未此字段为自增id自段,此时数据类型必须为整数型
#define BS_TDFF_AUTOINCREASE		0x0008
	//设置此标志指示此列所建的索引为降序，否则为升序
#define BS_TDFF_INDEX_DESC			0x0020
	ui16 uFlag;
	//如果是固定长度记录模式，则此值代表非固定长度的数据类型的限定长度，如字符串和二进制数据等
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