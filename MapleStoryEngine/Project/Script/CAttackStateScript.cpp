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
	SAFE_DELETE(m_pSkillPrefab1);

}



void CAttackStateScript::Enter()
{
	// Random Set Att State 
	UINT iRandomAtt = rand() % (UINT)BOSS_ATTACK_STATE::END;
	m_eCurAttState = (BOSS_ATTACK_STATE)iRandomAtt;

	m_fTimer = 0.f;
	m_bAttackEnd = false;

	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	if (pBossScript != nullptr)
	{
		pBossScript->SetBossAttState(m_eCurAttState);

	}


}

void CAttackStateScript::Exit()
{
	m_fTimer = 0.f;
	m_Skill1_Cnt = 0;

}

void CAttackStateScript::start()
{
}

void CAttackStateScript::update()
{
	m_fTimer += DT;


	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	if (pBossScript != nullptr)
		UpdateBossMobAttack();

	
}

void CAttackStateScript::lateupdate()
{




}

void CAttackStateScript::OnCollisionEnter(CGameObject* _OtherObject)
{
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



void CAttackStateScript::UpdateBossMobAttack()
{
	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Player");
	CGameObject* pPlayer = pLayer->FindObj(L"player");
	CRigidBodyScript* pPlayerRigidBody = (CRigidBodyScript*)pPlayer->GetScriptByName(L"CRigidBodyScript");

	switch (m_eCurAttState)
	{
	case BOSS_ATTACK_STATE::NORMAL_ATTACK1:
	{

	}
	break;
	case BOSS_ATTACK_STATE::NORMAL_ATTACK2:
		break;
	case BOSS_ATTACK_STATE::NORMAL_ATTACK3:
		break;
	case BOSS_ATTACK_STATE::NORMAL_ATTACK4:
		break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK1:
		break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK2:
		break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK3:
		break;
	case BOSS_ATTACK_STATE::SKILL_ATTACK4:
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
					pPlayerRigidBody->AddVelocity(Vec3(1000.f, 0.f, 0.f));
					vMaxVelocity.x = 1000.f;
				}
				else if (pBossScript->GetDir() == BOSS_DIRECTION::LEFT)
				{
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

			if (m_pSkillPrefab1 == nullptr)
			{
				wstring strPrefabKey = L"Prefab\\TimeOutSkill.pref";
				wstring strContent = CPathMgr::GetInst()->GetContentPath();
				wstring FullPath = strContent + strPrefabKey;

				m_pSkillPrefab1 = new CPrefab;
				m_pSkillPrefab1->Load(FullPath);
				assert(m_pSkillPrefab1);
			}


			// Prefab 파일에 저장된 gameObject 를 읽어서 해당 Layer 에 포함한다. 
			CGameObject* NewObj = m_pSkillPrefab1->Instantiate();
			NewObj->SetName(L"LaunChSkill");
			NewObj->AddComponent((CComponent*)CScriptMgr::GetScript(L"CRigidBodyScript"));
			NewObj->AddComponent((CComponent*)CScriptMgr::GetScript(L"CGravityScript"));
			NewObj->AddComponent((CComponent*)CScriptMgr::GetScript(L"CLaunchSkillScript"));

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

}