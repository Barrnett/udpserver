#pragma once

#ifndef WIN32

#include <unistd.h>   
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <ctype.h>
#include <netdb.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map>  
#include <stdarg.h>
#include <dlfcn.h>
#include <sys/ioctl.h>  
#include <mutex>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>  
#include <signal.h>
#include <limits.h>
#include <algorithm>

typedef unsigned int	DWORD;
typedef int				BOOL;
typedef unsigned int	UINT;
typedef unsigned short	USHORT;
typedef void			*LPVOID;
typedef int				HANDLE;  
typedef unsigned int    UINT32, *PUINT32;
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef const char		*LPCSTR, *PCSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;
typedef long			LRESULT;
typedef unsigned int	WPARAM;
typedef unsigned int	LPARAM;
typedef int				errno_t;  
typedef long long		INT64, LONGLONG;    
typedef long			LONG;
typedef unsigned long  ULONG;

typedef unsigned char byte;
typedef char *LPSTR, *LPTSTR;

typedef struct hostent *LPHOSTENT;

typedef BYTE  *LPBYTE;

typedef long	LONG;
typedef long			LONG_PTR; // 32bit in win32/64, linux32; 64 bit in linux64

typedef DWORD	COLORREF;
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))


#define __int64			long long
#define _int64			__int64 

#define FALSE   0
#define TRUE    1
#define INFINITE        0xFFFFFFFF
#define WM_USER 0x0400

#define MAKELONG(a, b)      ((long)(((uint16)((unsigned int)(a) & 0xffff)) | ((uint32)((uint16)((unsigned int)(b) & 0xffff))) << 16))
#define MAKELPARAM(l, h)    ((LPARAM)(DWORD)MAKELONG(l, h))
#define LOWORD(l)           ((WORD)((unsigned long)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((unsigned long)(l) >> 16))

#ifndef TRACE
	#ifdef _DEBUG
		#define TRACE(format, args...)	fprintf (stderr, format, ##args)
	#else
		#define TRACE
	#endif
#endif

#define AFX_EXT_CLASS
#define WINAPI
#define CALLBACK
#define FAR
#define far
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
typedef long   SOCKET;
#define SOCKET_ERROR            (-1)
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (-1)       // linux socket type is int, so use (-1), otherwise use (~0)
#endif
#define WSAETIMEDOUT  ETIMEDOUT   
#define WSAEWOULDBLOCK EINPROGRESS  
#define WSAECONNRESET	ECONNRESET
#define WSAEINVAL  EINVAL
#define WSAEPROTONOSUPPORT  EPROTONOSUPPORT
#define closesocket close
#define CloseHandle close
#ifdef DEBUG
#define ASSERT assert
#else
#define ASSERT(x) 
#endif

#define _tmain      main

#ifndef _UNICODE
typedef char TCHAR;
#define _TCHAR TCHAR  
#define lstrcpy strcpy
#define lstrcpyA lstrcpy
#define lstrcpyW wcscpy
#define _ttol       atol
#endif
  
#define stdext std
#define hash_map unordered_map  
#define __min(X,Y) (X < Y ? X : Y)     
#define __max(X,Y) (X < Y ? Y : X)  
typedef void* HMODULE;
typedef void* HINSTANCE; 
#define OutputDebugString	printf 
#define _atoi64 atoll
#define MAX_PATH          PATH_MAX 
#define _MAX_PATH          MAX_PATH 
#define _MAX_FNAME		   256
#define vsprintf_s	vsnprintf
#define _strdup		strdup
#define _stricmp	strcasecmp  
#define _vsnprintf	vsnprintf
#define _itoa		itoa  
#define __stdcall
// time64
#define __time64_t time_t        
#define _time64  time
#define _localtime64 localtime       
#define strnicmp   strncasecmp

typedef unsigned long DWORD_PTR;  

#define VERIFY(f) ((void)(f))  

inline DWORD GetTickCount()
{
    struct timespec ts;  
    clock_gettime(CLOCK_MONOTONIC, &ts);  
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);  
}

inline void WSASetLastError(int iError)
{
	errno = iError;
}

inline int WSAGetLastError()
{
	return errno;  
}

inline DWORD GetLastError() { return errno; }
inline void SetLastError(DWORD dwError) { errno = dwError;};

inline BOOL SetEnvironmentVariable(const char* name, const char* value)
{
	return setenv(name, value, 1) == 0;
}

