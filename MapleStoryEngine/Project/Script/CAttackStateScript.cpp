#include "pch.h"
#include "CAttackStateScript.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CPrefab.h>
#include <Engine/CCollider2D.h>


#include "CAIScript.h"
#include "CIdleStateScript.h"
#include "CRigidBodyScript.h"
#include "CSceneSaveLoad.h"

#include "CSceneStartScript.h"
#include "CBasicBallScript.h"

#include "CLaunchSkillScript.h"
#include "CSkillnearScript.h"

#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"

#include "CMonsterScript.h"
#include "CBossMonsterScript.h"
#include "CAnimOnceScript.h"





CAttackStateScript::CAttackStateScript()
	: CStateScript((int)SCRIPT_TYPE::ATTACKSTATESCRIPT)
	, m_Skill1_Cnt(0)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::ATT);
}


CAttackStateScript::CAttackStateScript(const CAttackStateScript& _origin)
	: CStateScript((int)SCRIPT_TYPE::ATTACKSTATESCRIPT)
	, m_Skill1_Cnt(0)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::ATT);
}

CAttackStateScript::~CAttackStateScript()
{


}



void CAttackStateScript::Enter()
{
	// Random Set Att State 
	UINT iRandomAtt = rand() % (UINT)BOSS_ATTACK_STATE::END;
	m_eCurAttState = (BOSS_ATTACK_STATE)iRandomAtt;

	// ** 임의로 조정 ** 지워도 됨 
	if (m_eCurAttState == BOSS_ATTACK_STATE::NORMAL_ATTACK1)
		m_eCurAttState = BOSS_ATTACK_STATE::SKILL_ATTACK6;
	else if (m_eCurAttState == BOSS_ATTACK_STATE::NORMAL_ATTACK3)
		m_eCurAttState = BOSS_ATTACK_STATE::SKILL_ATTACK5;




	// == ========================
	m_fTimer = 0.f;
	m_bAttackEnd = false;

	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	if (pBossScript != nullptr)
	{
		pBossScript->SetBossAttState(m_eCurAttState);
		if (m_eCurAttState == BOSS_ATTACK_STATE::SKILL_ATTACK6)
		{
			Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Papulatus\\Skill3.mp3", L"sound\\Papulatus\\Skill3.mp3");
			pBgm->Play(1, 1.f, true);

		}

		if (m_eCurAttState == BOSS_ATTACK_STATE::SKILL_ATTACK5)
		{
			Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Papulatus\\Skill2", L"sound\\Papulatus\\Skill2.mp3");
			pBgm->Play(1, 1.f, true);

		}
	}

	m_bAddSubSkill = false;
	m_bHitPlayer = false;

}

void CAttackStateScript::Exit()
{
	m_fTimer = 0.f;
	m_Skill1_Cnt = 0;

}

void CAttackStateScript::start()
{
	m_Skill1_Cnt = 0;

}

void CAttackStateScript::update()
{
	m_fTimer += DT;


	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	if (pBossScript != nullptr)
		UpdateBossMobAttack();

	CMonsterScript* pMobScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");
	if (pMobScript != nullptr)
		UpdateMonsterAttack();

}

void CAttackStateScript::lateupdate()
{




}

void CAttackStateScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CBasicBallScript* pBasicBallScript = (CBasicBallScript*)_OtherObject->GetScriptByName(L"CBasicBallScript");
	CSkillnearScript* pSkillNearScript = (CSkillnearScript*)_OtherObject->GetScriptByName(L"CSkillnearScript");

	if (pBasicBallScript != nullptr ||
		pSkillNearScript != nullptr)
	{

		GetAI()->ChangeState(MONSTER_STATE::HIT);

	}

}

void CAttackStateScript::OnCollision(CGameObject* _OtherObject)
{
}

void CAttackStateScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CAttackStateScript::SaveToScene(FILE* _pFile)
{
}

void CAttackStateScript::LoadFromScene(FILE* _pFile)
{
}



