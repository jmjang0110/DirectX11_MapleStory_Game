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
#include "CSkillnearScript.h"
#include "CSceneStartScript.h"

#include "CSceneSaveLoad.h"
#include "CLaunchSkillScript.h"

#include "CSceneStartScript.h"

#include "CItemScript.h"
#include "CRigidBodyScript.h"
CMonsterScript::CMonsterScript()
	: CScript((int)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_vPrevPos(Vec3(0.f, 0.f, 0.f))
	, m_iHitCnt(0)
	, m_fTimer(0)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CMonsterScript::CMonsterScript(const CMonsterScript& _origin)
	: CScript((int)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_vPrevPos(Vec3(0.f, 0.f, 0.f))
	, m_fTimer(0)

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

			// Drop Item 
			// test 
			CPrefab* pPrefab2 = nullptr;
			wstring portionName = L"";
			wstring MonsterName = GetOwner()->GetName();

			if (MonsterName == L"Tanatos")
				portionName = L"PowerElixirPortion";
			else if (MonsterName == L"GiganticBiking")
				portionName = L"ElixirPortion";
			
			
			pPrefab2 = CSceneSaveLoad::pSceneMgrScript->GetPrefab(portionName);
			CGameObject* pObj = pPrefab2->Instantiate();
			pObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());

			pObj->AddComponent((CComponent*)CScriptMgr::GetScript(L"CGravityScript"));
			pObj->AddComponent((CComponent*)CScriptMgr::GetScript(L"CRigidBodyScript"));
			CItemScript* pItemScript = (CItemScript*)pObj->GetScriptByName(L"CItemScript");
			CRigidBodyScript* pRigidbody = (CRigidBodyScript*)pObj->GetScriptByName(L"CRigidBodyScript");

			pItemScript->m_eSpotType = ITEM_SPOT::LAND;


			// Item Layer - Add Object 
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			pCurScene->AddObject(pObj, L"Item");

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
		// ATTACK_LEFT -> ATTACK1_LEFT 로 잘못지음.. 그냥 쓸래 귀찮아. 
		if (m_eDir == MONSTER_DIRECTION::LEFT)
			pAnimator2D->Play(L"ATTACK1_LEFT", false);
		else if (m_eDir == MONSTER_DIRECTION::RIGHT)
			pAnimator2D->Play(L"ATTACK1_RIGHT", false);
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

	case MONSTER_STATE::HIT:
	{
		if (m_eDir == MONSTER_DIRECTION::LEFT)
			pAnimator2D->Play(L"HIT_LEFT", true);
		else if (m_eDir == MONSTER_DIRECTION::RIGHT)
			pAnimator2D->Play(L"HIT_RIGHT", true);
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
		m_pAI->ChangeState(MONSTER_STATE::HIT);


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

			CPlayerScript* pPlayerScript = (CPlayerScript*)CSceneSaveLoad::pMainPlayer->GetScriptByName(L"CPlayerScript");
			pPlayerScript->AddExp(1000.f);

			
			if (GetOwner()->GetName() == L"GiganticBiking")
			{
				Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\GiganticBiking\\Die.mp3", L"sound\\GiganticBiking\\Die.mp3");
				pBgm->Play(1, 0.6f, true);
			}
			else if (GetOwner()->GetName() == L"Tanatos")
			{
				Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Tanatos\\Die.mp3", L"sound\\Tanatos\\Die.mp3");
				pBgm->Play(1, 0.6f, true);

			}
		}

		if (GetOwner()->GetName() == L"GiganticBiking")
		{
			Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\GiganticBiking\\Damage.mp3", L"sound\\GiganticBiking\\Damage.mp3");
			pBgm->Play(1, 0.6f, true);
		}
		else if (GetOwner()->GetName() == L"Tanatos")
		{
			Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Tanatos\\Damage.mp3", L"sound\\Tanatos\\Damage.mp3");
			pBgm->Play(1, 0.6f, true);

		}


		CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"Damage");

		CGameObject* pDamageObj = pPrefab->Instantiate();
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		vPos.y += (m_iHitCnt * OffsetY);

		pDamageObj->Transform()->SetRelativePos(vPos);
		CDamageScript* pDamageScript = (CDamageScript*)CScriptMgr::GetScript(L"CDamageScript");
		pDamageObj->AddComponent((CComponent*)pDamageScript);
		pDamageScript->Init(DAMAGE_TYPE::CRITICAL_1, damage, 2.f);
		pCurScene->AddObject(pDamageObj, L"Damage");


		// Hit Animation (Ball) 
		pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"StormHit");

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

	// Near Skill
	CSkillnearScript* pSkillNearScript = (CSkillnearScript*)_OtherObject->GetScriptByName(L"CSkillnearScript");
	if (pSkillNearScript != nullptr)
	{
		//pSkillNearScript->SetHitObjAddress(GetOwner());

		if (pSkillNearScript->IsAddMonsterHitPossible() == true)
		{
		pSkillNearScript->AddMonsterCnt();
		pSkillNearScript->AddHitMonster(GetOwner());
		m_iHitNumber = pSkillNearScript->GetMonsterCnt();

		}


	}
}

