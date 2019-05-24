#pragma  once
#include <string>
using namespace std;

#define MX_MAX_INDEX		0x0fffffff



#define STINGTABLEKEY		"StringTable"
#define SERVERADDR			"127.0.0.1"
#define NNMCMDMQ			"nnm_cmd_mq"

#define MXSESERVER			"Svr"
#define MXSENETWORK			"Net"
#define MXSEAPP				"App"

#define	CLIENTMQ			"BMPClient1_"
#define TOPOBACK			"Topo-Backup"

#define SUBGROUP			".SG"
#define SUBDEVICE			".SD"
#define ROOTID				"1"
#define GISKEY				"GIS"
#define CCUBASE             "ccubase"
//Column Label of MXDB
//Event Log
static const char CREATETIME[]=			"CreatTime";
static const char MONITORID[]=			"MonitorId";
static const char DEVICEID[]=			"DeviceId";
static const char SOURCE[]=				"Source";
static const char EVENTLEVEL[]=			"EventLevel";
static const char EVENTCLASS[]=			"EventType";
static const char REMARK[]=				"Remark";
static const char MONITORSTATUS[]=		"MonitorStat";
static const char LASTMONITORSTAT[]=	"LastMonitorStat";

//Alert Report
static const char ALERTID[]=			"AlertId";
static const char SENDTIME[]=			"SendTime";
static const char SENDTYPE[]=			"SendType";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SE Group || SE || Group(Sub Group) || Device || Monitor
enum CriticalLevel {NORMALLEVEL=1,CRITICALLEVEL=7};
enum VERSIONTYPE {TRAILTYPE=0,FULLTRAILTYPE=1};
enum ObjType{MONITORTYPE = 1,DEVICETYPE = 2,GROUPTYPE = 3,TSTYPE=4,CCUTYPE=5, COMPANYTYPE=6,MASTERTYPE=7,  NETCFGTYPE,NETMGRTYPE};
enum FINDTIME{CURDAY=0,THREEDAY=1,ONEWEEK=2,ONEMONTH=3,SELFTIME=4 };
enum INCLUDETYPE{INCLUDEDEVICE=0,INCLUDEVIRSUAL };
enum DEVICEPROPERTYTYPE{DEVICESINGLEPROPERTY=0,DEVICELISTPROPERTY=1,DEVICEFIXPROPERTY=2};
enum CRECLOUD_SERVER_VERSION {SINGLECCUSERVER=1,CLOUDCCU=6,SAASCCU=3,FOURCCU=4 , SAASCONTAINER=5, TSSERVER=2,FOURCONTANER=7,NETGAPAGENT=8 ,MINISERVER=9 ,NETDEVTRIAL=10, DBSERVER=11};
enum FourLevelRunType{FourLevelRunType_NO=0,FourLevelRunType_CONTANER=1,FourLevelRunType_SERVER=2};
//Time Mode

#define TIMEMODE_ONEDAY		0
#define TIMEMODE_THREEDAYS	1
#define TIMEMODE_ONEWEEK	2
#define TIMEMODE_ONEMONTH	3

//enum RADIOCAL{GOODRADIO=-10,ERRORRADIO=-11};

enum TOPOSTATE{REAL = 1,HISTORY,DIFF};           //肖军---用于右键显示线路的是哪种状态的拓补
enum REPROTTYPE{ DAYREPORT=0,WEEKREPORT=1,MONTHREPORT=2};
const int AlAWYSMONITOR = 0x03;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// monitor state
// 无数据 || 正常 || 警告状态 || 错误 || 禁止 || 严重错误的
enum MONITORSTAT{GOODSTAT=1,WARNINGSTAT=2,ERRORSTAT=3,BADSTAT=4,DISABLESTAT=5,NODATASTAT=0  ,ALLSTAT =6 };
enum SHOWSTAT{UNKNOWNSTATUS = 0, GOODSTATUS = 1, WARNINGSTATUS = 2, ERRORSTATUS = 3,  DISABLESTATUS = 5, ALLSTATUS = 6};
enum WMISCRIPT{SENDMSG=0 ,RESTATMACHINE=1,RESTARTSERVICE=2};
enum CCUSTATE{NEVERCONNECT=0,DISCONNECT,ALIVE};
//--------------树形菜单----------------------------------------------
enum MenuType{DATATYPE=0, ALERTROOTTYPE=1,ALERTCONFTYPE=2,ALERTREPORTTYPE=3,  
			ALERTTYPE=4  ,REPORTYPE=5 , TOPNTYPE=6, REALTIMETYPE=7,REALTIMETPLTYPE=8,
			STATISTICTYPE=9 ,ANALYSISREPORT=10,ANALYSISTPLREPORT=11,HANDTAILORREPORT,
			CURRENTTRAFFIC,TRAFFICHISTORY,TRAFFICHISTORYTPL ,TOPNTPLTYPE,DEVICEREPORT,
			REALPARAMTYPE,REALMONITORTYPE ,REALMONITORTYPETPL,AVGPARAMREPORT,AVGPARAMREPORTTPL,
			ALLDEVICESTATREPROT,ALLMONITORSTATREPORT,TOOLS,PINGTOOL,TELNETTOOL,REMOTETOOL,FARCONTROL,
			TRACEROUTTOOL,HTTPTOOL,VISIOTOPOLOGY,VISIOTOPOLOGYMAP,MONITORSTATIC,VIRSUALGROUP,
			VIRSUALDEVICE,VIRSUALMONITOR,EVENTLOGTYPE,FAULTCATEGORY,FAULTREPORT,FAULTREPORTTPL
			,URLREPORT,ALLVGROUPSTATREPROT,IISREPORT, TOPOLOGY,SUBNETS,SUBNETGROUP, SUBNET,NETWORKSET,NET3WORK,NET2WORK
			,OTHERNETWORK, TOPOLOGYMAPS,MAINTOPO,TOPOLOGYMAP ,MACHINES,SNMPMACHINES,IPSETS,DEVICEEGISINFO,
			DEVICEEGISMAN,CNPCREPORT,OPERATELOGTYPE,HANDETAILOR,DYNVIEW,ALLGROUPDASHBORAD,TOPGROUPDASHBORAD,GROUPDASHBORAD
			,DAYTRAFFIC,DAYTRAFFICPL,LOGICMAP,LOGICGROUP};

