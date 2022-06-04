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



void CMeshRender::SetUVReverse(bool _reverse)
{
	int ImageReverse = 0;
	if (true == _reverse)
		ImageReverse = 1;
	else 
		ImageReverse = 0;

	GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &ImageReverse);
}

void CMeshRender::SetUVUpsideDown(bool _upsideDown)
{
	int ImageUpsideDown = 0;

	if (true == _upsideDown)
		ImageUpsideDown = 1;
	else
		ImageUpsideDown = 0;

	GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &ImageUpsideDown);

}