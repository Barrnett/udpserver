#ifndef MXDB_VAR_INCLUDE_H_
#define MXDB_VAR_INCLUDE_H_

#ifndef WIN32
#include "Linux_Common.h"
#endif

//#include <mxdi.h>
//#include <MxErrorDef.h>
#include "VariableData.h"
#include "TimeFace.h"
#include "bsapi.h"
#include "bserror.h"
#include <string>
#include <list>
#include <stdarg.h>
#include <map>
using namespace std;

#define DBVAR_UNSERIALFAILED  5001

#define  DBVAR_CHECKUNSERIALDATA(_NRET)	if (BS_NOERROR!=_NRET) return DBVAR_UNSERIALFAILED;

typedef std::map<std::string ,chen::VariableData > BSPROPERTYSET;

class CDbVar
{
public:
	CDbVar(void);
	~CDbVar(void);
public:
	static int VariableDataToInt(chen::VariableData& rv);
	static std::string VariableDataToString(chen::VariableData& rv);
	static __int64 VariableDataToInt64(chen::VariableData& rv);
	static double VariableDataToDouble(chen::VariableData& rv);
	static float VariableDataToFloat(chen::VariableData& rv);
	static int SerialString(char ** p,int nLen,std::string szData);
	static int SerialTime(char ** p,int nLen,chen::TimeFace tmFace);
	static int SerialInt(char ** p,int nLen,int nData);
	static int SerialVar( char ** p,int nLen,chen::VariableData rv);
	static int UnSerialVar( char **pSData,chen::VariableData&rv, int nDataLen);
	static string GetStringFromProperty(string key,string defaultret, BSPROPERTYSET& PropList);
	static int GetIntFromProperty(string key,int defaultret, BSPROPERTYSET& PropList);
	//static void GetSomeStringDataFromProperty(BSPROPERTYSET& PropList,int nCount,...);
	static void GetSomeDataFromProperty( BSPROPERTYSET& PropList,char* sFormat,... );
	static int CalMapSerialSize(std::map<std::string, chen::VariableData > mapV);
	static int SerialStringVarMap( char *pData,int nDatalen,int& nUsedLen, std::map<std::string, chen::VariableData > mapV );
	static int UnSerialStringVarMap( char **pData,int nDataLen, std::map<std::string, chen::VariableData >& mapV );
	static double GetDoubleFromProperty(string key,double defaultret, BSPROPERTYSET& PropList);
	static float GetFloatFromProperty(string key,float defaultret, BSPROPERTYSET& PropList);
	static __int64 GetInt64FromProperty(string key,int defaultret, BSPROPERTYSET& PropList);
	//static bool abc(BSPROPERTYSET& PropList,std::string szProp,std::string& szValue);
	
};



#endif