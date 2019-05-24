#ifndef _BS_TREEDBDEF_H_
#define _BS_TREEDBDEF_H_

/////OpenKey flag////////////
#define TRDB_OPKF_CREATEMAINKEY			0x00010000
#define TRDB_OPKF_DELETEMAINKEY			0x00020000
#define TRDB_OPKF_RENAME				0x00040000
#define TRDB_OPKF_OPENEXIST				0x00080000

#define TRDB_OPKF_READONLY				0x00000001
#define TRDB_OPKF_SYSTEMKEY				0x00000002
#define TRDB_OPKF_HIDE					0x00000004

#define TRDB_OPKF_OPENCHILD				0x00000010
#define TRDB_OPKF_DOTPATH				0x00000080
////////////////////////////////////////////////////////

#define BS_TRDB_DOTSEPARATOR	"."
#define BS_TRDB_NODESEPARATOR	"\\"


#define BS_TRDB_MAXFILENAME	30
#define BS_TRDB_MAXSUBKEYPATH 2048

#define BS_TRDB_MAXMAINKYELEN	38
#define BS_TRDB_MAXNODENAELEN	800
#define BS_TRDB_MAXRECORDNAMELEN	800

#define BS_TRDB_MAXPWD		30


///////////////////////////////////////////////////////////////

	//TreeDB���ݿ��һЩ��־λ
	//ָʾʹ��˫����ʽ�Ӽ�
#define BS_TRDB_FLAG_LINKNODE			0x00000001
	//ָʾʹ�������Ӽ�
#define BS_TRDB_FLAG_INDEXNODE			0x00000002
	//ָδʹ��˫����ʽ����
#define BS_TRDB_FLAG_LINKRECORD			0x00000004
	//ָʾʹ������ʽ����
#define BS_TRDB_FLAG_INDEXRECORD		0x00000008
	//ָʾʹ��˫����ʽ����
#define BS_TRDB_FLAG_LINKMAINNODE		0x00000020
	//ָʾʹ������ʽ����
#define BS_TRDB_FLAG_INDEXMAINNODE		0x00000040
////////////////////////////////////////////////





#endif