inline DWORD GetEnvironmentVariable(const char* name, char* buf, DWORD size)
{
	char* value = getenv(name);
	if (value == NULL)
		return 0;
	int len = __min(size, strlen(value)+1); 
	strncpy(buf, value, len);
	return len - 1;
}

inline DWORD GetCurrentDirectory(DWORD len, char* buf)
{
	return (getcwd(buf, len) == NULL) ? 0 : strlen(buf) + 1;
}

inline BOOL CreateDirectory(const char* lpPathName, void* /*lpSecurityAttributes*/)  
{
	return mkdir(lpPathName, S_IRWXG) == 0;
}

inline BOOL RemoveDirectory(const char* lpPathName)  
{
	int ret = rmdir(lpPathName);
	if (ret != 0)
		perror("delete dir failed");
	return ret == 0;
}

inline char* itoa(int n, char* buf, int radix)
{
	switch (radix) {
		case 10:
			sprintf(buf, "%d", n);  
			return buf;
		case 16:
			sprintf(buf, "%x", n);
			return buf;
		case 8:
			sprintf(buf, "%o", n);
			return buf;
		default:
			assert(false);	// not implemented
			sprintf(buf, "%d", n);
			return buf;
	}
	return NULL;
}

inline errno_t _itoa_s(int n, char * buf, size_t len, int radix)  
{
	if (!buf || len <= 0)            
		return EINVAL;

	int nNeed;
	switch (radix) {
		case 10:
			nNeed = snprintf(buf, len, "%d", n);  
			return nNeed < len ? 0 : EINVAL;
		case 16:
			nNeed = snprintf(buf, len, "%x", n);
			return nNeed < len ? 0 : EINVAL;
		case 8:
			nNeed = snprintf(buf, len, "%o", n);
			return nNeed < len ? 0 : EINVAL;
		default:
			assert(false);	// not implemented
			return EINVAL;
	}
}

#define _ultoa ultoa  
inline char* ultoa(unsigned long n, char* buf, int radix)
{
	switch (radix) {
		case 10:
			sprintf(buf, "%lu", n);  
			return buf;
		case 16:
			sprintf(buf, "%lx", n);
			return buf;
		case 8:
			sprintf(buf, "%lo", n);
			return buf;
		default:
			assert(false);	// not implemented
			sprintf(buf, "%lu", n); 
			return buf;
	}
	return NULL;
}

inline char* ltoa(long n, char* buf, int radix)
{
	switch (radix) {
		case 10:
			sprintf(buf, "%ld", n);  
			return buf;
		case 16:
			sprintf(buf, "%lx", n);
			return buf;
		case 8:
			sprintf(buf, "%lo", n);
			return buf;
		default:
			assert(false);	// not implemented
			sprintf(buf, "%ld", n); 
			return buf;
	}
	return NULL;
}

inline char* _ui64toa(unsigned long long n, char* buf, int radix)
{
	switch (radix) {
		case 10:
			sprintf(buf, "%llu", n);  
			return buf;
		case 16:
			sprintf(buf, "%llx", n);
			return buf;
		case 8:
			sprintf(buf, "%llo", n);
			return buf;
		default:
			assert(false);	// not implemented
			sprintf(buf, "%llu", n);
			return buf;
	}
	return NULL;
}

inline char* _i64toa(unsigned long long n, char* buf, int radix)
{
	switch (radix) {
		case 10:
			sprintf(buf, "%llu", n);  
			return buf;
		default:
			assert(false);	// not implemented
			sprintf(buf, "%llu", n);
			return buf;
	}
	return NULL;
}

inline errno_t _i64toa_s(long long n, char * buf, size_t len, int radix)   
{
	if (!buf || len <= 0)            
		return EINVAL;

	int nNeed;
	switch (radix) {
		case 10:
			nNeed = snprintf(buf, len, "%lld", n);  
			return nNeed < len ? 0 : EINVAL;
		case 16:
			nNeed = snprintf(buf, len, "%llx", n);
			return nNeed < len ? 0 : EINVAL;
		case 8:
			nNeed = snprintf(buf, len, "%llo", n);
			return nNeed < len ? 0 : EINVAL;
		default:
			assert(false);	// not implemented
			return EINVAL;
	}
}

inline unsigned long long _strtoui64(const char *nptr, char **endptr, int base) 
{
	return strtoull(nptr, endptr, base);
}

