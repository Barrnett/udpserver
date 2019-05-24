#pragma once

#include <string>
#include <algorithm> 
#include <wctype.h>
using namespace std;

#define _T(x)	x

template <class chT>
class MFCString : public basic_string<chT>
{
public:
	typedef chT XCHAR;
	typedef basic_string<chT> strT;

	MFCString() : strT() {}
	MFCString(const chT* sz) : strT(sz) {}
	MFCString(const chT* sz, int len) : strT(sz, len) {}
	MFCString(const strT& str) : strT(str) {}  

	chT* LockBuffer() { return GetBuffer(); }
	void UnlockBuffer() {}

	void Empty() { return this->clear(); } 
	void Format(const chT* pszFormat, ... ) {  
		va_list argList;
		va_start( argList, pszFormat );  

		int len = _vscprintf(pszFormat, argList); 
		if (len >  0) {
			this->resize(len + 1);
			vsprintf(const_cast<chT*>(this->c_str()), pszFormat, argList); 
		}

		va_end( argList );
	}

	chT operator[]( int iChar ) const {
		assert( iChar > 0 && iChar <= this->length() );  // Indexing the '\0' is OK
		if( (iChar < 0) || iChar > this->length() )	return '\0';
		return this->c_str()[iChar]; 
	}

/*	friend MFCString<chT> operator+(const MFCString<chT>& str1, const MFCString<chT>& str2 )	{
		MFCString<chT> s(str1);
		s += str2;
		printf("%s+%s=%s\r\n", str1.c_str(), str2.c_str(), s.c_str());   
		return( s );
	}  
*/	
/*	friend MFCString<chT> operator+(const MFCString<chT>& str1, const chT* str2 )	{
		MFCString<chT> s(str1);
		s += MFCString<chT>(str2);    
		return( s );
	}   
	friend MFCString<chT> operator+(const chT* str1, const MFCString<chT>& str2)	{
		MFCString<chT> s(str1);
		s += str2;       
		return( s );
	}   
 */       
	operator const chT*() const { return this->c_str(); }         
 
	/*
	MFCString<chT>& operator+=(const MFCString<chT>& str) {   
		strT::operator +=(str.c_str());  
		return *this;    
	} 
	*/
/*	MFCString<chT>& operator+=(const chT* str) {   
		strT::operator +=(str);  
		return *this;    
	} 
	MFCString<chT>& operator+=(chT ch) {   
		strT::operator +=(ch);  
		return *this;    
	} 
	MFCString<chT>& operator+=(const strT& str) {   
		strT::operator +=(str);  
		return *this;    
	} 
*/
	int Compare(const chT* psz ) const { return this->compare(psz); }
	int CompareNoCase(const chT* psz ) const { 
		if (psz == 0 || *psz == 0)
			return this->empty() ? 0 : 1;
		auto it = this->begin();
		while (it != this->end() && *psz != '\0') {
			chT ch1 = ToUpper(*it++);
			chT ch2 = ToUpper(*psz++);
			if (ch1 != ch2) 
				return (ch1 < ch2) ? -1 : 1;
		}
		if (it != this->end())
			return 1;
		else if (*psz != 0)
			return -1;
		else
			return 0; 
	}
	int CollateNoCase(const chT* psz ) const { return CompareNoCase(psz); }
	
	int Replace(const chT* pszOld, const chT* pszNew) {
		int oldSubLen = strlen(pszOld);
		int newSubLen = strlen(pszNew);
		int nOccur = 0;
		for(typename strT::size_type pos(0); pos!=strT::npos; pos+=newSubLen)   {     
			if((pos=this->find(pszOld,pos)) != strT::npos) {
			    this->replace(pos, oldSubLen, pszNew);     
				nOccur++;
			} else   
				break;     
		}
		return nOccur;     
	}
	int Find(const chT* pszSub, int iStart = 0) const { return this->find(pszSub, iStart); }
	int Find(const chT ch, int iStart = 0) const { return this->find(ch, iStart); }
	int Find(const MFCString<chT>& str) const { return this->find((const chT*)str, 0); }  
	int SafePos(int pos) const {
		if (pos < 0)
			return 0;
		else if (pos > this->length())
			return this->length();
		return pos;
	} 
	void SafePos(int& start, int& count) const {
		start = SafePos(start);
		if (count < 0)
			count = 0;
		else if (count + start > this->length())
			count = this->length() - start;
	} 
	MFCString<chT> Right(int nCount) const { 
		nCount = SafePos(nCount);
		return MFCString<chT>( this->substr(this->length()-nCount, nCount).c_str() ); 
	}
	MFCString<chT> Left(int nCount) const { return MFCString<chT>( this->substr(0, SafePos(nCount)).c_str() ); }
	MFCString<chT> Mid(int iFirst, int nCount ) const { 
		SafePos(iFirst, nCount);
		return MFCString<chT>(this->substr(iFirst, nCount).c_str()); 
	}   
	MFCString<chT> Mid(int iFirst) const { return MFCString<chT>(this->substr(SafePos(iFirst)).c_str()); }
       
