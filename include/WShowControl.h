#ifndef SERVERDATA_CWShowControl
#define  SERVERDATA_CWShowControl

#include <string>
#include <list>
#include <map>
#include "VariableData.h"
using namespace std;
/**  
* @brief 控件信息 美信对控件的描述
*  
* 
*  
*/

class CWShowControl
{

public:
	CWShowControl(void);
	~CWShowControl(void);
public:
	char*  m_pName;
	
	char * m_pLabel;
	char * m_pHelp;
	char * m_pControlType;
	
	char * m_pDefault;
	
	char *	m_pFllowName;

	bool		m_bFollow;
	bool		m_bMust;
	bool		m_bSaveInMain;
	bool		m_bHidde;
	bool		m_bReadOnly;
	bool		m_bInputDll;
	bool		m_bOnlyNum;
	int			m_nIndex;
	std::map<std::string,chen::VariableData > m_mapAll; //reserver
public:
	CWShowControl& operator=(CWShowControl& A);
	void Clear();
	string GetName();
	string GetLabel();
	string GetHelp();
	string GetControlType();
	string GetDefault();
	string GetFllowName();

	std::string  Default();// const { return m_pDefault; }
	void Default(char * val) { 
		if (m_pDefault != NULL)
			delete m_pDefault;
		m_pDefault = val; 
	}

	std::string ControlType();// const { return m_pControlType; }
	void ControlType(char * val) { m_pControlType = val; }
    
	char* Name() const { return m_pName; }
	void Name(const char* val) { if (!val) return; if(m_pName) delete m_pName; m_pName = _strdup(val); }
	static bool indexcompare(CWShowControl* node1,CWShowControl* node2);
};

class CWShowComboBox :public CWShowControl
{
public:
	CWShowComboBox();
public:
	int		m_nParamCount;
	bool	m_bSort;
	std::list<std::pair<std::string,std::string>> m_lstDropContent;
};
class CWShowTextBox :public CWShowControl
{
public:
	CWShowTextBox();
	int		m_nRows;

};
class CWShowChekBox :public CWShowControl
{
public:
	CWShowChekBox();
};

#endif

