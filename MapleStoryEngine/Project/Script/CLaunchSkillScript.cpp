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



CLaunchSkillScript::CLaunchSkillScript()
	: CScript((int)SCRIPT_TYPE::LAUNCHSKILLSCRIPT)
	, m_bOnGorund(false)

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


}

void CLaunchSkillScript::update()
{

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

void CLaunchSkillScript::lateupdate()
{


}

void CLaunchSkillScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetScriptByName(L"CGroundScript") != nullptr)
	{
		m_bOnGorund = true;
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