enum SNMPDEVICETYPE {HUB=0,HOST=1,SWITCH2=2,SWITCH3=3,ROUTER=4, FIREWALL=5,OTHERSNMP=9,SUBTOPO=10};
enum TOPODATA {TOPOBOX=0, TOPOLINE=1};

#define TOPOMAIN	"maintopo"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 操作

#define MB_YES             0x0000000AL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 权限定义
enum MX_Purview
{
    Add_Group       = 0x2,          // 增加组
    Add_Device      = 0x3,          // 增加管理对象
    Add_Monitor     = 0x4,          // 增加监测点
    Edit_SVSE       = 0x10,         // 编辑SE
    Edit_Group      = 0x20,         // 编辑组
    Edit_Device     = 0x30,         // 编辑管理对象
    Edit_Monitor    = 0x40,         // 编辑监测点
    Delete_Group    = 0x200,        // 删除组
    Delete_Device   = 0x300,        // 删除管理对象
    Delete_Monitor  = 0x400,        // 删除监测点
    Refresh_Device  = 0x3000,       // 刷新管理对象
    Refresh_Monitor = 0x4000,       // 刷新监测点
    Test_Device     = 0x30000,      // 测试管理对象
};
static const char MX_ERRORSTOP[]					= "_monitorcondition";
#define ERRORSTOP			1
#define ERRORSTOPCONFIRM	2
#define MAXTABLENAMELENGTH		200
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 脚本报警
static const char MX_SENDMSG[]=					"发送消息";
static const char MX_RESTATMACHINE[]=           "重新启动机器";//
static const char MX_RESTARTSERVICE[]=			"重新启动服务";//
//－－－－－－－－－－－－－－－－－－－－－－－－－－－－－


static const char BM_REFRESH_QUEUE[]            = "BMP_RefreshInfo_%s";  // 刷新队列头
//static const char BM_REFRESH_QUEUE[]            = "MXBMP-ConfigTrack_%s";  // 刷新队列头

static const char BM_CONFIG_TRACK[]             = "MXBMP-ConfigTrack_%s";  // 配置更改队列头
static const char BM_USER_OPERATE_LOG_TABLE[]   = "UserOperateLog";             // 操作日志

// 数据类型





static const char mxHealth[]                      = "mxHealth";   
static const char mxName[]                      = "mxname";                    // 名称
static const char mxRetryTimes[]                      = "mxretrytimes";                    // 名称


static const char mxRegistTime[]				="registtime";
static const char mxCCUAddr[]				="ccuaddr";
static const char mxClientGuid[]					= "mxClientGuid";
static const char mxGuid[]					= "mxGuid";
static const char mxIndex[]                 = "mxindex";                   // 显示索引
static const char mxMaxIndex[]              = "mxmaxindex";                //最大索引
static const char mxDescription[]               = "mxdescription";             // 描述

static const char mxParent[]                   = "mxparent";                 // 是否禁止（true, false, time）
static const char mxDisable[]                   = "mxdisable";                 // 是否禁止（true, false, time）

static const char mxConditionCount[]            = "mx_conditioncount";          // 报警条件匹配式总数
static const char mxParamValue[]                = "mx_paramvalue%d";            // 参数值
static const char mxParamName[]                 = "mx_paramname%d";             // 参数名
static const char mxOperate[]                   = "mx_operate%d";               // 操作
static const char mxRelation[]                  = "mx_relation%d";              // 连接符

