#include "pch.h"
#include "CRenderComponent.h"
#include "CResMgr.h"


CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	: CComponent(_type)
	, m_pMesh(nullptr)
	, m_pMtrl(nullptr)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _origin)
	: CComponent(_origin)
	, m_pMesh(_origin.m_pMesh)
	, m_pMtrl(nullptr)
	, m_pSharedMtrl(_origin.m_pMtrl)
	, m_pDynamicMtrl(nullptr)
{
	if (nullptr != _origin.m_pSharedMtrl)
	{
		SetSharedMaterial(m_pSharedMtrl);
	}
}

CRenderComponent::~CRenderComponent()
{
	if (nullptr != m_pDynamicMtrl)
		delete m_pDynamicMtrl.Get();
}



void CRenderComponent::SetSharedMaterial(Ptr<CMaterial> _pMtrl)
{
	m_pSharedMtrl = _pMtrl;
	m_pMtrl = m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_pMtrl = m_pSharedMtrl;

	return m_pMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (nullptr != m_pDynamicMtrl && m_pDynamicMtrl->GetMasterMtrl() != m_pSharedMtrl)
	{
		CMaterial* pMtrl = m_pDynamicMtrl.Get();
		m_pDynamicMtrl = nullptr;
		delete pMtrl;
	}

	if (nullptr == m_pDynamicMtrl)
	{
		m_pDynamicMtrl = m_pSharedMtrl->GetMtrlInst();
	}

	m_pMtrl = m_pDynamicMtrl;
	return m_pMtrl;
}


void CRenderComponent::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	SaveResPtr(m_pMesh, _pFile);
	SaveResPtr(m_pMtrl, _pFile);
}

void CRenderComponent::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	LoadResPtr(m_pMesh, _pFile);

	Ptr<CMaterial> pMtrl;
	LoadResPtr(pMtrl, _pFile);
	SetSharedMaterial(pMtrl);
}