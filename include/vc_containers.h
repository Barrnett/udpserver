#pragma once

#include <map>
#include <list>
#include <memory>
#include <vector>
using namespace std; 

class base_of_any { 
public:
	virtual ~base_of_any() {};    
};

template<class AnyTy>
class wrapper_of_any : public base_of_any {
public:
	AnyTy m_any;
	virtual ~wrapper_of_any() {} 
};

struct POSITION {
	shared_ptr<base_of_any> m_pIter;   

	operator bool() const { return m_pIter != NULL; }  
	operator base_of_any*() const { return m_pIter.get(); }          
	POSITION()  {}
	POSITION(base_of_any* ptr) : m_pIter(ptr) {} 
	void reset() { m_pIter.reset(); }
};

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CMap : public map<KEY, VALUE> {   
public:
	typedef typename map<KEY, VALUE>::iterator base_iterator;  
	typedef typename map<KEY, VALUE>::const_iterator const_base_iterator;  
	CMap() : map<KEY, VALUE>() {}  
	POSITION GetStartPosition() {
		if (this->empty())
			return POSITION(NULL); 
		wrapper_of_any<base_iterator>* pIter = new wrapper_of_any<base_iterator>;
		pIter->m_any = this->begin();  
		return POSITION(pIter); 
	}  

	void GetNextAssoc(POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const { 
		wrapper_of_any<base_iterator>* pIter = (wrapper_of_any<base_iterator>*)rNextPosition.m_pIter.get();
		if (pIter == NULL)
			return;
		base_iterator& it = pIter->m_any;  
		rKey = it->first;
		rValue = it->second;
		++it;
		if (it == this->end())  
			//rNextPosition.m_pIter = NULL;
			rNextPosition.reset();
	}

	void RemoveAll() { this->clear(); }
	void SetAt(KEY key, VALUE newValue) {  
		this->operator[](key) = newValue;    
	}
	BOOL Lookup(ARG_KEY key, VALUE& rValue) const {
		const_base_iterator it = this->find(key);     
		if (it == this->end()) 
			return FALSE;
		rValue = it->second;
		return TRUE;
	}
	unsigned int GetCount() { return this->size(); }

	BOOL RemoveKey(ARG_KEY key)
	{
		//const_base_iterator it = this->find(key);
		base_iterator it = this->find(key);
		if(it == this->end())
			return FALSE;  // not found

		this->erase(it);
		return TRUE;
	}
};

typedef CMap<void*, void*&, void*, void*&> CMapPtrToPtr;

template<class TYPE,class ARG_TYPE = const TYPE&>
class CArray : public vector<TYPE>
{
public:
	CArray () : vector<TYPE>() {}
	void Copy(const CArray& src) { *this = src; }

    int GetCount() const {	return this->size(); }
    int GetSize() const {	return this->size(); } // equal to GetCount
	const TYPE* GetData( ) const {	return this->data(); }
	TYPE* GetData( ) {	return this->data(); }

    TYPE& GetAt(int nIndex) {
		ASSERT(nIndex >= 0 && nIndex < this->size());
		if(nIndex >= 0 && nIndex < this->size())
			return this->at(nIndex);
		return this->at(0);
	}

	int Add(ARG_TYPE newElement) {
		this->push_back(newElement);
		return this->size();
	}

	void SetAt(int nIndex, ARG_TYPE newElement) {
		ASSERT(nIndex >= 0 && nIndex < this->size());
		if(nIndex >= 0 && nIndex < this->size())
			(*this)[nIndex] = newElement;
	}

	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount =1) {	this->insert(this->begin()+nIndex,newElement); }
	void SetSize(int nNewSize, int nGrowBy = -1) {	this->resize(nNewSize); }
        void RemoveAll() { this->clear();}
        
};

