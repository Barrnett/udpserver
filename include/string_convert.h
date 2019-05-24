#pragma once

#include <iconv.h>

inline bool code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	char **pin = (char**) &inbuf;
	char **pout = &outbuf;
	bool ret = false;
	iconv_t cd = iconv_open(to_charset, from_charset);
	if (cd == 0) 
		return false;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) != (size_t)-1) 
		ret = true;
	iconv_close(cd);
	return ret;
}

inline bool UnicodeToANSI(const wchar_t* pwcSource, std::string& strDestination)
{
	size_t srcLen = wcslen(pwcSource) * 2;
	size_t destLen = srcLen * 2;

	char* destBuf = new char[destLen + 2];
	if (destBuf == NULL)
		return false;
	memset(destBuf, 0, destLen + 2);

	bool bret = code_convert("gbk//IGNORE", "utf-16", (const char*)pwcSource, srcLen, destBuf, destLen);
	if (bret) {
		strDestination = destBuf;
	}
	delete[] destBuf;
	return bret;
}

inline bool UnicodeToUTF8(const wchar_t* pwcSource, std::string& strDestination)
{
	size_t srcLen = wcslen(pwcSource) * sizeof(wchar_t);
	size_t destLen = srcLen * 2;

	char* destBuf = new char[destLen + sizeof(wchar_t)];
	if (destBuf == NULL)
		return false;
	memset(destBuf, 0, destLen + sizeof(wchar_t));

	bool bret = code_convert("gbk//IGNORE", "utf-8", (const char*)pwcSource, srcLen, destBuf, destLen);
	if (bret) {
		strDestination = destBuf;
	}
	delete[] destBuf;
	return bret;
}

inline bool ANSIToUnicode(const char* pcSource, std::wstring& wstrDestination)
{
	size_t srcLen = strlen(pcSource);
	size_t destLen = srcLen * 2;

	char* destBuf = new char[destLen + 2];
	if (destBuf == NULL)
		return false;
	memset(destBuf, 0, destLen + 2);

	bool bret = code_convert("ansi//IGNORE", "utf-16", pcSource, srcLen, destBuf, destLen);
	if (bret) {
		wstrDestination = (wchar_t*)destBuf;
	}
	delete[] destBuf;
	return bret;
}

inline bool UTF8ToUnicode(const char* pcSource, std::wstring& wstrDestination)
{
	size_t srcLen = strlen(pcSource);
	size_t destLen = srcLen * 2;

	char* destBuf = new char[destLen + 2];
	if (destBuf == NULL)
		return false;
	memset(destBuf, 0, destLen + 2);

	bool bret = code_convert("utf-8//IGNORE", "utf-16", pcSource, srcLen, destBuf, destLen);  
	if (bret) {
		wstrDestination = (wchar_t*)destBuf;
	}
	delete[] destBuf;
	return bret;
}

inline bool ANSIToUTF8(const char* pcSource, std::string& strDestination)
{
	size_t srcLen = strlen(pcSource);
	size_t destLen = srcLen * 2;

	char* destBuf = new char[destLen + 2];
	if (destBuf == NULL)
		return false;
	memset(destBuf, 0, destLen + 2);

	bool bret = code_convert("gbk//IGNORE", "utf-8", pcSource, srcLen, destBuf, destLen);
	if (bret) {
		strDestination = destBuf;
	}
	delete[] destBuf;
	return bret;
}

inline bool UTF8ToANSI(const char* pcSource, int srcLen, std::string& strDestination)
{
	size_t destLen = srcLen * 2;

	char* destBuf = new char[destLen + 2];
	if (destBuf == NULL)
		return false;
	memset(destBuf, 0, destLen + 2);

	bool bret = code_convert("utf-8//IGNORE", "gbk", pcSource, srcLen, destBuf, destLen);
	if (bret) {
		strDestination = destBuf;
	}
	delete[] destBuf;
	return bret;
}

inline bool UTF8ToANSI(const char* pcSource, std::string& strDestination)
{
	size_t srcLen = strlen(pcSource);
	return UTF8ToANSI(pcSource, srcLen, strDestination); 
}
