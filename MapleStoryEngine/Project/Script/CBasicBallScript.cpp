#include "pch.h"
#include "CBasicBallScript.h"


#include <Engine/CTransform.h>
#include <Engine/CEventMgr.h>

#include "CRigidBodyScript.h"
#include "CGravityScript.h"

#include "CBossMonsterScript.h"
#include "CMonsterScript.h"

#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"


CBasicBallScript::CBasicBallScript()
	: CScript((int)SCRIPT_TYPE::BASICBALLSCRIPT)
	, m_fSpeed(600.f)
	, m_fMaxTime(1.f)
	, m_fTimer(0.f)
	, m_fRadian(0.f)
	, m_eMoveType(BALLMOVE_TYPE::LINEAR)
	, m_bStart(true)
	, m_bHit(false)

{
	SetName(CScriptMgr::GetScriptName(this));

}


CBasicBallScript::CBasicBallScript(const CBasicBallScript& _origin)
	: CScript((int)SCRIPT_TYPE::BASICBALLSCRIPT)
	, m_fSpeed(600.f)
	, m_fMaxTime(1.f)
	, m_fTimer(0.f)
	, m_fRadian(0.f)
	, m_eMoveType(BALLMOVE_TYPE::LINEAR)
	, m_bStart(true)
	, m_bHit(false)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CBasicBallScript::~CBasicBallScript()
{


}




void CBasicBallScript::Init(Vec3 _startpos)
{
	if (GetOwner() == nullptr)
		return;


	//if(m_pKeyDownBgm == nullptr)
	//	m_pKeyDownBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Storm\\Use.mp3", L"sound\\Storm\\Use.mp3");

	//m_iChannelIdx = m_pKeyDownBgm->Play(1, 0.5f, true);


	if (m_eMoveType == BALLMOVE_TYPE::DIAGONAL)
	{
		CRigidBodyScript* pRigidScript = (CRigidBodyScript*)CScriptMgr::GetScript(L"CRigidBodyScript");
		CGravityScript* pGravityScript = (CGravityScript*)CScriptMgr::GetScript(L"CGravityScript");
		GetOwner()->AddComponent((CComponent*)pRigidScript);
		GetOwner()->AddComponent((CComponent*)pGravityScript);

		if (m_eDir == BALL_DIRECTION::RIGHT)
		{
			pRigidScript->AddVelocity(Vec3(700.f, 500.f, 0.f));
			pRigidScript->SetMaxVelocity(Vec3(1000.f, 500.f, 0.f));
		}
		else if (m_eDir == BALL_DIRECTION::LEFT)
		{
			pRigidScript->AddVelocity(Vec3(-700.f, 500.f, 0.f));
			pRigidScript->SetMaxVelocity(Vec3(1000.f, 500.f, 0.f));
		}


		// 시작위치 조정 

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		vPos.y += 70.f;
		GetOwner()->Transform()->SetRelativePos(vPos);
	
	}

	m_vStartPos = _startpos;
	m_vPrevPos = _startpos;



}

void CBasicBallScript::start()
{
	m_fSpeed = 500.f;
	m_eDir = BALL_DIRECTION::LEFT;
	m_fMaxTime = 1.f;



}

void CBasicBallScript::update()
{
	m_fTimer += DT;

	switch (m_eMoveType)
	{
	case BALLMOVE_TYPE::LINEAR:
	{
		LinearMove();
	}
		break;
	case BALLMOVE_TYPE::DIAGONAL:
	{
		DiagonalMove();

	}
		break;
	case BALLMOVE_TYPE::PARABOLA:
	{
		ParabolaMove();

	}
		break;
	case BALLMOVE_TYPE::BALLISTIC:
	{
		BallisticMove();

	}
		break;

	case BALLMOVE_TYPE::STAND:
	{
		StandMove();

	}

		break;


	}

	m_vPrevPos = GetOwner()->Transform()->GetRelativePos();

}



void CBasicBallScript::lateupdate()
{
	// HIt Monster Success - Delete Obj 
	if (m_bHit == true)
	{
		// Event 
		CGameObject* pDelObj = GetOwner();
		int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

		CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
		GetOwner()->Destroy();
		return;
	}

	// TImeOut - Delete Obj
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


void CBasicBallScript::SetHitBgm(wstring _RelativePath)
{
	m_pHitSound = CResMgr::GetInst()->Load<CSound>(_RelativePath, _RelativePath);

}

void CBasicBallScript::LinearMove()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

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
	}


	GetOwner()->Transform()->SetRelativePos(vPos);

}

void CBasicBallScript::DiagonalMove()
{


	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vRot = GetOwner()->Transform()->GetRelativeRotation();


	Vec3 Vector1 = vPos - m_vPrevPos;
	Vector1.Normalize();

	Vec3 Vector2 = Vec3(1.f, 0.f, 0.f);
	if (m_eDir == BALL_DIRECTION::LEFT)
		Vector2 = Vec3(-1.f, 0.f, 0.f);
	Vector2.Normalize();

	float fDot = Vector1.Dot(Vector2);
	float fLength = Vector1.Length() * Vector2.Length();

	m_fRadian = acos(fDot / fLength);
	float degree = m_fRadian * 180.f / 3.141592f;


	if (m_bStart)
	{
		Vector1 = Vec3(700.f, 500.f, 0.f);
		if (m_eDir == BALL_DIRECTION::LEFT)
			Vector1.x *= -1;

		Vector1.Normalize();
		float fDot = Vector1.Dot(Vector2);
		float fLength = Vector1.Length() * Vector2.Length();

		m_fRadian = acos(fDot / fLength);
		m_bStart = false;
	}

	// Move 
	if (Vector1.y < 0.f)
		vRot.z = -m_fRadian;
	else
		vRot.z = m_fRadian;

	if (m_eDir == BALL_DIRECTION::LEFT)
		vRot.z *= -1;

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeRotation(vRot);



}

void CBasicBallScript::ParabolaMove()
{
}

void CBasicBallScript::BallisticMove()
{
}

void CBasicBallScript::StandMove()
{
	// PLayer 위치에 생성 
	GetOwner()->Transform()->SetRelativePos(m_vStartPos);

	


}



void CBasicBallScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	
	CMonsterScript* pMonsterScript = (CMonsterScript*)_OtherObject->GetScriptByName(L"CMonsterScript");
	CBossMonsterScript* pBossMonsterScript = (CBossMonsterScript*)_OtherObject->GetScriptByName(L"CBossMonsterScript");
	
	// Die  -> return 
	if (pMonsterScript != nullptr)
	{
		if (pMonsterScript->GetDie() == true)
			return;

		//if (m_pHitSound == nullptr)
		//	m_pHitSound = CResMgr::GetInst()->Load<CSound>(L"sound\\Storm\\Hit.mp3", L"sound\\Storm\\Hit.mp3");

		if (m_pHitSound != nullptr)
		{
			m_pHitSound->Play(1, 0.5f, true);
			CSound::UpdateFMOD();
		}


	}
	if (pBossMonsterScript != nullptr)
	{
		if (pBossMonsterScript->GetDie())
			return;
	}




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

