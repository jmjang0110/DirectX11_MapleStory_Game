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
	Vec3 vPos = Transform()->GetPos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * m_fCamSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * m_fCamSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * m_fCamSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * m_fCamSpeed;
	}

	Transform()->SetPos(vPos);
}
