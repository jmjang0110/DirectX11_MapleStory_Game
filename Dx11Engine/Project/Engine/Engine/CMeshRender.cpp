#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"

CMeshRender::CMeshRender()
	: CComponent(COMPONENT_TYPE::MESHRENDER)
	, m_pMesh(nullptr)
	, m_pMtrl(nullptr)
{
}

CMeshRender::CMeshRender(const CMeshRender& _meshrender)
	: CComponent(COMPONENT_TYPE::MESHRENDER)
	, m_pMesh(_meshrender.m_pMesh)
	, m_pMtrl(nullptr)
	, m_pSharedMtrl(_meshrender.m_pMtrl)
	, m_pDynamicMtrl(nullptr)
{
	if (nullptr == _meshrender.m_pDynamicMtrl)
	{
		GetDynamicMaterial();
	}

	else if (nullptr == m_pSharedMtrl)
	{
		SetSharedMaterial(m_pSharedMtrl);

	}


}

CMeshRender::~CMeshRender()
{
	if (nullptr != m_pDynamicMtrl)
		delete m_pDynamicMtrl.Get();

}


void CMeshRender::finalupdate()
{
}

void CMeshRender::render()
{
	if (nullptr == m_pMesh || nullptr == m_pMtrl)
		return;

	Transform()->UpdateData();
	m_pMtrl->UpdateData();
	m_pMesh->render();
}


void CMeshRender::SetSharedMaterial(Ptr<CMaterial> _pMtrl)
{
	m_pSharedMtrl = _pMtrl;
	m_pMtrl = m_pSharedMtrl;


}

Ptr<CMaterial> CMeshRender::GetSharedMaterial()
{
	// 물체가 공유재질을 사용하겠다라는 의미이기도 하다. 
	m_pMtrl = m_pSharedMtrl;


	return m_pMtrl;

}

Ptr<CMaterial> CMeshRender::GetDynamicMaterial()
{
	if (nullptr == m_pDynamicMtrl)
	{
		m_pDynamicMtrl = m_pSharedMtrl->Clone();
	}

	m_pMtrl = m_pDynamicMtrl;

	return m_pMtrl;

}