void CAttackStateScript::AddPapulatusSkill(BOSS_ATTACK_STATE _eState)
{
	if (m_bAddSubSkill == true)
		return;


	CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"PapulatusSkill");
	CGameObject* NewObj = pPrefab->Instantiate();
	NewObj->Transform()->SetIgnoreParentScale(true);

	CAnimOnceScript* pAnimOnceScript = (CAnimOnceScript*)CScriptMgr::GetScript(L"CAnimOnceScript");
	NewObj->AddComponent((CComponent*)pAnimOnceScript);

	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	BOSS_DIRECTION eDir = pBossScript->GetDir();


	switch (_eState)
	{
		case BOSS_ATTACK_STATE::SKILL_ATTACK1:
		{
			if (eDir == BOSS_DIRECTION::LEFT)
			{
				NewObj->Transform()->SetRelativePos(Vec3(-105.f, -107.f, -5.f));
				NewObj->Animator2D()->Play(L"SKILL1_LEFT",false);

			}
			else if (eDir == BOSS_DIRECTION::RIGHT)
			{
				NewObj->Transform()->SetRelativePos(Vec3(105.f, -107.f, -5.f));
				NewObj->Animator2D()->Play(L"SKILL1_RIGHT", false);

			}

		}
			break;
		case BOSS_ATTACK_STATE::SKILL_ATTACK2:
		{
			if (eDir == BOSS_DIRECTION::LEFT)
			{
				NewObj->Transform()->SetRelativePos(Vec3(-105.f, -107.f, -5.f));

				NewObj->Animator2D()->Play(L"SKILL2_LEFT", false);

			}
			else if (eDir == BOSS_DIRECTION::RIGHT)
			{
				NewObj->Transform()->SetRelativePos(Vec3(105.f, -107.f, -5.f));

				NewObj->Animator2D()->Play(L"SKILL2_RIGHT", false);

			}

		}
			break;
		case BOSS_ATTACK_STATE::SKILL_ATTACK3:
		{
			if (eDir == BOSS_DIRECTION::LEFT)
			{
				NewObj->Transform()->SetRelativePos(Vec3(-105.f, -107.f, -5.f));

				NewObj->Animator2D()->Play(L"SKILL3_LEFT", false);

			}
			else if (eDir == BOSS_DIRECTION::RIGHT)
			{
				NewObj->Transform()->SetRelativePos(Vec3(105.f, -107.f, -5.f));

				NewObj->Animator2D()->Play(L"SKILL3_RIGHT", false);

			}

		}
			break;
		case BOSS_ATTACK_STATE::SKILL_ATTACK4:
		{
			if (eDir == BOSS_DIRECTION::LEFT)
			{
				NewObj->Transform()->SetRelativePos(Vec3(-105.f, 0.f, -5.f));

				NewObj->Animator2D()->Play(L"SKILL4_LEFT", false);

			}
			else if (eDir == BOSS_DIRECTION::RIGHT)
			{
				NewObj->Transform()->SetRelativePos(Vec3(105.f, 0.f, -5.f));

				NewObj->Animator2D()->Play(L"SKILL4_RIGHT", false);

			}

		}
			break;
			
	}
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(NewObj, GetOwner()->GetLayerIndex());
	CSceneMgr::GetInst()->AddChild(GetOwner(), NewObj);

	m_bAddSubSkill = true;


}

void CAttackStateScript::ShootPlayer(BOSS_ATTACK_STATE _eState)
{
	if (m_bHitPlayer == true)
		return;

	CGameObject* pPlayer = CSceneSaveLoad::pMainPlayer;


	// Player 가 공격 범위 내에 있다면 
	Vec3 vBossPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();
	Vec2 vDiff = Vec2(vPlayerPos.x - vBossPos.x, vPlayerPos.y - vBossPos.y);
	float fLen = vDiff.Length();

	CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"PapulatusSkillHit");
	CGameObject* NewObj = pPrefab->Instantiate();
	vPlayerPos.z -= 10.f;
	NewObj->Transform()->SetRelativePos(vPlayerPos);


	CAnimOnceScript* pAnimOnceScript = (CAnimOnceScript*)CScriptMgr::GetScript(L"CAnimOnceScript");
	NewObj->AddComponent((CComponent*)pAnimOnceScript);
	switch (_eState)
	{
	case BOSS_ATTACK_STATE::SKILL_ATTACK1:
	{

		NewObj->Animator2D()->Play(L"SKILL1_HIT", false);
		Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Papulatus\\Attack1.mp3", L"sound\\Papulatus\\Attack1.mp3");
		pBgm->Play(1, 1.f, true);


	}
	break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK2:
	{
		NewObj->Animator2D()->Play(L"SKILL2_HIT", false);
		Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Papulatus\\Attack1.mp3", L"sound\\Papulatus\\Attack1.mp3");
		pBgm->Play(1, 1.f, true);


	}
	break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK3:
	{
		NewObj->Animator2D()->Play(L"SKILL3_HIT", false);
		Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Papulatus\\Attack3.mp3", L"sound\\Papulatus\\Attack3.mp3");
		pBgm->Play(1, 1.f, true);


	}
	break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK4:
	{
		NewObj->Animator2D()->Play(L"SKILL4_HIT", false);
		Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Papulatus\\Attack3.mp3", L"sound\\Papulatus\\Attack3.mp3");
		pBgm->Play(1, 1.f, true);


	}
	break;
	}

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(NewObj, L"SubSkill_2");
	m_bHitPlayer = true;

}

