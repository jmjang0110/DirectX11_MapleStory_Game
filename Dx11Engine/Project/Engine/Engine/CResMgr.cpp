#include "pch.h"
#include "CResMgr.h"

CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Del_Map(m_Res[i]);
	}
}