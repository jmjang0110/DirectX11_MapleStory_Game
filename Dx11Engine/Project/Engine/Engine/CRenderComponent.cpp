#include "pch.h"
#include "CRenderComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"
#include "CAnimator2D.h"


void CRenderComponent::render()
{
}

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
}

CRenderComponent::~CRenderComponent()
{
	if (nullptr != m_pDynamicMtrl)
		delete m_pDynamicMtrl.Get();
}



void CRenderComponent::finalupdate()
{
}

void CRenderComponent::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}


	Transform()->UpdateData();
	GetMesh()->UpdateData();
	GetMaterial()->render();


	if (Animator2D())
	{
		CAnimator2D::Clear();
	}
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
	if (nullptr == m_pDynamicMtrl)
	{
		m_pDynamicMtrl = m_pSharedMtrl->Clone();
	}

	m_pMtrl = m_pDynamicMtrl;
	return m_pMtrl;
}

