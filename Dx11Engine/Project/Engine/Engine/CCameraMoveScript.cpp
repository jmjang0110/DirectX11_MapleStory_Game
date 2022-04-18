#include "pch.h"
#include "CCameraMoveScript.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CTransform.h"

CCameraMoveScript::CCameraMoveScript()
	: m_fCamSpeed(200.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::update()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{

		Vec3 vFront = Transform()->GetWorldFrontDir();
		vPos += DT * vFront * m_fCamSpeed;

	}

	if (KEY_PRESSED(KEY::S))
	{
		Vec3 vFront = Transform()->GetWorldFrontDir();
		vPos -= DT * vFront * m_fCamSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		Vec3 vRight = Transform()->GetWorldRightDir();
		vPos -= DT * vRight * m_fCamSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		Vec3 vRight = Transform()->GetWorldRightDir();
		vPos += DT *  vRight * m_fCamSpeed;
	}

	if (KEY_PRESSED(KEY::Y))
	{

		Vec3 vRot = Transform()->GetRelativeRotation();

	}

	Transform()->SetPos(vPos);
}
