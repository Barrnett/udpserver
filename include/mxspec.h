#pragma once

inline string GetMXInstallPath(const char* dir_name = "Server")      
{
	char szPath[MAX_PATH];
	if (GetModuleFileName(NULL, szPath, sizeof(szPath)) <= 0)
		return "";
	
	string strpath = szPath;
	string dir, base;
	for(;;) {
		size_t pos = strpath.rfind("/");
		if (pos == string::npos)
			return "";
		dir = strpath.substr(0, pos);
		base = strpath.substr(pos+1);
		if (base == string(dir_name))
			return strpath;
		strpath = dir;
	} 
	return "";
}

inline string GetRootPath(const char* dir_name = "Server")
{
	return GetMXInstallPath(dir_name); 
}

