#include "pch.h"
#include "CLaunchSkillScript.h"


#include "CScriptMgr.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CLight2D.h>
#include <Engine/CCollider2D.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CAnimation2D.h>

#include "CGroundScript.h"
#include "CBossMonsterScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"



CLaunchSkillScript::CLaunchSkillScript()
	: CScript((int)SCRIPT_TYPE::LAUNCHSKILLSCRIPT)
	, m_bOnGorund(false)
	, m_bHit(false)
	, m_fTimer(0.f)
{
	SetName(CScriptMgr::GetScriptName(this));

}


CLaunchSkillScript::CLaunchSkillScript(const CLaunchSkillScript& _origin)
	: CScript((int)SCRIPT_TYPE::LAUNCHSKILLSCRIPT)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CLaunchSkillScript::~CLaunchSkillScript()
{
}


void CLaunchSkillScript::start()
{
	// LAyer - Subskill_2

	m_bOnGorund = false;
	m_bHit = false;

	m_fTimer = 0.f;

}

void CLaunchSkillScript::update()
{
	if(m_bOnGorund  == true)
		m_fTimer += DT;


	if (m_UseGround == true)
	{
		// Anim When On Ground 
		if (m_bOnGorund == true)
		{
			GetOwner()->Animator2D()->Play(L"Default", false);

		}

		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish() == true)
		{
			// Event 
			CGameObject* pDelObj = GetOwner();
			int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

			CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
			GetOwner()->Destroy();
		}
	}
	else if (m_bHit == true)
	{
		
		// Anim When Hit Obj
		GetOwner()->Animator2D()->Play(L"HIT", false);
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish() == true)
		{
			// Event 
			CGameObject* pDelObj = GetOwner();
			int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

			CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
			GetOwner()->Destroy();
		}
	}

}

void CLaunchSkillScript::lateupdate()
{


}

void CLaunchSkillScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetScriptByName(L"CGroundScript") != nullptr)
	{
		m_bOnGorund = true;

	}

	CBossMonsterScript* pBossScript = (CBossMonsterScript*)_OtherObject->GetScriptByName(L"CBossMonsterScript");
	CMonsterScript* pMobScript = (CMonsterScript*)_OtherObject->GetScriptByName(L"CMonsterScript");
	CPlayerScript* pPlayerScript = (CPlayerScript*)_OtherObject->GetScriptByName(L"CPlayerScript");

	if (pBossScript != nullptr ||
		pMobScript != nullptr ||
		pPlayerScript != nullptr  )
	{
		m_bHit = true;
	}

}

void CLaunchSkillScript::OnCollision(CGameObject* _OtherObject)
{
	if (_OtherObject->GetScriptByName(L"CGroundScript") != nullptr)
	{
		m_bOnGorund = true;
	}

}

void CLaunchSkillScript::OnCollisionExit(CGameObject* _OtherObject)
{

}

void CLaunchSkillScript::SaveToScene(FILE* _pFile)
{
}

void CLaunchSkillScript::LoadFromScene(FILE* _pFile)
{
}

