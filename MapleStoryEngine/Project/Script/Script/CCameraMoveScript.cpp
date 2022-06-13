#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>

CCameraMoveScript::CCameraMoveScript()
	: CScript((int)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_Filter(nullptr)
	, m_fCamSpeed(500.f)
	
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::update()
{
	Vec3 vPos = Transform()->GetRelativePos();
	float fSpeed = m_fCamSpeed;

	if (KEY_PRESSED(KEY::LSHFT))
	{
		fSpeed *= 2.f;
	}


	if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
	{
		if (KEY_PRESSED(KEY::W))
		{
			Vec3 vFront = Transform()->GetWorldFrontDir();
			vPos += DT * vFront * fSpeed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			Vec3 vFront = Transform()->GetWorldFrontDir();
			vPos -= DT * vFront * fSpeed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			Vec3 vRight = Transform()->GetWorldRightDir();
			vPos -= DT * vRight * fSpeed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			Vec3 vRight = Transform()->GetWorldRightDir();
			vPos += DT * vRight * fSpeed;
		}
	}

	else
	{
		if (KEY_PRESSED(KEY::W))
		{			
			vPos.y += DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::S))
		{
			vPos.y -= DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::A))
		{
			vPos.x -= DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::D))
		{
			vPos.x += DT * fSpeed;
		}
	}

	if (nullptr != m_Filter && KEY_TAP(KEY::F))
	{
		if (m_Filter->IsActive())
			m_Filter->Deactivate();
		else
			m_Filter->Activate();
	}
	

	

	/*if (KEY_PRESSED(KEY::RBTN))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += DT * vMouseDir.x * XM_PI;
		vRot.x -= DT * vMouseDir.y * XM_PI;

		Transform()->SetRelativeRotation(vRot);
	}*/

	Transform()->SetRelativePos(vPos);
}
