#include "pch.h"
#include "CBasicBallScript.h"


#include <Engine/CTransform.h>
#include <Engine/CEventMgr.h>



CBasicBallScript::CBasicBallScript()
	: CScript((int)SCRIPT_TYPE::BASICBALLSCRIPT)
	, m_fSpeed(200.f)
	, m_fMaxTime(3.f)
	, m_fTimer(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));

}


CBasicBallScript::CBasicBallScript(const CBasicBallScript& _origin)
	: CScript((int)SCRIPT_TYPE::BASICBALLSCRIPT)
	, m_fSpeed(200.f)
	, m_fMaxTime(3.f)
	, m_fTimer(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CBasicBallScript::~CBasicBallScript()
{

}



void CBasicBallScript::start()
{
	m_fSpeed = 200.f;
	m_eDir = BALL_DIRECTION::LEFT;
	m_fMaxTime = 3.f;

}

void CBasicBallScript::update()
{
	CGameObject* pOwner = GetOwner();
	if (pOwner == nullptr)
		return;

	m_fTimer += DT;
	Vec3 vPos = pOwner->Transform()->GetRelativePos();
	
	if (m_eDir == BALL_DIRECTION::LEFT)
	{
		vPos.x -= DT * m_fSpeed;
	}
	else if (m_eDir == BALL_DIRECTION::RIGHT)
	{
		vPos.x += DT * m_fSpeed;

	}

	pOwner->Transform()->SetRelativePos(vPos);
}

void CBasicBallScript::lateupdate()
{

	// Delete Obj
	if (m_fTimer >= m_fMaxTime)
	{

		tEventInfo info = {};

		info.eType = EVENT_TYPE::DEREGISTER_OBJ_IN_LAYER;
		info.lParam = (DWORD_PTR)GetOwner();
		info.wParam = (DWORD_PTR)GetOwner()->GetLayerIndex();

		CEventMgr::GetInst()->AddEvent(info);

		GetOwner()->Destroy();
		return;

	}

}

void CBasicBallScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CBasicBallScript::OnCollision(CGameObject* _OtherObject)
{
}

void CBasicBallScript::OnCollisionExit(CGameObject* _OtherObject)
{
}



void CBasicBallScript::SaveToScene(FILE* _pFile)
{
}

void CBasicBallScript::LoadFromScene(FILE* _pFile)
{
}