template<class TYPE, class ARG_TYPE = const TYPE&> 
class CList : public list<TYPE> {    
public:
	typedef typename list<TYPE>::iterator base_iterator;  
	typedef typename list<TYPE>::const_iterator const_base_iterator;  
	CList() : list<TYPE>() {}  
	POSITION GetHeadPosition() {    
		if (this->empty())
			return POSITION(NULL); 
		wrapper_of_any<base_iterator>* pIter = new wrapper_of_any<base_iterator>;
		pIter->m_any = this->begin();   
		return POSITION(pIter); 
	}
	POSITION GetTailPosition()
	{
		wrapper_of_any<base_iterator>* pIter = new wrapper_of_any<base_iterator>;
		pIter->m_any=this->end();
		pIter->m_any--;
		return POSITION(pIter);
	}

	const TYPE& GetNext(POSITION& rPosition) const { 
		ASSERT(rPosition);

		wrapper_of_any<base_iterator>* pIter = (wrapper_of_any<base_iterator>*)rPosition.m_pIter.get(); 
		base_iterator& it = pIter->m_any;      

		const TYPE& ret = *it++; 
		if (it == this->end())  
			//rPosition.m_pIter = NULL;
			rPosition.reset();
		return ret;
	}
	const TYPE& GetPrev(POSITION& rPosition) const
	{
		ASSERT(rPosition);

		wrapper_of_any<base_iterator>* pIter = (wrapper_of_any<base_iterator>*)rPosition.m_pIter.get(); 
		base_iterator& it = pIter->m_any;

		const TYPE& ret=*it;
		if(it == this->begin())
			//rPosition.m_pIter=NULL;
			rPosition.reset();
		else it--;
		
		return ret;
	}

	void RemoveAt(POSITION position)
	{
		ASSERT(position);

		wrapper_of_any<base_iterator>* pIter=(wrapper_of_any<base_iterator>*)position.m_pIter.get();
		base_iterator it=pIter->m_any;

		this->erase(it);
	}

	void AddTail(ARG_TYPE newElement) { 
		this->push_back(newElement);
	}   
	void AddHead(ARG_TYPE newElement) { 
		this->push_front(newElement);   
	}   

	// add another list of elements before head or after tail
	void AddHead(CList* pNewList)
	{
		ASSERT(pNewList);

		// add a list of same elements to head (maintain order)
		POSITION pos=pNewList->GetTailPosition();
		while(pos)
			AddHead(pNewList->GetPrev(pos));
	}
	void AddTail(CList* pNewList)
	{
		ASSERT(pNewList);

		// add a list of same elements
		POSITION pos = pNewList->GetHeadPosition();
		while (pos)
			AddTail(pNewList->GetNext(pos));
	}

	// get head or tail (and remove it) - don't call on empty list !
	TYPE RemoveHead()
	{
		ASSERT(this);
		TYPE retValue=this->front();
		this->pop_front();
		return retValue;
	}
	TYPE RemoveTail()
	{
		ASSERT(this);
		TYPE retValue=this->back();
		this->pop_back();
		return retValue;
	}

	TYPE& GetHead()
	{
		return this->front();
	}
	TYPE& GetTail()
	{
		return this->back();
	}

	// remove all elements
	void RemoveAll()
	{
		this->clear();
	}

	POSITION Find(ARG_TYPE searchValue, POSITION startAfter = POSITION(NULL)) {  
		wrapper_of_any<base_iterator>* pIter = new wrapper_of_any<base_iterator>;
		base_iterator& it = pIter->m_any;       
		it = this->begin();   
		if (startAfter.m_pIter != NULL) {
			wrapper_of_any<base_iterator>* pIter1 = (wrapper_of_any<base_iterator>*)startAfter.m_pIter.get();  
			it = pIter1->m_any;      
		}
		while (it != this->end()) {
			if (*it == searchValue) 
				return POSITION(pIter);
			++it;
		}
		return POSITION(NULL);   
	}
	unsigned int GetCount() { return this->size(); } 

	BOOL IsEmpty() const{return (this->size() == 0);}
};  

class CStringList : public CList<CString> {
public:
	CStringList() : CList<CString>() {}    
};


typedef  CArray<DWORD>  CDWordArray; 
typedef  CArray<string> CStringArray;

typedef CList<void*> CPtrList;
typedef CArray<char> CByteArray;