void CAttackStateScript::UpdateBossMobAttack()
{
	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	CGameObject* pPlayer = CSceneSaveLoad::pMainPlayer;
	CRigidBodyScript* pPlayerRigidBody = (CRigidBodyScript*)pPlayer->GetScriptByName(L"CRigidBodyScript");

	switch (m_eCurAttState)
	{
	case BOSS_ATTACK_STATE::NORMAL_ATTACK1:
	{

	}
	break;
	case BOSS_ATTACK_STATE::NORMAL_ATTACK2:
	{

	}
		break;
	case BOSS_ATTACK_STATE::NORMAL_ATTACK3:
	{

	}
		break;
	case BOSS_ATTACK_STATE::NORMAL_ATTACK4:
	{

	}
		break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK1:
	{

		AddPapulatusSkill(m_eCurAttState);
		ShootPlayer(m_eCurAttState);
	

	}
		break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK2:
	{

		AddPapulatusSkill(m_eCurAttState);
		ShootPlayer(m_eCurAttState);

	}
		break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK3:
	{

		AddPapulatusSkill(m_eCurAttState);
		ShootPlayer(m_eCurAttState);

	}
		break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK4:
	{
		AddPapulatusSkill(m_eCurAttState);
		ShootPlayer(m_eCurAttState);

	}
		break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK5:
	{

		if (m_fTimer >= 1.f)
		{
	
			if (m_ePrevAttState != m_eCurAttState)
			{

				// 밀기 
				Vec3 vMaxVelocity = pPlayerRigidBody->GetMaxVelocity();
				if (pBossScript->GetDir() == BOSS_DIRECTION::RIGHT)
				{
					pPlayerRigidBody->SetAccelRatio(1.f);
					pPlayerRigidBody->AddVelocity(Vec3(1000.f, 0.f, 0.f));
					vMaxVelocity.x = 1000.f;
				}
				else if (pBossScript->GetDir() == BOSS_DIRECTION::LEFT)
				{
					pPlayerRigidBody->SetAccelRatio(1.f);

					pPlayerRigidBody->AddVelocity(Vec3(-1000.f, 0.f, 0.f));
					vMaxVelocity.x = -1000.f;
				}
				pPlayerRigidBody->SetMaxVelocity(vMaxVelocity);
			}
			m_ePrevAttState = m_eCurAttState;

		}

	}
	break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK6:
	{
		if (m_Skill1_Cnt <= 5)
		{
			m_Skill1_Cnt++;
			Vec3 vPos = GetOwner()->Collider2D()->GetWorldPos();

			int randomX = rand() % 500;
			if (randomX % 2 == 1)
				randomX *= -1;

			vPos.x += randomX;

			CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"TimeOutSkill");

			// Prefab 파일에 저장된 gameObject 를 읽어서 해당 Layer 에 포함한다. 
			CGameObject* NewObj = pPrefab->Instantiate();
			NewObj->AddComponent((CComponent*)CScriptMgr::GetScript(L"CRigidBodyScript"));
			NewObj->AddComponent((CComponent*)CScriptMgr::GetScript(L"CGravityScript"));
			NewObj->AddComponent((CComponent*)CScriptMgr::GetScript(L"CLaunchSkillScript"));

			CLaunchSkillScript* pLaunchScript = (CLaunchSkillScript*)NewObj->GetScriptByName(L"CLaunchSkillScript");
			pLaunchScript->UseGround(true);

			Vec3 SkillPos = NewObj->Transform()->GetRelativePos();
			Vec3 Scale = NewObj->Transform()->GetRelativeScale();

			SkillPos = vPos;
			SkillPos.y += Scale.y / 2.f + 50.f;
			NewObj->Transform()->SetRelativePos(SkillPos);

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			CLayer* pLayer = pCurScene->GetLayer(L"SubSkill_2");

			pCurScene->AddObject(NewObj, pLayer->GetName());
		}
		

	}
	break;



	}



	m_bAttackEnd = pBossScript->GetAttackEnd();

	if (m_bAttackEnd == true)
	{
		m_ePrevAttState = m_eCurAttState;

		if (m_eCurAttState == BOSS_ATTACK_STATE::SKILL_ATTACK5)
		{

			Vec3 vMaxVelocity = pPlayerRigidBody->GetMaxVelocity();
			vMaxVelocity.x = 150.f;
			pPlayerRigidBody->SetMaxVelocity(vMaxVelocity);

		}
		GetAI()->ChangeState(MONSTER_STATE::IDLE);

	}

	tBossMonInfo tBossInfo = pBossScript->GetMonsterInfo();
	if (tBossInfo.fHP <= 0.f)
	{
		GetAI()->ChangeState(MONSTER_STATE::DEAD);
	}



}