inline long long _strtoi64(const char * _String, char ** _EndPtr, int _Radix)
{
	return strtoll(_String, _EndPtr, _Radix);
}

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

inline void GetLocalTime(LPSYSTEMTIME lpSystemTime) 
{
	time_t timep;
    struct tm *p;
    
	time(&timep);
	p = localtime(&timep); 

	lpSystemTime->wYear = p->tm_year;
	lpSystemTime->wMonth = p->tm_mon;
	lpSystemTime->wDay = p->tm_mday;
	lpSystemTime->wDayOfWeek = p->tm_wday;
	lpSystemTime->wHour = p->tm_hour;
	lpSystemTime->wMinute = p->tm_min;
	lpSystemTime->wSecond = p->tm_sec;
	lpSystemTime->wMilliseconds = 0;
}  

inline int _vscprintf (const char * format, va_list pargs)
{ 
    va_list argcopy; // it's said vsxxxx will used it up, so copy it 
    va_copy(argcopy, pargs);	
    int retval = vsnprintf(NULL, 0, format, argcopy); 
    va_end(argcopy);  

    return retval;
}

inline int sprintf_s(char* dest, size_t bufsize, const char * format, ...)
{
	va_list argList;
	va_start( argList, format );    
	int ret = vsnprintf(dest, bufsize, format, argList);
	va_end( argList );
	return ret;
}

inline int sprintf_s(char* dest, const char * format, ...)         
{
	va_list argList;
	va_start( argList, format );    
	int ret = vsprintf(dest, format, argList);
	va_end( argList );
	return ret;
}

/*
this overidden doesn't work, why?
template <size_t _Size>
inline int sprintf_s(char dest[_Size], size_t bufsize, const char * format, ...)       
{
	printf("using sprintf_s, _Size=%d, format=%s\n", _Size, format);
	va_list argList;
	va_start( argList, format );    
	int ret = vsnprintf(dest, _Size, format, argList);
	va_end( argList );
	printf("after sprintf_s, dest=%s\n", dest);
	printf("sprintf_s return %d\n", ret); 
	return ret;
}
*/

inline bool NameDll2So(std::string& name)
{
	const char* sdll = ".dll";
	if (name.length() <= strlen(sdll))
		return false;
	std::string tail = name.substr(name.length()-strlen(sdll));
	if (strcasecmp(tail.c_str(), sdll) != 0)
		return false;
	name = "lib" + name.substr(0, name.length()-strlen(sdll)) + ".so";
	printf("modified filename from *.dll to lib*.so\r\n");
	return true;
}

inline bool PathDll2So(std::string& fullpath)
{
	std::string path, file(fullpath);
	std::string::size_type slashpos = file.find_last_of('/');
	if (slashpos != std::string::npos) {
		path = file.substr(0, slashpos+1);
		file = file.substr(slashpos+1);
	}
	if (NameDll2So(file)) {
		fullpath = path + file;
		return true;
	}
	return false;
}

inline HMODULE LoadLibrary(const char* name)
{
	// split path & filename. if filename likes "*.dll", change it to "lib*.so"
	std::string path(name);
	PathDll2So(path);

	HMODULE ret = dlopen(path.c_str(), RTLD_LAZY);
	if (ret == NULL)
		printf("LoadLibrary %s failed. %s\r\n", name, dlerror());                      

	return ret;
}

#define FreeLibrary dlclose

inline BOOL IsDBCSLeadByteEx(UINT CodePage, BYTE TestChar)
{
	return TestChar >= 0x80;
}

#include "IniFile.h"  
inline DWORD GetPrivateProfileString(LPCSTR lpszSection, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName)
{
	chen::CIniFile iFile;
	iFile.Init(lpFileName);
	string str = iFile.GetIniSetting(lpszSection, lpKeyName, lpDefault);
	if(str.empty())
		return 0;
	int nCopy = std::min((DWORD)str.length(), nSize-1);
	strncpy(lpReturnedString, str.c_str(), nCopy);
	lpReturnedString[nCopy] = '\0';
	return nCopy;
}

