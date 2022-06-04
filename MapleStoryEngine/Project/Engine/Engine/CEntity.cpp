#include "pch.h"
#include "CEntity.h"


UINT CEntity::g_iNextID = 0;

CEntity::CEntity()
	: m_iID(g_iNextID++)
{
}

CEntity::CEntity(const CEntity& _origin)
	: m_strName(_origin.m_strName)
	, m_iID(g_iNextID++)
{
}

CEntity::~CEntity()
{
}


void CEntity::SaveToFile(FILE* _pFile)
{
	SaveWString(m_strName, _pFile);
	
	//fwrite(&g_iNextID, sizeof(UINT), 1, _pFile);
	//fwrite(&m_iID, sizeof(UINT), 1, _pFile);
	

}

void CEntity::LoadFromFile(FILE* _pFile, bool IsPrevRead)
{
	if (IsPrevRead)
		return;

	LoadWString(m_strName, _pFile);
	
	//fread(&g_iNextID, sizeof(UINT), 1, _pFile);
	//fread(&m_iID, sizeof(UINT), 1, _pFile);
	

}
