#include "pch.h"
#include "CMonsterScript.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CCollider2D.h>


#include "CAIScript.h"
#include "CTraceStateScript.h"
#include "CIdleStateScript.h"

#include "CBasicBallScript.h"
#include "CDamageScript.h"
#include "CPlayerScript.h"
#include "CMonsterFactoryScript.h"
#include "CMobGroundScript.h"


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

	// Monster Info Test
	m_tInfo.fRecogRange = 500.f;
	m_tInfo.fSpeed = 100.f;
	m_tInfo.fHP = 100000.f;

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
	if (m_eCurStateType == MONSTER_STATE::DEAD && m_bDie == true)
	{

		CAnimator2D* pAnimator2D = GetOwner()->Animator2D();
		if (pAnimator2D->GetCurAnim()->IsFinish() == true)
		{
			// delete Obj 
			CSceneMgr::GetInst()->DeRegisterObjInLayer(GetOwner(), GetOwner()->GetLayerIndex());
			GetOwner()->Destroy();

			if (m_pMobGround != nullptr)
				m_pMobGround->SubMonsterCnt();

		}
	}

	// Check Attack End 
	if (m_eCurStateType == MONSTER_STATE::ATT)
	{
		CAnimator2D* pAnimator2D = GetOwner()->Animator2D();
		if (pAnimator2D->GetCurAnim()->IsFinish() == true)
		{
			m_bAttackEnd = true;
			pAnimator2D->GetCurAnim()->Reset();
		}

	}
	else
		m_bAttackEnd = false;

	// Check Dir 
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	float fDiff = m_vPrevPos.x - vPos.x;

	if (fDiff > 0)
		SetDir(MONSTER_DIRECTION::LEFT);
	else if (fDiff < 0)
		SetDir(MONSTER_DIRECTION::RIGHT);
	else
		m_eDir = m_ePrevDir;


	m_ePrevDir = m_eDir;

}

void CMonsterScript::Update_Move()
{


}

void CMonsterScript::Update_Gravity()
{
}

void CMonsterScript::Update_Animation()
{

	if (m_pAI == nullptr)
		m_pAI = (CAIScript*)GetOwner()->GetScriptByName(L"CAIScript");

	m_eCurStateType = m_pAI->GetCurStateType();
	CAnimator2D* pAnimator2D = GetOwner()->Animator2D();

	if (pAnimator2D == nullptr)
		return;

	CAnimator2D* pAnimator = GetOwner()->Animator2D();

	if (pAnimator == nullptr)
		return;


	switch (m_eCurStateType)
	{
	case MONSTER_STATE::IDLE:
	{
		if (m_eDir == MONSTER_DIRECTION::LEFT)
			pAnimator2D->Play(L"STAND_LEFT", true);
		else if (m_eDir == MONSTER_DIRECTION::RIGHT)
			pAnimator2D->Play(L"STAND_RIGHT", true);
	}
	break;
	case MONSTER_STATE::PATROL:
	{

		if (m_eDir == MONSTER_DIRECTION::LEFT)
			pAnimator2D->Play(L"MOVE_LEFT", true);
		else if (m_eDir == MONSTER_DIRECTION::RIGHT)
			pAnimator2D->Play(L"MOVE_RIGHT", true);

	}
	break;
	case MONSTER_STATE::TRACE:
	{
		if (m_eDir == MONSTER_DIRECTION::LEFT)
			pAnimator2D->Play(L"MOVE_LEFT", true);
		else if (m_eDir == MONSTER_DIRECTION::RIGHT)
			pAnimator2D->Play(L"MOVE_RIGHT", true);
	}
	break;
	case MONSTER_STATE::ATT:
	{
		if (m_eDir == MONSTER_DIRECTION::LEFT)
			pAnimator2D->Play(L"ATTACK_LEFT", false);
		else if (m_eDir == MONSTER_DIRECTION::RIGHT)
			pAnimator2D->Play(L"ATTACK_RIGHT", false);
	}
		break;
	case MONSTER_STATE::RUN:
		break;
	case MONSTER_STATE::DEAD: 
	{
		if (m_eDir == MONSTER_DIRECTION::LEFT)
			pAnimator2D->Play(L"DIE_LEFT", false);
		else if (m_eDir == MONSTER_DIRECTION::RIGHT)
			pAnimator2D->Play(L"DIE_RIGHT", false);
	}
		break;
	}
}


void CMonsterScript::OnCollisionEnter(CGameObject* _OtherObject)
{

	if (m_eCurStateType == MONSTER_STATE::DEAD && m_bDie == true)
		return;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CBasicBallScript* pBallScript = (CBasicBallScript*)_OtherObject->GetScriptByName(L"CBasicBallScript");


	if (pBallScript != nullptr)
	{
		pBallScript->SetHit(true); // Next Frame - Delete Ball(Arrow)
		m_iHitCnt++;
		if (m_iHitCnt >= 10)
			m_iHitCnt = 0;


		float OffsetY = 60.f;
		if (m_iHitCnt > 0)
			OffsetY = 35.f;

		// Damage 
		int MaxAttack = pBallScript->GetMaxAttack();
		int MinAttack = pBallScript->GetMinAttack();

		int randNum = rand() % (MaxAttack - MinAttack);
		int damage = MinAttack + randNum;

	
		m_tInfo.fHP -= damage;
		if (m_tInfo.fHP <= 0.f)
		{
			m_eCurStateType = MONSTER_STATE::DEAD;
			m_bDie = true;

		}


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