void CAttackStateScript::UpdateMonsterAttack()
{




	CMonsterScript* pMobScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();


	// Ball Shoot 
	if (m_Skill1_Cnt < 1)
	{
		m_Skill1_Cnt++;

		if (GetOwner()->GetName() == L"GiganticBiking")
		{
			Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\GiganticBiking\\Attack1.mp3", L"sound\\GiganticBiking\\Attack1.mp3");
			pBgm->Play(1, 1.f, true);
			ShootGiganticBall();
		}

		if (GetOwner()->GetName() == L"Tanatos")
		{
			Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Tanatos\\Skill1.mp3", L"sound\\Tanatos\\Skill1.mp3");
			pBgm->Play(1, 1.f, true);
			ShootTanatosBall();
		}

	

	}
	


	// Change State
	m_bAttackEnd = pMobScript->GetAttackEnd();
	if (m_bAttackEnd == true)
	{
		GetAI()->ChangeState(MONSTER_STATE::IDLE);
	}

	tMonsterInfo tMobInfo = pMobScript->GetMonsterInfo();
	if (tMobInfo.fHP <= 0.f)
	{
		GetAI()->ChangeState(MONSTER_STATE::DEAD);

	}



}

void CAttackStateScript::ShootGiganticBall()
{
	CMonsterScript* pMobScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");

	CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"GiganticBall");
	CGameObject* pBall = pPrefab->Instantiate();


	CBasicBallScript* pBallScript = (CBasicBallScript*)CScriptMgr::GetScript(L"CBasicBallScript");
	pBallScript->SetBallMoveType(BALLMOVE_TYPE::LINEAR);


	Vec3 SkillPos = pBall->Transform()->GetRelativePos();
	Vec3 Scale = pBall->Transform()->GetRelativeScale();

	SkillPos = GetOwner()->Transform()->GetRelativePos(); // Get Pos
	

	if (pMobScript->GetDir() == MONSTER_DIRECTION::LEFT)
	{
		pBallScript->SetDir(BALL_DIRECTION::LEFT);
		SkillPos.y += -30.f;
		SkillPos.x += -57.f;
		SkillPos.z = -100.f;	// Setting StartPos 
		pBall->Animator2D()->Play(L"MOVE_LEFT", true);
	}
	else
	{
		pBallScript->SetDir(BALL_DIRECTION::RIGHT);
		SkillPos.y += -30.f;
		SkillPos.x += 57.f;
		SkillPos.z = -100.f;	// Setting StartPos 
		pBall->Animator2D()->Play(L"MOVE_RIGHT", true);

	}
	pBallScript->SetMaxTime(2.f);
	pBallScript->SetSpeed(200.f);
	pBallScript->SetMaxAttack(1000.f);
	pBallScript->SetMinAttack(500.f);

	pBall->Transform()->SetRelativePos(SkillPos);	// Set pos
	pBall->AddComponent((CComponent*)pBallScript);	// Set Script

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	pCurScene->AddObject(pBall, L"SubSkill_2");

}

void CAttackStateScript::ShootTanatosBall()
{
	CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"TanatosBall");
	CGameObject* pBall = pPrefab->Instantiate();

	CBasicBallScript* pBallScript = (CBasicBallScript*)CScriptMgr::GetScript(L"CBasicBallScript");
	Vec3 vpos = CSceneSaveLoad::pMainPlayer->Transform()->GetRelativePos();

	pBall->Transform()->SetRelativePos(CSceneSaveLoad::pMainPlayer->Transform()->GetRelativePos());

	pBallScript->SetBallMoveType(BALLMOVE_TYPE::STAND);
	pBallScript->SetStartPos(CSceneSaveLoad::pMainPlayer->Transform()->GetRelativePos());
	pBallScript->SetMaxTime(0.6f);
	pBallScript->SetSpeed(0.f);
	pBallScript->SetMaxAttack(1500.f);
	pBallScript->SetMinAttack(1000.f);

	pBall->AddComponent((CComponent*)pBallScript);	// Set Script
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(pBall, L"SubSkill_2");


}