static const char mxExpression[]                = "mx_expression";              // 

static const char mxPlan[]                      = "mx_plan";                    // 任务计划
static const char mxCheckErr[]                  = "mx_checkerr";                // 校验错误
static const char mxErrFreqSave[]               = "mx_errfreqsave";             // 错误时监测频率存储
static const char mxErrFreq[]                   = "mx_errfreq";                 // 错误时监测频率
static const char mxErrFreqUnit[]               = "mx_errfrequnit";             // 错误监测频率单位
static const char mxErrFreqSec[]               = "mx_errfreqsec";             // 错误时监测频率存储
static const char mxDay[]								= "dayctrl" ;            // 属性文本框
static const char mxDayTime[]                           = "daytimectrl";
static const char mxTime[]                              = "timectrl";
static const char mxGroupBox[]                           = "groupbox";







/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char LeftParenthesis                 = '[';                          // 左括弧
static const char RightParenthesis               = ']';                          // 右括弧



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int  mxMaxBufferSize                  = 1024 * 50;                    // 最大缓冲区大小





//-------------------snmp-------------------------------------------------------------------
static const char mxSysDescr[]                = "sysDescr";              // 报告描述
static const char mxSysContact[]              = "sysContact";                    // 任务计划
static const char mxSysName[]                 = "sysName";                // 校验错误
static const char mxSysObjectID[]                 = "sysObjectID";                // 校验错误

static const char mxSysLocation[]             = "sysLocation";              // 报告描述
static const char mxSysService[]              = "mx_SysService";                // 校验错误
static const char mxipForwarding[]            = "mx_ipForwarding";              // 报告描述
static const char mxbRouteTable[]             = "mx_bRouteTable";                    // 任务计划
static const char mxbFdbTable[]               = "mx_bFdbTable";                // 校验错误
static const char mxbSupportBulk[]            = "mx_bSupportBulk";                // 校验错误
static const char mxVendor[]			      = "mx_Vendor";                // 设备厂商
static const char mxModel[]					  = "mx_Model";                // 校验错误
static const char mxSnmpDeviceType[]            = "_snmpdevicetype";                // 校验错误
static const char mxNetConfig[]            = "mx_NetConfig";                // 网络配置设备
static const char mxSyslog[]					= "mxsyslog";						//Syslog识别
static const char mxSyslogRule[]							="_syslogrule";
static const char mxSyslogRuleLabel[]			="_syslogrule_Label";
static const char SyslogPassive[]				="SyslogPassive";
static const char mxTranspondMessage[]			="mxTranspondMessage";
static const char Syslog[]						="Syslog";
static const char SnmpTrap[]					="SnmpTrap";
static const char mxMac[]			            = "mx_mac";                // 校验错误
static const char mxSpeed[]						= "mx_Speed";                // 校验错误
//static const char mxMSpeed[]						= "mxMSpeed";                // 校验错误
static const char mxIfType[]					= "mx_iftype";                // 校验错误
static const char mxOpstat[]		            = "mx_opstat";                // 校验错误
static const char mxAdstat[]		            = "mx_ifadminstat";                // 接口管理状态
static const char mxSort[]		            = "mxsort";                // 校验错误

//snmptrap add
static const char mxSnmpTrap[]					="mxsnmptrap";						//SnmpTrap识别
static const char mxSnmpTrapRule[]				="_snmptraprule";
static const char mxSnmpTrapRuleLabel[]			="_snmptraprule_Label";
static const char SnmpTrapPassive[]				="SnmpTrapPassive";
//snmptrap end

//-------------------------------------------------------------------------------
static const char mxSortType[]		            = "mxsorttype";                // 校验错误
static const char mxDelete[]					= "mx_delete";
static const char mxPropName[]					= "mxpropname";
static const char mxTempProp[]					= "mx_istemp";
static const char mxPropType[]					= "mxproptype";
static const char mxBaseProp[]					= "BaseInfo";
static const char mxAutoProp[]					= "Auto";
static const char mxIsInR[]						= "mxisInR";

static const char mxHub[]						= "Hub";
static const char mxSubTopo[]					= "subtopo.";
static const char mxTopo[]						= "Topo.";
static const char mxRelatePanel[]					= "mx_relatepanel";
static const char mxDataUpdateTime[]					= "mx_updatedatatime";
static const char mxSaasCCUCanRun[]             ="mx_saasccucanrun";
static const char mxSaasCCUCanRunTimeAfter[]             ="mx_saasccucanruntimeafter";
//------------------------------------------------------------------------------------
static const char mxStartTime[]					="mx_starttime";
static const char mxEndTime[]					="mx_endtime";
#define   MX_STAT							"mx_stat"

