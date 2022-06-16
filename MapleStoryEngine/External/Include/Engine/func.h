#pragma once


template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		SAFE_DELETE(_vec[i]);
	}
	_vec.clear();
}

template<typename T>
void Safe_Del_list(list<T*>& _list)
{
	typename list<T*>::iterator iter = _list.begin();

	for (; iter != _list.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	_list.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();
	for (; iter != _map.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	_map.clear();
}

template<typename T, UINT _iSize>
void Safe_Del_Arr(T(&_Arr)[_iSize])
{
	for (UINT i = 0; i < _iSize; ++i)
	{
		SAFE_DELETE(_Arr[i]);
	}
}


const wchar_t* ToWString(COMPONENT_TYPE _type);
const char* ToString(COMPONENT_TYPE _type);
const wchar_t* ToWString(RES_TYPE _type);
const char* ToString(RES_TYPE _type);

// To Do 
const wchar_t* ToWString(SHADER_DOMAIN _type);
const char* ToString(SHADER_DOMAIN _type);
const wchar_t* ToWString(RS_TYPE _type);
const char* ToString(RS_TYPE _type);
const wchar_t* ToWString(CB_TYPE _type);
const char* ToString(CB_TYPE _type);

const wchar_t* ToWString(DS_TYPE _type);
const char* ToString(DS_TYPE _type);
const wchar_t* ToWString(BS_TYPE _type);
const char* ToString(BS_TYPE _type);


void SaveStringToFile(const string& _str, FILE* _pFile);
void LoadStringFromFile(string& _str, FILE* _pFile);

void SaveWStringToFile(const wstring& _str, FILE* _pFile);
void LoadWStringFromFile(wstring& _str, FILE* _pFile);




#include "Ptr.h"
#include "CResMgr.h"

template<typename RES>
void SaveResPtr(Ptr<RES> _ptr, FILE* _pFile)
{
	bool bNullPtr = nullptr == _ptr ? true : false;
	fwrite(&bNullPtr, sizeof(bool), 1, _pFile);

	if (!bNullPtr)
	{
		SaveWStringToFile(_ptr->GetKey(), _pFile);
		SaveWStringToFile(_ptr->GetRelativePath(), _pFile);
	}
}

template<typename RES>
void LoadResPtr(Ptr<RES>& _ptr, FILE* _pFile)
{
	bool bNull = false;
	fread(&bNull, sizeof(bool), 1, _pFile);

	if (!bNull)
	{
		wstring strKey, strRelativePath;

		LoadWStringFromFile(strKey, _pFile);
		LoadWStringFromFile(strRelativePath, _pFile);

		_ptr = CResMgr::GetInst()->Load<RES>(strKey, strRelativePath);
	}
}