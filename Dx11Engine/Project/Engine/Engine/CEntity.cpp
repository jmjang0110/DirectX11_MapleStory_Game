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