#define VERTICALSPACE   7
#define WINDOWHEIGHT    24
#define LABELWIDTH      110
#define HORIZONSPACE    20
#define WINDOWWIDTH     400
#define HALFWINDOWWIDTH     200     //为了 跟随,所以减半


#define ERROR_NONULL  1
#define ERROR_ISNUM   2



//--------------------------系统设置------------------------------------------------------------
#define RANGETYPE 2
#define ABSTYPE   1
#define SMSTPL  1
#define EMAILTPL 2
#define SMSTPLSec "SmsTpl"
#define EMAILTPLSec "EmailTpl"
#define TRAPTPLSec "TrapTpl"

#define WEEKDAY   7
#define DOUBLEHOUR  48

#define SELFID			 5000
#define DEVICEDYNID		10000
#define MONITORDYNID	15000
//----------------------mainfrm----------------------------------------
enum WINDOWOP{CLOSEWIN =0, UPDATETITLE=1  };
enum OBJOP {ADDOP=0,UPDATEOP=1,DELETEOP=2,UPDATESTAT=3};
enum   EventLevel { EVENTNOTICE=0,EVENTWARNING=1,EVENTEMERGENCE=2,EVENTERROR=3};
enum   OtherEventType {IPMACEVENT =0};

#define COLOR_MASK	RGB(0x00,0x80,0x80)
enum   TOPORELEATION{ SELRELEATION =0, IPRELEATION=1 };
enum  TOPODEVICETYPE{ROUTERTYPE=0, SWITCHTYPE =1 ,SERVERTYPE =2 };
 // SELRELEATION  系统已有id里面选择
// IPADDRRELEATION ip地址关联

enum TOPOOPERATOR {TOPOSELECT =0, ADDTOPOLINE =1,ZOOMMAX=2, ZOOMMIN=3,NORMALSIZE=4 };

enum PROPERTYTYPE {PROTOCOL = -1,SINGLEOJB=0,MULITOBJ=1,MONITOREX=2};

/*
#define COLOR_BLACK         RGB(0, 0, 0)
#define COLOR_WHITE         RGB(255, 255, 255)
#define COLOR_BLUE          RGB(0, 0, 255)
#define COLOR_YELLOW        RGB(255, 255, 0)
#define COLOR_RED           RGB(255, 0, 0)
#define COLOR_LIGHTCORAL    RGB(240, 128, 128)
#define COLOR_GREEN         RGB(0, 255,0 )
*/

#define SYSLOG		"SysLog"
#define ALERTREPORT "AlertReport"
#define EVENTTABLE  "EventLog"
#define OPERATETABLE "OperateLog"
#define  ASSERTTRUE(X) if(!X) {  return false;}
#define  NEWASSERTTRUE(X) if(!X) { CloseMXCTHandle(hd); return false;}


//-----------------ALERT DEFINE	---------------------------------------
#define INVALID_NUM          -1

enum AlertAction{EMAILALERT=0, SMSALERT = 1,SOUNDALERT= 2,SCRIPTALERT = 3,SNMPTRAPALERT=4,LEDALERT=5,MMSGALERT=6,APPALERT=7,WORKORDERALERT=8};
enum AlertCondition{  SEQUENCE=0,ONCE=1,JUMP=2,AllStat=3,ELECTT=4};
enum TplCondition {ALERTALL =0,ALERTINCLUD=1,ALERTEXCLUD=2};

enum CellShowType{CELLTEXTTYPE =0,CELLSCALETYPE=1,CELLSTATTYPE=2 ,CELLCOLUMN=3};
enum ValueType{TEXTTYPE =0,INTETYPE=1,FLOATTYPE=2};

enum TOPCALTYPE {CALAVG=1,CALLAST=0};
enum SORTORDER{SORTASC=1,SORTDECS=0 };
enum SORTTYPE{NOSORT=0 ,SORTBYNAME=1,SORTBYTYPE=2,SORTDEFAULT=3,SORTBYSELF=4 };

enum DRAWTYPE{PIE=0,LINE=1,BAR=2,GAUGES=3,AREAR};
enum AVGCALTYPE {SIMPLEAVG=0,MODAVG };

enum AVGUNIT {FIVEMIN=0,TENMIN,HALFHOUR,HOUR,DAY,WEEK,MONTH };
#define HTTPSTPL		"SSL"
#define MASSURLTPL		"MassURL"
#define MASSURLEXTPL	"MassUrlEx"
#define GSMMQ "gsmmq"
#define GSMRETMQ "gsmretmq"

