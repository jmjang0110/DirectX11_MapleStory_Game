#include "pch.h"
#include "CComponent.h"

#include "CEventMgr.h"

CComponent::CComponent(COMPONENT_TYPE _eType)
	: m_eComType(_eType)
	, m_bActive(true)
{
}

CComponent::~CComponent()
{
}

void CComponent::Activate()
{
	tEventInfo info = {};

	info.eType = EVENT_TYPE::ACTIVATE_COMPONENT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

void CComponent::Deactivate()
{
	tEventInfo info = {};

	info.eType = EVENT_TYPE::DEACTIVATE_COMOPNENT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

void CComponent::SaveToFile(FILE* _pFile)
{
	CEntity::SaveToFile(_pFile);

	//fwrite(&m_eComType, sizeof(UINT), 1, _pFile);
	//fwrite(&m_pOwner, sizeof(CGameObject*), 1, _pFile);
	fwrite(&m_bActive, sizeof(bool), 1, _pFile);


}

void CComponent::LoadFromFile(FILE* _pFile, bool IsPrevRead)
{
	CEntity::LoadFromFile(_pFile, IsPrevRead);

	//fread(&m_eComType, sizeof(UINT), 1, _pFile);
	//fread(&m_pOwner, sizeof(CGameObject*), 1, _pFile);
	fread(&m_bActive, sizeof(bool), 1, _pFile);
}