inline DWORD GetPrivateProfileSection(const char* lpszSection, char* buf, int size, const char* lpFileName)  
{
	chen::CIniFile iFile;
	iFile.Init(lpFileName);
	map<string, string> keymap;
	if (!iFile.GetSection(lpszSection, keymap))   
		return 0;
	memset(buf, 0, size);
	char* cursor = buf;
	char* boundary = buf + size - 2/*keep '\0\0' at tail*/;
	for (map<string, string>::iterator it=keymap.begin(); it!=keymap.end(); ++it) {
		if (cursor >= boundary) {
			printf("GetPrivateProfileSection: content is too long, truncated.\n");
			return size-2;
		}
		cursor += snprintf(cursor, boundary-cursor, "%s=%s", it->first.c_str(), it->second.c_str());
		cursor++; // for '\0'
	}
	return cursor - buf; // cursor point to the terminating 0, so needn't +1  
}

inline BOOL WritePrivateProfileString(LPCSTR lpszSection, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName)
{
	chen::CIniFile iFile;
	iFile.Init(lpFileName);
	iFile.PutIniSetting(lpszSection, lpKeyName, lpString);
	return TRUE;
}

inline UINT GetPrivateProfileInt(LPCSTR lpszSection, LPCSTR lpKeyName, int nDefault, LPCSTR lpFileName)
{
	chen::CIniFile iFile;
	iFile.Init(lpFileName);
	string str = iFile.GetIniSetting(lpszSection, lpKeyName, ""); 
	if(str.empty())
		return nDefault;
	return atoi(str.c_str());
}

inline DWORD GetModuleFileName(HMODULE hModule, char* name, DWORD size) 
{
	if (hModule != NULL) {
		printf("GetModuleFileName: not supported if hModule != NULL\n"); 
		return 0;	// only can get exe file name
	}
	char buf[PATH_MAX];
	int rslt = readlink("/proc/self/exe", buf, sizeof(buf)-1);
	if (rslt <= 0 || rslt >= size)
		return 0;
	buf[rslt] = '\0';
	if (realpath(buf, name) == NULL)
		return 0;
	return rslt;
}

#define _chdir chdir
inline char* _fullpath(char * _FullPath, const char * _Path, size_t _SizeInBytes)
{
	return realpath(_Path, _FullPath);  
}

#define sscanf_s sscanf    

#define AFX_MANAGE_STATE(x)  

class CCriticalSection : public recursive_timed_mutex {     
public:
	BOOL Unlock() { unlock(); return TRUE; }
	BOOL Lock() { lock(); return TRUE; } 
	BOOL Lock(DWORD dwTimeout) { return try_lock_for(chrono::milliseconds(dwTimeout)); }        
};

typedef CCriticalSection CRITICAL_SECTION;
typedef CRITICAL_SECTION* LPCRITICAL_SECTION;  
inline void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection) {}
inline void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection) { lpCriticalSection->Unlock(); }
inline void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection) { lpCriticalSection->Lock(); }
inline void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection) { lpCriticalSection->Unlock(); }
inline BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection) { return lpCriticalSection->try_lock(); }  

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))  

inline errno_t fopen_s(FILE** pFile, const char *filename, const char *mode)
{
	*pFile = fopen(filename, mode);
	return errno; 
}

#define _access access 
#define _alloca	alloca  
inline int localtime_s(struct tm * _Tm, const time_t * _Time)
{
	return localtime_r(_Time, _Tm) == NULL ? errno : 0;
}

#define _strupr	strupr
inline char *strupr(char *str)
{
	if (str == NULL)
		return NULL;
    char *orign = str;
    for (; *str!='\0'; str++)  
        *str = toupper(*str);
    return orign;
}

#define _strlwr	strlwr
inline char *strlwr(char *str)
{
	if (str == NULL)
		return NULL;
    char *orign = str;
    for (; *str!='\0'; str++)  
        *str = tolower(*str);
    return orign;
}

#include <sys/syscall.h>     
#define gettid() syscall(__NR_gettid)        
inline DWORD GetCurrentThreadId() { return gettid(); }       

#define SD_BOTH	(0x2)

#define INVALID_HANDLE_VALUE ((HANDLE)-1)

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION; 

#define WAIT_TIMEOUT ETIMEDOUT
#define WAIT_OBJECT_0	0
#define WAIT_FAILED ((DWORD)0xFFFFFFFF)

inline BOOL FlushFileBuffers(HANDLE hFile)
{
	return fsync(hFile) != -1; 
}

inline BOOL DeleteFile(LPCSTR lpFileName)
{
	return remove(lpFileName) != -1;
}

