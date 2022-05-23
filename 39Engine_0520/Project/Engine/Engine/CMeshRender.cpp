#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
	
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
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	Transform()->UpdateData();
	GetMaterial()->UpdateData();
	GetMesh()->render();


	if (Animator2D())
	{
		CAnimator2D::Clear();
	}
}


