#include "pch.h"
#include "CEditorCamScript.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>

CEditorCamScript::CEditorCamScript()
	: CScript(-1)
	, m_fSpeed(400.f)
{
}

CEditorCamScript::~CEditorCamScript()
{
}

void CEditorCamScript::update()
{
	Vec3 vPos = Transform()->GetRelativePos();

	float fSpeed = m_fSpeed;
	if (KEY_PRESSED(KEY::LSHFT))
		fSpeed *= 3.f;

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += fSpeed * DT;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= fSpeed * DT;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= fSpeed * DT;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += fSpeed * DT;
	}

	Transform()->SetRelativePos(vPos);


	if (KEY_PRESSED(KEY::_1))
	{
		Camera()->SetWidth(Camera()->GetWidth() - DT * 300.f);
	}

	if (KEY_PRESSED(KEY::_2))
	{
		Camera()->SetWidth(Camera()->GetWidth() + DT * 300.f);
	}


}
