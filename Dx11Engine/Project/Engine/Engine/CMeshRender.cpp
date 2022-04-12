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

CMeshRender::~CMeshRender()
{
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
