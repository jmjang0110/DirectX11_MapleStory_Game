#include "pch.h"
#include "CCamera.h"

#include "CTransform.h"
#include "CDevice.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
{

}

CCamera::~CCamera()
{

}

void CCamera::finalupdate()
{
	// View 행렬 계산
	Vec3 vCamPos = Transform()->GetPos();
	m_matView = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 투영행렬 계산
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	m_matProj = XMMatrixOrthographicLH(vRenderResolution.x, vRenderResolution.y, 0.f, 5000.f);	

	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;
}