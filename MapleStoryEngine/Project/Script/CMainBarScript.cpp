#include "pch.h"
#include "CMainBarScript.h"

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

#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"
#include "CNumberScript.h"
#include "CPlayerScript.h"


CMainBarScript::CMainBarScript()
	: CScript((int)SCRIPT_TYPE::MAINBARSCRIPT)
	, m_pSubObj(nullptr)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CMainBarScript::CMainBarScript(const CMainBarScript& _origin)
	: CScript((int)SCRIPT_TYPE::MAINBARSCRIPT)
	, m_pSubObj(nullptr)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CMainBarScript::~CMainBarScript()
{
}

void CMainBarScript::start()
{
	CGameObject* LevelNum = GetOwner()->FindChildObj(L"Level");
	if (LevelNum != nullptr)
	{
		CNumberScript* pNumScript = (CNumberScript*)LevelNum->GetScriptByName(L"CNumberScript");
		pNumScript->Init(NUMBER_TYPE::MESO);
		CPlayerScript* pPlayerScript = (CPlayerScript*)CSceneSaveLoad::pMainPlayer->GetScriptByName(L"CPlayerScript");
		int level = 0;
		if(pPlayerScript != nullptr)
		level = pPlayerScript->GetLevel();

		pNumScript->UpdateNumber(level);

		
	
	}


}

void CMainBarScript::update()
{
	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//CLayer* pLayer = pCurScene->GetLayer(L"Default");
	CGameObject* pCamera = CSceneSaveLoad::pMainCamera;

	static Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 CameraPos = pCamera->Transform()->GetRelativePos();
	
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	Vec3 vPos = CameraPos;
	vPos.z += 100.f;

	vPos.y = CameraPos.y - (Resolution.y / 2) + (vScale.y / 2);
	GetOwner()->Transform()->SetRelativePos(vPos);

	if (m_pSubObj == nullptr)
	{
		m_pSubObj = GetOwner()->FindChildObj(L"HpMpSub");
	}

	if (m_pSubObj != nullptr)
	{
		Vec3 vPosSub = Vec3(0.f, 0.f, -5.f);
		vPosSub.x += 22.f;
		vPosSub.y -= 8.f;
		m_pSubObj->Transform()->SetRelativePos(vPosSub);
	}




}

void CMainBarScript::lateupdate()
{
}

void CMainBarScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CMainBarScript::OnCollision(CGameObject* _OtherObject)
{
}

void CMainBarScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CMainBarScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);
}

void CMainBarScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);

}
