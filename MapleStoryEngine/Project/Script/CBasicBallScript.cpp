#include "pch.h"
#include "CBasicBallScript.h"


#include <Engine/CTransform.h>
#include <Engine/CEventMgr.h>



CBasicBallScript::CBasicBallScript()
	: CScript((int)SCRIPT_TYPE::BASICBALLSCRIPT)
	, m_fSpeed(200.f)
	, m_fMaxTime(3.f)
	, m_fTimer(0.f)
	, m_fAngle(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));

}


CBasicBallScript::CBasicBallScript(const CBasicBallScript& _origin)
	: CScript((int)SCRIPT_TYPE::BASICBALLSCRIPT)
	, m_fSpeed(200.f)
	, m_fMaxTime(3.f)
	, m_fTimer(0.f)
	, m_fAngle(0.f)
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
	
	// Move 
	switch (m_eDir)
	{
	case BALL_DIRECTION::LEFT:
	{
		vPos.x -= DT * m_fSpeed;

	}
		break;
	case BALL_DIRECTION::RIGHT:
	{
		vPos.x += DT * m_fSpeed;

	}
		break;
	case BALL_DIRECTION::LEFT_DIAGONAL:
		break;
	case BALL_DIRECTION::RIGHT_DIAGONAL:
		break;
	case BALL_DIRECTION::LEFT_PARABOLA:
		break;
	case BALL_DIRECTION::RIGHT_PARABOLA:
		break;
	}


	pOwner->Transform()->SetRelativePos(vPos);
}

void CBasicBallScript::lateupdate()
{

	// Delete Obj
	if (m_fTimer >= m_fMaxTime)
	{
		// Event 
		CGameObject* pDelObj = GetOwner();
		int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

		CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
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