//--------------------------------NNM---------------------------------
enum SNMPTPVERB {IDLEVER=0, //休闲状态
				STOPSEARCH=1, //停止搜索， 双向发送 搜索设备完成或被停止，搜索线路被中止
				ADDSUBNET=2,  //增加子网  服务器向客户发送
				STARTSEACHDEVIE=3, //开始搜索设备 客户向服务器发送
				STARTSEARCHTP=4,   //开始搜索拓扑 客户向服务器发送
				GETDEVICEPROP=5,   //得到设备属性 获取设备属性时发送
				ADDSNMPDEVICE=6,   //增加设备   双向发送 
				DELETESNMPDEVICE=7, //删除设备  客户向服务器发送
				EDITSNMPDEVICE=8,   //编辑设备 客户向服务器发送
				GETTPSTAT=9,		//得到当前搜索设备状态
				DELETEALLSUBNET=10, // 删除所有子网 客户向服务器发送 ？？？
				DELETESUBNET=11,	//删除子网
				GETSTEP=12,			//得到搜索设备的进展情况
				ResetDeviceThreadMax=13,//重新设置搜索设备的线程数
				CurrentSearchStat=14,//通知客户端当前搜素情况  服务器向客户发送
				FINISHTOPOLOGY=15,//完成链路分析
				GETDEVICEIFSTAT=16,
				SETDEVICEIFCLOSE=17,
				SETDEVICEIFOPEN=18,
				GETAFTDATA=19,
				GETARPDATA=20,
				REFRESHCOMMDATA=21,
				DELETESNMPGROUP=22,
				GETIFDATA=23,
				GETIFTWOSTATDATA=24,
				GETDEVICESPROP=25,
				TESTSOUNDALERT=26,
				GETCDPDATA=27,
				GETARPFTPDATA=28,
				GETTRAFFICDATA=29,
				GETLLDPDATA=30
				};

enum SNMPTPSTAT {SEACHIDLE=0, SEARCHDEVICES=1,SEARCHTOPO=2};
#define PROPERTYGETOK	"OK"
//Add New
//--------------------------维护人信息-----------------------------------------------
static const char mxGroup[]         ="_Group";
static const char mxComm[]          = "_Comm"; 
static const char mxProperty[]          = "Property"; 
static const char mxSelfGetPro[]          = "SelfGetProperty"; 

static const char mxContactInfo[]          = "ContactInfo";           // 联系人信息
static const char mxContactName[]		   = "contactName";		      // 姓名
static const char mxTelephony[]			   = "telephony";		      // 电话
static const char mxDuty[]				   = "duty";				  // 职责
static const char mxUnit[]				   = "unit";				  // 单位

static const char mxMaintenanceInfo[]	   = "MaintenanceInfo";		  // 维保信息
static const char mxTimeLimit[]			   = "timeLimit";			  // 维保期限
static const char mxBeginDate[]			   = "begin_date";			  // 开始日期
static const char mxEndDate[]			   = "end_date";				  // 结束日期
static const char mxManuFacturer[]		   = "manufacturer";		  // 维保厂商

//Add End
enum MAINVERB 
{
	STATCHANGE=1201,
	UPDATEDIPMAC=1202,
	ALERTSENDMSG=1203,
	CLIENTUPDATE,
	CLIENTSYNCHRONIZATION,

};

enum CLIENTUPDATE_type
{
	CLIENTUPDATE_ADD = 1,
	CLIENTUPDATE_MODIFY,
	CLIENTUPDATE_DELETE,
	CLIENTUPDATE_UNKNOWN,
};

enum TPLTYPE {XMLTEMPLATE=0, ODBCTEMPLATE=1}; //生成配置模板
enum MACEVENTTYPE {MACCHANGED=803 ,MACNEW=804,MACWRONGIP=805};
enum OperateType {				EDITSE=2,							//根节点
					ADDGROUP=11,EDITGROUP=12,DELETEGROUP=13, FORBITGROUP = 14, ALLOWGROUP = 15,		//组
					ADDDEVICE=21,EDITDEVICE=22,DELETEDEVICE=23, FORBITDEVICE= 24, ALLOWDEIVCE = 25,TEMPORARYFORBITDEVICE=26,//设备
					ADDMONITOR=31,EDITMONITOR=32,DELETEMONITOR=33, FORBITMONITOR = 34, ALLOWMONITOR = 35,TEMPORARYFORBITMONITOR=36,//监测点
					ADDUSER=41,EDITUSER=42,DELETEUSER=43,			//用户
					ADDVRGROUP=51,EDITVRGROUP=52,DELETEVRGROUP=53,	//虚拟组
					ADDREALREPORT=61,EDITREALREPORT=62,DELETEREALREPORT=63,			//实时报告
					ADDTOPNREPORT=71,EDITTOPNREPORT=72,DELETETOPNREPORT=73,			//TopN报告
					ADDAVGREPORT=81,EDITAVGREPORT=82,DELETEAVGREPORT=83,			//统计数据报告(平均报告)
					ADDANYSREPORT=91,EDITANYSREPORT=92,DELETEANYSREPORT=93,			//统计原始报告(分析报告)
					ADDFAULTKIND=101,EDITFAULTKIND=102,DELETEFAULTKIND=103,			//故障分类
					ADDFAULTREPORT=111,EDITFAULTREPORT=112,DELETEFAULTREPORT=113,	//故障统计报告
					ADDALERT=121,EDITALERT=122,DELETEALERT=123,		//报警
					USERLOGIN=131,USEREXIT=132,EDITDAYTRFFICPORT=133,ADDDAYTRAFFICMONITOR=134,
					DELDAYTRIFFICREPORT=135,
                 ADDROOTNAME=140};//配置管理