inline void Sleep(DWORD ms) { 
	usleep(ms * 1000);
}

inline void ExitProcess(unsigned int n) { exit(n); }
//inline void ExitThread(unsigned int n) { pthread_exit((void*)n); }
inline void ExitThread(unsigned int n) { pthread_exit((void*)NULL); }

typedef int (*FARPROC)();

inline FARPROC GetProcAddress (HMODULE hModule, const char* lpProcName) {
	return (FARPROC) dlsym(hModule, lpProcName);
}

inline errno_t strcpy_s(char *pDes,size_t sizeInByte,const char *pSrc)
{
	if (sizeInByte <= 0)            
		return ERANGE;
	if (pDes == NULL)
		return EINVAL;
	if (pSrc == NULL) {
		pDes[0] = 0;
		return EINVAL;
	}

	int nSize = strlen(pSrc) + 1;
	if (nSize > sizeInByte) {
		pDes[0] = 0;
		return ERANGE;
	}

	strncpy(pDes,pSrc,nSize);
	return 0;
}

#define _TRUNCATE ((size_t)-1)
inline errno_t _strncpy_s(char *pDes,size_t sizeInByte,const char *pSrc, size_t count)
{
	if (sizeInByte <= 0 || pDes == NULL)
		return EINVAL;
	if (pSrc == NULL) {
		pDes[0] = 0;
		return EINVAL;
	}

	bool trunc = (count == _TRUNCATE);
	if (trunc)
		count = strlen(pSrc);
	if (count >= sizeInByte) {
		if (trunc) {
			count = sizeInByte - 1;
		} else { 
			pDes[0] = 0;
			return EINVAL;
		} 
	}

	strncpy(pDes,pSrc,count);
	pDes[count+1] = 0;
	return 0;
}
#define tcsncpy_s _strncpy_s

inline int _tcsnicmp(const char * _String1, const char * _String2, size_t _Char_count) 
{
	return strncasecmp(_String1, _String2, _Char_count);
}

inline char* _tcsinc(const char* p)
{
	return const_cast<char*>(++p);
}

#define SW_HIDE             0
#define SW_SHOW             5
inline unsigned int WinExec(const char* cmd, UINT /*uCmdShow*/)    
{
	pid_t pid = fork();
	if (pid == 0) {
		execl("/bin/sh", "sh", "-c", cmd, (char *)0);   
		exit(127);
	}

	if (pid < 0) 
		return false;

	return pid + 31; // msdn: If the function succeeds, the return value is greater than 31.
}

template <typename _CountofType, size_t _SizeOfArray>
char (*__countof_helper(_CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
#define _countof(_Array) sizeof(*__countof_helper(_Array))

inline errno_t memmove_s(void *dest, size_t numberOfElements, const void *src, size_t count)
{
	if (dest == NULL || src == NULL)
		return EINVAL;
	if (count < 0 || numberOfElements < 0 || count > numberOfElements)
		return ERANGE;
	memmove(dest, src, count);
	return 0;
}

#define _tcsstr	strstr

// fake GlobalAlloc: not moveable
#define GMEM_MOVEABLE	0
typedef void*           HGLOBAL;
inline HGLOBAL GlobalAlloc(UINT uFlags, size_t dwBytes) {	return malloc(dwBytes); }
inline void* GlobalLock (HGLOBAL hMem) { return hMem; }
inline BOOL GlobalUnlock(HGLOBAL hMem) { return TRUE; }
inline HGLOBAL GlobalReAlloc(HGLOBAL hMem, size_t dwBytes, UINT uFlags) {	return realloc(hMem, dwBytes); }
inline size_t GlobalSize(HGLOBAL hMem) { return (*(((unsigned int *)hMem)-1) & ~(0x01|0x02)); }
inline HGLOBAL GlobalFree(HGLOBAL hMem) { free(hMem);	return NULL; }


typedef long long INT_PTR, *PINT_PTR, UINT_PTR;
typedef unsigned long long ULONG_PTR, *PULONG_PTR;
typedef unsigned int        *PUINT;

inline int lstrlenA(LPCSTR lpString) {	return strlen(lpString); }
inline int lstrlenW(const wchar_t* lpString) { return wcslen(lpString); }
#define lstrlen lstrlenA

#define lstrcmp strcmp
#define lstrcmpA lstrcmp
#define lstrcmpW wcscmp
#define _popen popen
#define _pclose pclose


#endif
