#include "pch.h"
#include "CExpScript.h"
#include "CScriptMgr.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CLight2D.h>
#include <Engine/CCollider2D.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CAnimation2D.h>

#include "CRigidBodyScript.h"
#include "CBasicBallScript.h"
#include "CDoubleJumpScript.h"
#include "CSkillScript.h"
#include "CSkillnearScript.h"
#include "CInventoryScript.h"
#include "CPlayerScript.h"


#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"

#include "CScriptMgr.h"
#include "CAnimOnceScript.h"

#include "CNumberScript.h"

CExpScript::CExpScript()
	: CScript((int)SCRIPT_TYPE::EXPSCRIPT)
	, m_fMaxExp(20000.f)
	, m_pPlayer(nullptr)

{

}

CExpScript::CExpScript(const CExpScript& _origin)
	: CScript((int)SCRIPT_TYPE::EXPSCRIPT)
	, m_fMaxExp(20000.f)
	, m_pPlayer(nullptr)


{

}

CExpScript::~CExpScript()
{
}





void CExpScript::start()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Player");

	m_pPlayer = pLayer->FindObj(L"player");

	m_pLevelUpBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\PvpLevelUp.mp3", L"sound\\PvpLevelUp.mp3");
	m_fBgmstartTimer = 0.f;
	m_bLEvelUp = false;


}

void CExpScript::update()
{
	if (m_bLEvelUp == true)
		m_fBgmstartTimer += DT;

	if (m_fBgmstartTimer >= 3.f)
	{
		m_fBgmstartTimer = 0.f;
		m_bLEvelUp = false;
		m_pLevelUpBgm->Stop();
		
	}

	// Exp Update
	if (m_pPlayer == nullptr)
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Player");
		m_pPlayer = pLayer->FindObj(L"player");
	}

	CPlayerScript* playerScript = (CPlayerScript*)m_pPlayer->GetScriptByName(L"CPlayerScript");
	m_fExp = playerScript->GetExp();

	// Child Of MainBar Object 
	static Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	
	static int dir = -1;

	// Exp Scale Max Å©±â : Resolution.x - 15.f
	float Ratio = (m_fExp / m_fMaxExp);
	m_fDest = Ratio * (Resolution.x - 15.f);

	if (m_fExp >= m_fMaxExp)
	{
		m_pLevelUpBgm->Play(1, 0.8f);
		m_bLEvelUp = true;

		m_fExp = 0.f;
		playerScript->SetExp(m_fExp);


		// Level Up Effect 
		CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"LevelUpEffect");
		CGameObject* pEffect = pPrefab->Instantiate();
		CAnimOnceScript* pScript = (CAnimOnceScript*)CScriptMgr::GetScript(L"CAnimOnceScript");

		pEffect->Animator2D()->Play(L"Default", false);
		pEffect->AddComponent((CComponent*)pScript);

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pEffect, L"Player");
		CSceneMgr::GetInst()->AddChild(CSceneSaveLoad::pMainPlayer, pEffect);


		// Level Up Effect 2 
		pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"LevelUp2");
		pEffect = pPrefab->Instantiate();
		pScript = (CAnimOnceScript*)CScriptMgr::GetScript(L"CAnimOnceScript");

		pEffect->Animator2D()->Play(L"Default", false);
		pEffect->AddComponent((CComponent*)pScript);

		pCurScene = CSceneMgr::GetInst()->GetCurScene();
		pCurScene->AddObject(pEffect, L"Player");
		CSceneMgr::GetInst()->AddChild(CSceneSaveLoad::pMainPlayer, pEffect);

		// Level Up - Number 
		playerScript->LevelUp();
		CGameObject* pLevelObj =  GetOwner()->GetParent()->FindChildObj(L"Level");
		CNumberScript* pNumScript = (CNumberScript*)pLevelObj->GetScriptByName(L"CNumberScript");
		m_iLevel = playerScript->GetLevel();
		pNumScript->UpdateNumber(m_iLevel);

	}

	vScale.x += DT * 200.f;
	if (vScale.x >= m_fDest)
		vScale.x = m_fDest;
	
	//vScale.x = (Resolution.x - 15.f) * Ratio;


	//vScale.x += dir * DT * 10.f;
	//if (vScale.x <= 0.f)
	//	dir = 1;
	//else if (vScale.x >= Resolution.x - 15.f)
	//	dir = -1;
	
	vPos.x = 0.f - (Resolution.x / 2) + 15.f;
	vPos.y = 0.f - 50.f + (vScale.y / 2);
	vPos.z = -1.f;

	vPos.x += (vScale.x / 2);

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeScale(vScale);



}

void CExpScript::lateupdate()
{

}

void CExpScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CExpScript::OnCollision(CGameObject* _OtherObject)
{
}

void CExpScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

