#include "pch.h"
#include "CSkillnearScript.h"


#include <Engine/CGameObject.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

#include <Engine/CPrefab.h>


#include "CBasicBallScript.h"
#include "CScriptMgr.h"
#include "CPlayerScript.h"


CSkillnearScript::CSkillnearScript()
	: CScript((int)SCRIPT_TYPE::SKILLNEARSCRIPT)
	, m_SkillObj(nullptr)
	, m_pSkillUser(nullptr)
	, m_iAttackCnt(0)
	, m_MyHitObjAddress(nullptr)
	, m_fTimer(0.f)


{
	SetName(CScriptMgr::GetScriptName(this));

}



CSkillnearScript::CSkillnearScript(const CSkillnearScript& _origin)
	: CScript((int)SCRIPT_TYPE::SKILLNEARSCRIPT)
	, m_SkillObj(nullptr)
	, m_pSkillUser(nullptr)
	, m_iAttackCnt(0)
	, m_MyHitObjAddress(nullptr)
	, m_fTimer(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CSkillnearScript::~CSkillnearScript()
{

}

CGameObject* CSkillnearScript::GetHitMonseterObjAddress(int _hitNum)
{
	auto obj = m_mapHitMob.find(_hitNum);
	if (obj == m_mapHitMob.end())
		return nullptr;

	return obj->second;
}

// 매개변수로 들어온 Prefab은 외부에서 알아서 잘 지우도록.. 
void CSkillnearScript::SetSkillObjByPrefab(CPrefab* _Pref, wstring _SkillName)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pCurLayer = pCurScene->GetLayer(L"SubSkill_1");

	if (_Pref != nullptr)
	{
		m_SkillObj = _Pref->Instantiate();
		m_SkillObj->SetName(_SkillName.c_str());
		m_SkillObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

		GetOwner()->AddChild(m_SkillObj);
	}
	
}


void CSkillnearScript::start()
{

	// test 
	m_iAttackCnt = 0;
	m_iAttackMaxCnt = 6;
	m_fMAxAttack = 10000;
	m_fMinAttack = 5000;

	m_iHitMaxMonsterCnt = 0;
	m_iHitMonsterCnt = 0;

	

}

void CSkillnearScript::update()
{
	// 맞고 나서 몇초가 흘렀는지 
	//if (m_MyHitObjAddress != nullptr)
	//	m_fTimer += DT;


	if (m_iHitMonsterCnt != 0)
		m_fTimer += DT;

	CAnimator2D* pAnimator2D = GetOwner()->Animator2D();
	if (pAnimator2D == nullptr)
		return;

	// TAP
	if (KEY_TAP(m_eKey))
	{
		CPlayerScript* pPlayerScript = (CPlayerScript*)m_pSkillUser->GetScriptByName(L"CPlayerScript");

		if (pPlayerScript->GetDir() == PLAYER_DIRECTION::LEFT)
		{
			pAnimator2D->Play(L"Move_Left", false);

		}
		else if (pPlayerScript->GetDir() == PLAYER_DIRECTION::RIGHT)
		{
			pAnimator2D->Play(L"Move_Right", false);
		}

		m_bDelete = true;

		if (m_pUseBgm == nullptr)
		{
			if (GetOwner()->GetName() == L"LargeSkill")
			{
				m_pUseBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Blast\\Use.mp3", L"sound\\Blast\\Use.mp3");
				m_pHitBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Blast\\Hit.mp3", L"sound\\Blast\\Hit.mp3");

			}
			else if (GetOwner()->GetName() == L"FinalBlow")
			{
				m_pUseBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\FinalBlow\\Use.mp3", L"sound\\FinalBlow\\Use.mp3");
				m_pHitBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\FinalBlow\\Hit.mp3", L"sound\\FinalBlow\\Hit.mp3");

			}

			m_pUseBgm->Play(1, 0.8f, true);
		}

		
		CSound::UpdateFMOD();

	}

	// PRESSED
	if (KEY_PRESSED(m_eKey))
	{
		CPlayerScript* pPlayerScript = (CPlayerScript*)m_pSkillUser->GetScriptByName(L"CPlayerScript");

		if (pPlayerScript->GetDir() == PLAYER_DIRECTION::LEFT)
		{
			pAnimator2D->Play(L"Move_Left", true);

		}
		else if (pPlayerScript->GetDir() == PLAYER_DIRECTION::RIGHT)
		{
			pAnimator2D->Play(L"Move_Right", true);
		}

		m_bDelete = false;
	}

	// AWAY
	if (KEY_AWAY(m_eKey))
	{
		CPlayerScript* pPlayerScript = (CPlayerScript*)m_pSkillUser->GetScriptByName(L"CPlayerScript");

		if (pPlayerScript->GetDir() == PLAYER_DIRECTION::LEFT)
		{
			pAnimator2D->Play(L"Move_Left", false);

		}
		else if (pPlayerScript->GetDir() == PLAYER_DIRECTION::RIGHT)
		{
			pAnimator2D->Play(L"Move_Right", false);
		}

		m_bDelete = true;
	}


	// Delete
	CAnimation2D* pCurAnim = pAnimator2D->GetCurAnim();
	if (pCurAnim->IsFinish() == true && m_bDelete == true)
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		CGameObject* pDelObj = GetOwner();
		CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj->GetLayerIndex());
		pDelObj->Destroy();

	}

	return;
}

void CSkillnearScript::lateupdate()
{

}

void CSkillnearScript::OnCollisionEnter(CGameObject* _OtherObject)
{


}

void CSkillnearScript::OnCollision(CGameObject* _OtherObject)
{



}

void CSkillnearScript::OnCollisionExit(CGameObject* _OtherObject)
{


}

void CSkillnearScript::SaveToScene(FILE* _pFile)
{


}

void CSkillnearScript::LoadFromScene(FILE* _pFile)
{


}