	int Remove(chT ch) { 
		auto it = std::remove(this->begin(), this->end(), ch); // std::remove doesn't really remove, it only moves matched chars to the end, and return iterator.
		int ret = this->end() - it;
		this->resize(it - this->begin());
		return ret;
	} 
	int GetLength() const { return this->length(); }
	bool IsEmpty() const {	return this->empty(); }
	int ReverseFind(chT ch) const { return this->rfind(ch); }
	chT GetAt(int iChar) const { return this->c_str()[iChar]; }
	
	chT* GetBuffer() { return const_cast<chT*>(this->c_str()); }
	chT* GetBuffer(int buflen) { 
		if (buflen > this->length())
			this->resize(buflen);
		return const_cast<chT*>(this->c_str()); 
	}
	chT* GetBufferSetLength(int len) { 
		this->resize(len);
		return const_cast<chT*>(this->c_str()); 
	}
	void ReleaseBuffer(int newLength = -1) { 
		if (newLength == -1) {
			newLength = this->size();
			const chT* psz = this->c_str();
			for (int i=0; i<newLength; i++) {
				if (psz[i] == 0) {
					newLength = i;
					break;
				}
			}
		}

		if (newLength >= 0)
			this->resize(newLength);
	}  

	MFCString<chT>& TrimRight(const chT* pszTargets = " \t\r\n") {  
		auto pos = this->find_last_not_of(pszTargets);
		if (pos == strT::npos)
			this->clear();
		else
			*this = Left(pos+1);     
		return *this;
	}  
	MFCString<chT>& TrimLeft(const chT* pszTargets = " \t\r\n") {  
		auto pos = this->find_first_not_of(pszTargets);
		if (pos != strT::npos)
			*this = Right(this->length()-pos);        
		return *this;
	} 
	MFCString<chT>& Trim(const chT* pszTargets = " \t\r\n") { return TrimLeft(pszTargets).TrimRight(pszTargets); }  
	MFCString<chT>& MakeUpper() { 
		transform(this->begin(), this->end(), this->begin(), ToUpper);
		return *this;
	}
	MFCString<chT>& MakeLower() { 
		transform(this->begin(), this->end(), this->begin(), ToLower);
		return *this;
	}
	int Insert(int nPos, chT *szStr) { // return: new string length 
		if (NULL != szStr) {
			nPos = SafePos(nPos); 
			this->insert(nPos, szStr); 
		}
		return this->length(); 
	} 
	int Insert(int nPos, const MFCString<chT>& str) {  return Insert(nPos, (const chT*)str); }
	void SetAt(int iChar, chT ch) {
		int count = 1;
		SafePos(iChar, count);
		if (count == 1)
			this->at(iChar) = ch;
	}

	int Delete(int iIndex, int nCount = 1) {	return this->erase(max(0, iIndex), max(0, nCount)).length(); }
	void Append(const chT* sz, int len) {	this->append(sz, len); }
protected:
	static chT ToUpper(chT ch);
	static chT ToLower(chT ch);
};   

typedef MFCString<char> CStringA;
typedef MFCString<wchar_t> CStringW;
typedef CStringA CString;

template <> inline char CStringA::ToUpper(char ch) {	return ::toupper(ch); }
template <> inline char CStringA::ToLower(char ch) {	return ::tolower(ch); }
template <> inline wchar_t CStringW::ToUpper(wchar_t ch) {	return ::towupper(ch); }
template <> inline wchar_t CStringW::ToLower(wchar_t ch) {	return ::towlower(ch); }