void CMonsterScript::OnCollision(CGameObject* _OtherObject)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	CSkillnearScript* pSkillNearScript = (CSkillnearScript*)_OtherObject->GetScriptByName(L"CSkillnearScript");
	if (pSkillNearScript == nullptr)
		return;

	if (pSkillNearScript->GetHitMonseterObjAddress(m_iHitNumber) != GetOwner())
		return;

	//if (pSkillNearScript->GetHitObjAddress() != GetOwner())
	//	return;

	if (pSkillNearScript != nullptr)
	{
		if (pSkillNearScript->GetTimer() >= 0.4f)
		{
			if (pSkillNearScript->GetAttackCnt() < pSkillNearScript->GetMaxAttackCnt())
			{
				int iAttackCnt = pSkillNearScript->GetAttackCnt();
				pSkillNearScript->AddAttackCnt();

				float OffsetY = 60.f;
				if (iAttackCnt > 0)
					OffsetY = 35.f;

				// Damage 
				int MaxAttack = pSkillNearScript->GetMaxAttack();
				int MinAttack = pSkillNearScript->GetMinAttack();

				int randNum = rand() % (MaxAttack - MinAttack);
				int damage = MinAttack + randNum;

				m_tInfo.fHP -= damage;
				if (m_tInfo.fHP <= 0.f)
				{
					m_eCurStateType = MONSTER_STATE::DEAD;

					if (m_bDie == false)
					{
						CPlayerScript* pPlayerScript = (CPlayerScript*)CSceneSaveLoad::pMainPlayer->GetScriptByName(L"CPlayerScript");
						pPlayerScript->AddExp(1000.f);

					}					
					m_bDie = true;

					wstring name = GetOwner()->GetName();

					
					if (GetOwner()->GetName() == L"GiganticBiking")
					{
						Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\GiganticBiking\\Die.mp3", L"sound\\GiganticBiking\\Die.mp3");
						pBgm->Play(1, 0.6f, true);
					}
					else if (GetOwner()->GetName() == L"Tanatos")
					{
						Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Tanatos\\Die.mp3", L"sound\\Tanatos\\Die.mp3");
						pBgm->Play(1, 0.6f, true);

					}
			
				}
				if (GetOwner()->GetName() == L"GiganticBiking")
				{
					Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\GiganticBiking\\Damage.mp3", L"sound\\GiganticBiking\\Damage.mp3");
					pBgm->Play(1, 0.6f, true);
				}
				else if (GetOwner()->GetName() == L"Tanatos")
				{
					Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Tanatos\\Damage.mp3", L"sound\\Tanatos\\Damage.mp3");
					pBgm->Play(1, 0.6f, true);

				}

				pSkillNearScript->PlayHItBgm(1, 0.6f, true);
				CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"Damage");
				CGameObject* pDamageObj = pPrefab->Instantiate();

				Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
				vPos.y += (iAttackCnt * OffsetY);
				pDamageObj->Transform()->SetRelativePos(vPos);

				CDamageScript* pDamageScript = (CDamageScript*)CScriptMgr::GetScript(L"CDamageScript");
				pDamageObj->AddComponent((CComponent*)pDamageScript);
				pDamageScript->Init(DAMAGE_TYPE::CRITICAL_2, damage, 2.f);

				pCurScene->AddObject(pDamageObj, L"Damage");

				// Hit Animation (Ball) 
				pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"LargeSkillHit");

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

		
	}
	
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

