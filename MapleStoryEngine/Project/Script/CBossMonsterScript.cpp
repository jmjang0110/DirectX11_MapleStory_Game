#include "pch.h"
#include "CBossMonsterScript.h"
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
#include "CBossMonsterScript.h"
#include "CLaunchSkillScript.h"
#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"



CBossMonsterScript::CBossMonsterScript()
	: CScript((int)SCRIPT_TYPE::BOSSMONSTERSCRIPT)
	, m_fTimer(0.f)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CBossMonsterScript::CBossMonsterScript(const CBossMonsterScript& _origin)
	: CScript((int)SCRIPT_TYPE::BOSSMONSTERSCRIPT)
	, m_fTimer(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CBossMonsterScript::~CBossMonsterScript()
{

}

void CBossMonsterScript::start()
{

	CAIScript* pAI = (CAIScript*)GetOwner()->GetScriptByName(L"CAIScript");

	if (pAI != nullptr)
		m_pAI = pAI;


	// Monster Info Test
	m_tInfo.fRecogRange = 700.f;
	m_tInfo.fSpeed = 100.f;
	m_tInfo.fHP = 10000000.f * 0.4f;
	m_tInfo.fMaxHP = 10000000.f * 0.4f;


}

void CBossMonsterScript::update()
{
	m_fTimer += DT;

	if (m_fTimer >= 3.f)
	{
		m_fTimer = 0.f;
		m_iHitCnt = 0;
	}

	Update_State();
	Update_Move();
	Update_Gravity();
	Update_Animation();

	m_ePrevStateType = m_eCurStateType;
	m_ePrevDir = m_eDir;
	m_vPrevPos = GetOwner()->Transform()->GetRelativePos();

}

void CBossMonsterScript::lateupdate()
{
}


void CBossMonsterScript::Update_State()
{
	if (m_eCurStateType == MONSTER_STATE::DEAD && m_bDie == true)
	{

		CAnimator2D* pAnimator2D = GetOwner()->Animator2D();
		if (pAnimator2D->GetCurAnim()->IsFinish() == true)
		{
			// delete Obj 
			CSceneMgr::GetInst()->DeRegisterObjInLayer(GetOwner(), GetOwner()->GetLayerIndex());
			GetOwner()->Destroy();

			CPlayerScript* pPlayerScript = (CPlayerScript*)CSceneSaveLoad::pMainPlayer->GetScriptByName(L"CPlayerScript");
			pPlayerScript->AddExp(100000.f);

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
		SetDir(BOSS_DIRECTION::LEFT);
	else if (fDiff < 0)
		SetDir(BOSS_DIRECTION::RIGHT);
	else
		m_eDir = m_ePrevDir;


	m_ePrevDir = m_eDir;
}

void CBossMonsterScript::Update_Move()
{
}

void CBossMonsterScript::Update_Gravity()
{
}

void CBossMonsterScript::Update_Animation()
{
	if (m_pAI == nullptr)
		m_pAI = (CAIScript*)GetOwner()->GetScriptByName(L"CAIScript");

	m_eCurStateType = m_pAI->GetCurStateType();
	CAnimator2D* pAnimator2D = GetOwner()->Animator2D();

	if (pAnimator2D == nullptr)
		return;

	switch (m_eCurStateType)
	{
	case MONSTER_STATE::IDLE:
	{
		if (m_eDir == BOSS_DIRECTION::LEFT)
			pAnimator2D->Play(L"STAND_LEFT", true);
		else if (m_eDir == BOSS_DIRECTION::RIGHT)
			pAnimator2D->Play(L"STAND_RIGHT", true);
	}
		break;
	case MONSTER_STATE::PATROL:
	{

		if (m_eDir == BOSS_DIRECTION::LEFT)
			pAnimator2D->Play(L"MOVE_LEFT", true);
		else if (m_eDir == BOSS_DIRECTION::RIGHT)
			pAnimator2D->Play(L"MOVE_RIGHT", true);

	}
		break;
	case MONSTER_STATE::TRACE:
	{
		if (m_eDir == BOSS_DIRECTION::LEFT)
			pAnimator2D->Play(L"MOVE_LEFT", true);
		else if (m_eDir == BOSS_DIRECTION::RIGHT)
			pAnimator2D->Play(L"MOVE_RIGHT", true);
	}
		break;
	case MONSTER_STATE::ATT:
	{
		switch (m_eAttackState)
		{
		case BOSS_ATTACK_STATE::NORMAL_ATTACK1:
		{
			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"ATTACK1_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"ATTACK1_RIGHT", false);
		}
			break;
		case BOSS_ATTACK_STATE::NORMAL_ATTACK2:
		{
			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"ATTACK2_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"ATTACK2_RIGHT", false);
		}
			break;
		case BOSS_ATTACK_STATE::NORMAL_ATTACK3:
		{
			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"ATTACK3_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"ATTACK3_RIGHT", false);
		}
			break;
		case BOSS_ATTACK_STATE::NORMAL_ATTACK4:
		{
			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"ATTACK4_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"ATTACK4_RIGHT", false);
		}
			break;
		case BOSS_ATTACK_STATE::SKILL_ATTACK1:
		{
			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"SKILL1_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"SKILL1_RIGHT", false);
		}
			break;
		case BOSS_ATTACK_STATE::SKILL_ATTACK2:
		{
			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"SKILL2_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"SKILL2_RIGHT", false);
		}
			break;
		case BOSS_ATTACK_STATE::SKILL_ATTACK3:
		{
			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"SKILL3_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"SKILL3_RIGHT", false);
		}
			break;
		case BOSS_ATTACK_STATE::SKILL_ATTACK4:
		{
			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"SKILL4_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"SKILL4_RIGHT", false);
		}
			break;

		case BOSS_ATTACK_STATE::SKILL_ATTACK5:
		{
			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"SKILL5_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"SKILL5_RIGHT", false);
		}
			break;
		case BOSS_ATTACK_STATE::SKILL_ATTACK6:
		{
	

			if (m_eDir == BOSS_DIRECTION::LEFT)
				pAnimator2D->Play(L"SKILL6_LEFT", false);
			else if (m_eDir == BOSS_DIRECTION::RIGHT)
				pAnimator2D->Play(L"SKILL6_RIGHT", false);
		}
			break;

		}

	}
		break;
	case MONSTER_STATE::RUN:
		break;
	case MONSTER_STATE::DEAD:
	{
		if (m_eDir == BOSS_DIRECTION::LEFT)
			pAnimator2D->Play(L"DIE_LEFT", false);
		else if (m_eDir == BOSS_DIRECTION::RIGHT)
			pAnimator2D->Play(L"DIE_RIGHT", false);
	}
	break;
	}



	


}




void CBossMonsterScript::OnCollisionEnter(CGameObject* _OtherObject)
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

		if (m_eCurStateType == MONSTER_STATE::ATT)
		{
			if (m_eAttackState == BOSS_ATTACK_STATE::NORMAL_ATTACK1)
			{
				damage = 1;
			}
		}

		SubHP(damage);

		Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Papulatus\\Damage.mp3", L"sound\\Papulatus\\Damage.mp3");
		pBgm->Play(1, 1.f, true);


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

		// Hit Animation (Ball) 
		CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"StormHit");

		// CLaunchSkillScript : 시간 없어서 그냥 여기다가 응용해서 만들었음.. 
		CGameObject* pHitObj = pPrefab->Instantiate();
		CLaunchSkillScript* pLaunchSkillScript = (CLaunchSkillScript*)CScriptMgr::GetScript(L"CLaunchSkillScript");
		pLaunchSkillScript->SetHit(true);

		pHitObj->AddComponent((CComponent*)pLaunchSkillScript);
		vPos = GetOwner()->Transform()->GetRelativePos();
		vPos.z -= 10.f;
		pHitObj->Transform()->SetRelativePos(vPos);


		pCurScene->AddObject(pHitObj, L"SubSkill_1");




	}


}

void CBossMonsterScript::OnCollision(CGameObject* _OtherObject)
{
}

void CBossMonsterScript::OnCollisionExit(CGameObject* _OtherObject)
{
}





void CBossMonsterScript::SaveToScene(FILE* _pFile)
{
}

void CBossMonsterScript::LoadFromScene(FILE* _pFile)
{
}
