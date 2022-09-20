#include "pch.h"
#include "CDieNoticeScript.h"
#include "CButtonScript.h"
#include "CPlayerScript.h"

#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"
#include <Engine/CPrefab.h>
#include "CAnimOnceScript.h"
#include <Engine/CAnimator2D.h>



CDieNoticeScript::CDieNoticeScript()
	: CScript((int)SCRIPT_TYPE::DIENOTICESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CDieNoticeScript::CDieNoticeScript(const CDieNoticeScript& _origin)
	: CScript((int)SCRIPT_TYPE::DIENOTICESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CDieNoticeScript::~CDieNoticeScript()
{
}



void CDieNoticeScript::BtnClickedEvent(DWORD_PTR _param)
{
	// _param : nullptr 
	// Player 상태 Reset 
	CPlayerScript* pPlayerScript = (CPlayerScript*)CSceneSaveLoad::pMainPlayer->GetScriptByName(L"CPlayerScript");
	pPlayerScript->Revival();

	// Destroy 
	// sKill ( Revival) 
	CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"RevivalEffect");
	CGameObject* pEffect = pPrefab->Instantiate();
	CAnimOnceScript* pScript = (CAnimOnceScript*)CScriptMgr::GetScript(L"CAnimOnceScript");

	pEffect->Animator2D()->Play(L"Default", false);
	pEffect->AddComponent((CComponent*)pScript);

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->AddObject(pEffect, L"Player");
	CSceneMgr::GetInst()->AddChild(CSceneSaveLoad::pMainPlayer, pEffect);


	// Delete Die Notice  
	CGameObject* pDelObj = GetOwner();
	int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

	if (GetOwner()->GetParent() != nullptr)
	{
		CSceneMgr::GetInst()->DisconnectParent(pDelObj);
	}

	CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
	GetOwner()->Destroy();


}

void CDieNoticeScript::start()
{
	m_pBtn = GetOwner()->FindChildObj(L"YesBtn");
	if (m_pBtn != nullptr)
	{
		m_pBtnScript = (CButtonScript*)m_pBtn->GetScriptByName(L"CButtonScript");
		m_pBtnScript->SetBtnClickedFunc(this, (CLIKCED)&CDieNoticeScript::BtnClickedEvent);

	}

}

void CDieNoticeScript::update()
{
	// 위치 조정 
	CGameObject* pCamera = CSceneSaveLoad::pMainCamera;
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

	static Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 CameraPos = pCamera->Transform()->GetRelativePos();
	Vec3 vPos = CameraPos;
	vPos.z += 100.f;
	GetOwner()->Transform()->SetRelativePos(vPos);


}

void CDieNoticeScript::lateupdate()
{
}

void CDieNoticeScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CDieNoticeScript::OnCollision(CGameObject* _OtherObject)
{
}

void CDieNoticeScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