//-----------------------------Return----------------------------------
static const  char mxDataType[]	=				"mxdatatype";
static const  char mxDataUnit[]	=				"mxunit";
static const  char mxVar[] =					"mxvar";
static const  char mxDataLabel[]	=			"mxlabel";
static const  char mxRow[]	=					"mxrow";
static const  char mxCol[]	=					"mxcol";
static const  char mxIcon[]	=					"mxIcon";
static const  char mxTplIcon[] =				"mxTplIcon";
static const  char mxMainShow[]=                "isMainShow";

//Topo标准监测类型
static const  char mxNetworkInterface[]	=			"networkinterface";
static const  char mxSFInterface[]	=	"SFIntf";
static const char mxNetWork[]							=	"NetWork";
static const char mxAgentLinuxNetworkCard[]	= "AgentLinuxNetworkCard";
static const  char mxRecvTraffic[]	=			"recivetraffic";
static const  char mxSendTraffic[]	=			"sendtraffic";
static const char mxTargetIp[]					= "_target_ip";
static const char mxAgentMonitorType[]					= "_monitor_type";
static const char mxDevName[]					= "dev_name";
static const char mxDevIp[]					= "dev_ip";
static const char mxPointName[]					= "point_name";
//-------------------Report--------------------------------------------------
#define		MX_GOOD_RADIO						"监测正常率(%)"
#define		MX_ERROR_RADIO						"监测故障率(%)"
#define GOODRADIO    "GoodRadio"
#define ERRORRADIO    "ErrorRadio"

//----------------------Monitor Device Group--------------------------------------------------------
static const  char mxLabel[]	=					"mxlabel";
static const  char mxEntityMastGroupIcon[]		=	"mxIcon";
static const  char mxEntityMastGroupIconActive[]=	"mxIconActive";
static const  char mxControlType[]	=				"mxcontroltype";
static const  char mxIsMust[]		=				 "mxismust";
static const  char mxDefault[]		=				 "mxdefault";
static const  char mxOnlyNum[]		=				 "mxisonlynum";
static const  char mxHelp[]		=					"mxhelp";
static const  char mxRows[]		=					"mxrows";
static const  char mxIsFollow[]		=				 "mxisfollow";
static const  char mxFllowName[]		=				 "mxfollowname";
static const  char mxIsInputDll[]		=				 "mxisinputdll";
static const  char mxIsDyn[]			=				 "mxisdyn";
static const  char mxDllFunc[]			=					"mxdllfun";
static const  char mxDll[]				=					"mxdll";
static const  char mxIsMulti[]			=					"mxismutli";
static const  char mxIsHide[]			=						"mxishide";
static const  char mxIsReadOnly[]			=					"mxisreadonly";
static const  char mxIsSort[]			=						"mxissort";
static const char mxSaveInMain[]	=						"mxsaveinmain";
static const char mxParamdContent[]	=						"mxparam%dcontent";
static const char mxParamdName[]	=						"mxparam%dname";
static const char mxParamdIcon[]	=						"mxparam%dicon";
static const char mxParamdNum[]	=						"mxparamnum";
static const char mxHasParent[]	=						"mxhasparent";
static const char mxDistanceTime[]				= "_distancetime";             // 监测点类型
static const char mxDistanceTimeUnit[]                = "_distancetime_unit";             // 监测点类型
static const char mxPluginName[]=							"mxpluginname";
static const char mxSubPlugin[]=							"mxsubplug";
static const char mxRemark[]						= "mxRemark";
static const char mxKey[]						= "mx_key";						// 关键Key
// controltype
static const char mxTextCtrl[]	=						"textbox";
static const char mxPwdTextCtrl[]	=					"passwordtextbox";
static const char mxComboCtrl[]	=						"combobox";
static const char mxMonitorType[]=						"monitorTypebox";
static const char mxDropCtrl[]	=						"dropbox";
static const char mxCheckCtrl[]	=						"checkbox";
static const char mxRadiotCtrl[]	=						"radio";
static const char mxlistCtrl[]	=						"listbox";
static const char mxTextboxProperty[]					= "textboxproperty" ;            // 属性文本框
static const char mxLink[]					=			"mxlink" ;            // 链接

//static const char mxDay[]								= "dayctrl" ;            // 属性文本框

