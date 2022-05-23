#include "pch.h"
#include "CPathMgr.h"

CPathMgr::CPathMgr()
	: m_strContentPath{}
{

}

CPathMgr::~CPathMgr()
{

}

void CPathMgr::init()
{
	// Content 폴더 경로를 찾아낸다.
	GetCurrentDirectory(256, m_strContentPath);

	// 부모 폴더 경로를 알아낸다( OutputFile 폴더 )
	int iLen = (int)wcslen(m_strContentPath);

	for (int i = iLen - 1; 0 <= i; --i)
	{
		if (L'\\' == m_strContentPath[i])
		{
			m_strContentPath[i] = L'\0';
			break;
		}
	}

	// content 폴더 경로를 붙인다.
	wcscat_s(m_strContentPath, L"\\bin\\content\\");
}
