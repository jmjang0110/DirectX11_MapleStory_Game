#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CAIScript.h"
#include "CTraceStateScript.h"
#include "CIdleStateScript.h"

#include "CBasicBallScript.h"
#include "CDamageScript.h"
#include "CPlayerScript.h"



CMonsterScript::CMonsterScript()
	: CScript((int)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_vPrevPos(Vec3(0.f, 0.f, 0.f))
	, m_iHitCnt(0)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CMonsterScript::CMonsterScript(const CMonsterScript& _origin)
	: CScript((int)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_vPrevPos(Vec3(0.f, 0.f, 0.f))

{
	SetName(CScriptMgr::GetScriptName(this));

}

CMonsterScript::~CMonsterScript()
{
}





void CMonsterScript::start()
{
	CAIScript* pAI = (CAIScript*)GetOwner()->GetScriptByName(L"CAIScript");

	if (pAI != nullptr)
		m_pAI = pAI;

	srand((unsigned int)(NULL));
	int randDir = rand() % 2;

	if (randDir == 1)
		m_eDir = MONSTER_DIRECTION::LEFT;
	else
		m_eDir = MONSTER_DIRECTION::RIGHT;

	CGameObject* pOwner = GetOwner();
	if (pOwner != nullptr)
	{
		CAnimator2D* pAnimator = pOwner->Animator2D();
		if (pAnimator != nullptr)
		{
			if(m_eDir == MONSTER_DIRECTION::LEFT)
				pAnimator->Play(L"Stand_Left", true);
			else if (m_eDir == MONSTER_DIRECTION::RIGHT)
				pAnimator->Play(L"Stand_Right", true);
		}
	}


	// Monster Info Test
	m_tInfo.fRecogRange = 200.f;
	m_tInfo.fSpeed = 1.f;


}

void CMonsterScript::update()
{

	Update_State();
	Update_Move();
	Update_Gravity();
	Update_Animation();

	m_ePrevStateType = m_eCurStateType;
	m_ePrevDir = m_eDir;
	m_vPrevPos = GetOwner()->Transform()->GetRelativePos();

}

void CMonsterScript::lateupdate()
{
}



void CMonsterScript::Update_State()
{
	CGameObject* pOwner = GetOwner();
	if (pOwner == nullptr)
		return;

	Vec3 vPos = pOwner->Transform()->GetRelativePos();
	if (m_vPrevPos.x - vPos.x > 0)
		SetDir(MONSTER_DIRECTION::LEFT);
	else
		SetDir(MONSTER_DIRECTION::RIGHT);

}

void CMonsterScript::Update_Move()
{

	
}

void CMonsterScript::Update_Gravity()
{
}

void CMonsterScript::Update_Animation()
{



	CAIScript* pAI = (CAIScript*)GetOwner()->GetScriptByName(L"CAIScript");
	m_pAI = pAI;

	MONSTER_STATE eStateType = pAI->GetCurStateType();
	CGameObject* pOwner = GetOwner();

	m_eCurStateType = eStateType;

	if (pOwner == nullptr)
		return;

	CAnimator2D* pAnimator = pOwner->Animator2D();
	
	if (pAnimator == nullptr)
		return;

	switch (eStateType)
	{
	case MONSTER_STATE::IDLE:
	{
		if (m_eDir == MONSTER_DIRECTION::LEFT)
			pAnimator->Play(L"Stand_Left", true);
		else if (m_eDir == MONSTER_DIRECTION::RIGHT)
			pAnimator->Play(L"Stand_Right", true);
	}
		break;
	case MONSTER_STATE::PATROL:
		break;
	case MONSTER_STATE::TRACE:
	{
		if (m_eDir == MONSTER_DIRECTION::LEFT)
			pAnimator->Play(L"Move_Left", true);
		else if (m_eDir == MONSTER_DIRECTION::RIGHT)
			pAnimator->Play(L"Move_Right", true);
	}
		break;
	case MONSTER_STATE::ATT:
		break;
	case MONSTER_STATE::RUN:
		break;
	case MONSTER_STATE::DEAD:
		break;
	}
}


void CMonsterScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	
	CBasicBallScript* pBallScript = (CBasicBallScript*)_OtherObject->GetScriptByName(L"CBasicBallScript");
	if (pBallScript != nullptr)
	{
		pBallScript->SetHit(true); // Next Frame - Delete Ball(Arrow)
		m_iHitCnt++;
		if (m_iHitCnt >= 8)
			m_iHitCnt = 0;


		float OffsetY = 50.f;
		if (m_iHitCnt > 0)
			OffsetY = 35.f;

		// Damage 
		int damage = rand() % 100000; // Test 
		CGameObject* pDamageObj = new CGameObject;
		pDamageObj->SetName(L"damage");
		pDamageObj->AddComponent(new CTransform);
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		vPos.y += (m_iHitCnt * OffsetY);

		pDamageObj->Transform()->SetRelativePos(vPos);
		CDamageScript* pDamageScript = (CDamageScript*)CScriptMgr::GetScript(L"CDamageScript");
		pDamageObj->AddComponent((CComponent*)pDamageScript);
		pDamageScript->Init(DAMAGE_TYPE::CRITICAL_1, damage, 2.f);

		pCurScene->AddObject(pDamageObj, L"Damage");

	}
	
}

void CMonsterScript::OnCollision(CGameObject* _OtherObject)
{


}

void CMonsterScript::OnCollisionExit(CGameObject* _OtherObject)
{


}



void CMonsterScript::SaveToScene(FILE* _pFile)
{
}

void CMonsterScript::LoadFromScene(FILE* _pFile)
{
}