static const char mxThresholdRemark[]							="mxthresholdremark";
static const char mxFaciLevel[]							= "_important_level";
static const char mxDependCondition[]				   = "mxdependscondition";        // 依靠条件
static const char mxDependON[]							 = "mxdependson";               // 依靠于
static const char mxDeviceType[]						= "mxdevicetype";              // 管理对象类型
static const char mxType[]								= "mxtype";                    // 数据类型
static const char mxSelfSign[]								= "mxselfsign";                    // 数据类型
static const char mxIden[]								= "mxiden";                    // 数据类型
static const char mxStyle[]								= "mxstyle";                    // 数据类型
static const char mxMonitorBox[]								= "monitorbox";                    // 数据
static const char mxFixServer[]								= "mx_fixtsserver";                    // 数据类型
static const char mxBackFixServer[]								= "mx_backfixtsserver";                    // 数据类型
static const char mxLogServer[]								= "mx_logdbserver";                    // 数据类型
static const char mxCanbeOhterServerMonitor[]					= "mx_canbeloadwhilenofixserver";
static const char mxImportantLevel[]					= "_important_level";			//重要程度

static const char mxIconBase[]							= "mxIcon";						//ICON BASE
static const char mxIsTplDev[]							= "mxIsTplDev";					//是否是设备模板
static const char mxGroup_[]								= "Group";						//组类型
static const char mxFolder[]							= "FOLDER";						//文件夹
static const char mxGroupOpen[]							= "GroupOpen";					//打开组类型
static const char mxFolderOpen[]						= "FOLDER_O";					//打开的文件夹
static const char mxMonitor[]							= "Monitor";					//监测点类型
static const char mxDot[]								= "DOT";						//监测点图标
static const char mxCriticalMonitor[]					= "CriticalMonitor";			//重要监测点
static const char mxSun[]								= "SUN";						//重要监测点图标
static const char mxPassiveMonitor[]					= "PassiveMonitor";				//被动式监测点
static const char mxRhombus[]							= "RHOMBUS";					//被动式监测蒂娜图标
static const char mxSwitch2[]							= "Switch";						//二层交换机
static const char mxSwitch3[]							= "SwtichRouter";				//三层交换机
static const char mxSwitch[]							= "SWITCH";						//二层交换机图标
static const char mxSwitchRouter[]						= "SWITCHROUTER";				//三层交换机图标
static const char mxRouter[]							= "Router";						//路由器
static const char mxRouterIcon[]						= "ROUTER";						//路由器图标
static const char mxCommon[]							= "COMMON";						//通用设备
static const char mxUnKnown[]							= "COMMON";						//未指定设备图标

static const char IconBaseKey[]							= "IconBase";		
static const char mxVersion[]                           ="mxversion";
static const char mxNameRule[]                          ="mx_namerule";
static const char mxPluginType[]                        ="mx_plugintype";
static const char mxDataType_bin[]                      ="binary";
static const char mxDataType_table[]                    ="table";
static const char mxPwd[]                               ="_password";
//------------------------------------------------------------------------------------

enum MONITORWAY{
	MXCOMMONMONITOR = 1,					//普通监测点
	MXPASSVICEMONITOR,						//被动式监测点
	MXCUSTOMMONITOR,						//自定义监测点
};

enum ExportTable {	RESOURCE=0,				//中文资源表
					MONITORTEMPLATE=1,		//监测点模板表
					MONITORCONFIG=2,		//监测类型表
					DEVICETYPETABLE=3,		//设备类型表
					GROUPTABLE=4,			//组表
					DEVICETABLE=5,			//设备表
					MONITORTABLE=6,			//监测点表
					DEVICEMINITOR=7,		//设备监测点表
					GROUPDEVICE=8,			//组和设备关系表
					EVENTTYPE=9,			//事件级别表
					CONTROLTABLE=10,		//管理单元表
					CONTROLGROUP=11,		//管理单元组关系表
					CONTROLDEVICE=12,       //管理单元设备关系表
					RESULTTABLE=13,			//监测结果表
					EVENTLOG=14,			//事件表
					ALERTLOG=15				//报警表
};		
enum MXVERSION {SINGLEVERSION=1,FENBUVERSION=2};
static const char IPF_FIRSTLINE_FLAG[]="<$MXCC_InputBegin$>";
static const char IPF_LASTLINE_FLAG[]="<$MXCC_InputEnd$>";
static const char IPF_TASKID_FLAG[]="ID:";
static const char IPF_STATUS[]="Status:";
static const char IPF_DATALINE_FLAG[]="<$data_line$>";
static const char IPF_DATAUNIT_FLAG[]="<$Unit$>";
static const char IPF_DATAINT32TYPE_FLAG[]="<$Int$>";
static const char IPF_DATAINT64TYPE_FLAG[]="<$Int64$>";
static const char IPF_DATAFLOATTYPE_FLAG[]="<$Float$>";
static const char IPF_DATASTRINGTYPE_FLAG[]="<$string$>";			//default
static const char IPF_DATAPERCENTTYPE_FLAG[]="<$Percent$>";
static const char IPF_DATATIMETYPE_FLAG[]="<$Time$>";

static const char PLUGINDATA_RETURNFLAG[]="data";

//Plugin分类
static string arrayCpu[]={"UnixCPU","netscreenCPU","PixCPU","SnmpCPUInfo","CiscoCPU","CPU","huaweiCPU","NetSnmpCPUInfo","ZhongXingCPU",
"NetWorkCPU","AgentLinuxCPU","radwareCpu","SnmpH3cCpu","TOS3_cpu","TOSVI3_cpu","BarrCPUTemp","LenovoCpu","VMwareEsxHostCPU",
"VMwareEsxHostCpuCore","HyperVCPU","HyperVOsCPU","dellOMCPU","CheckPointCPU","F5Cpu","NetAppCpu","AVAYAPBXCpu",
"Venus_cpu","radwareCpuUtilization","LenovoCPUInfo","Venus_NIPS860_CPU","Venus210_cpu","VenusCA500S_CPU","WMICPU"};

static string arrayDisk[]={"UnixDisk","SnmpDiskInfo","AgentDisk","WMIDiskInfo","SnmpDiskIOInfo","AgentLinuxDisk","VMwareEsxHostDisk","VMwareEsxHostDiskIndependent",
"DiskInfo","HyperVOsDiskInfo","DominoLotusDisk","CheckPointDisk","F5Disk","NetAppDiskInfo","emc_DiskInfo","LenovoDiskInfo","VenusCA500S_Disk"};

static string arrayMemory[]={"MemoryInfo","UnixMemory","PixMemory","SnmpMemoryInfo","AgentWinMemory","ExchangeMemory",
"huaweiMemory","SQLMemoryStatus","ZhongXingMemory","arraySysMemory","SnmpH3cMemory","TOS3_memory"
"TOSVI3_memory","Squid_Memory_Usage","VMwareEsxHostMemory","HyperVOsMemoryInfo","dellOMMemory","JBoss7MemoryInfo",
"DominoLotusMemory","JBossMemoryPool","MsNetMemoryC","F5Memory","Venus_memory","radwareDynamicMemory","NetWorkMemory",
"radwareMPMemory","LenovoMemory","Venus_NIPS860_Memory","Venus210_memory","VenusCA500S_Memory"};

static string arrayPhysical[]={"AgentLinuxMem","PhysicalMemory","UnixPhysicalMemory","WmiPhysicalMemory","HyperVOsPhysicalMemory","SNMPphysicalmemoryInfo"};

static string arrayInterface[]={"NetWork","networkinterface","SFIntf"};

static string arrayCpuTop5[]={"UnixCpuTop5","CpuTop5"};

static string arrayPhysicalTop5[]={"UnixMemTop5","PhysicalMemTopFive"};

static string arrayVirtualTop5[]={"MemTopFive"};

static string arrayCheck[]={"AgentCheck","OcciCheck","ODBCCheck","SnmpCheck","WMICheck","VMwareCheck","sqlServerCheck","AgentLinuxCheck",
"DominoLotusCheck","TomcatCheck","ftpCheck","DB2Check","PostgresqlCheck","SybaseCheck","MySQLDependence","UnixDependence",
"Ping","CiscoProxyPing","UnixProxyPing","AgentPing"};

static string arrayTcpConnection[]={"tcpconnection","SnmpTCPConnection","AgentLinuxTCPConnection",};

static string arrayDiskIo[]={"AgentDisk"};

//特定功能类型的Plugin所使用的字段
static string arrayIndicator_CPU_Uage[]={"usage"};

static string arrayIndicator_Mem_Uage[]={"usage"};

static string arrayIndicator_Disk_Uage[]={"usage"};

static string arrayIndicator_DiskIo_Read[]={"diskreadspersec"};

static string arrayIndicator_DiskIo_Write[]={"diskwritespersec"};

/************************************************************************/
/* raw_message---接受信息                                               */
/************************************************************************/

#include <string.h>
   
struct RawMessage{
	time_t	t_tm;
	char szIp[128];
	char szTsId[128];
	char szMsg[2048];
	RawMessage()
	{
		memset(szTsId,0,128);
		memset(szMsg,0,2048);
		memset(szIp,0,128);
		t_tm = 0;
	}
};

//APPTYPE
enum APPTYPE{
	APPTYPE_NULL=-1,
	APPTYPE_ANDROID=0,
	APPTYPE_IOS,
};

#define API_STATUS_SUCCESS "success"
#define API_STATUS_FAILED  "failed"

#define SAAS_VERSION_KEY "saas_version"
#define SAAS_VERSION "100"

//本单机版或者多TS版License监测方式
enum LicenseCheckType{LicenseCheckType_Local=0,LicenseCheckType_Web=1};