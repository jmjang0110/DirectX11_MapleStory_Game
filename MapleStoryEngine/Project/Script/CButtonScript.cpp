#include "pch.h"
#include "CButtonScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CInventoryScript.h"	

CButtonScript::CButtonScript()
	: CScript((int)SCRIPT_TYPE::BUTTONSCRIPT)
	, m_bEnable(false)

{
	SetName(CScriptMgr::GetScriptName(this));

}



CButtonScript::CButtonScript(const CButtonScript& _origin)
	: CScript((int)SCRIPT_TYPE::BUTTONSCRIPT)
	, m_bEnable(false)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CButtonScript::~CButtonScript()
{
}





void CButtonScript::start()
{
	m_bEnable = false;

}

void CButtonScript::update()
{
	CAnimator2D* pAnimator2D = GetOwner()->Animator2D();
	wstring name = GetOwner()->GetName();



	

	if (m_bEnable)
		pAnimator2D->Play(L"ENABLED", true);
	else
		pAnimator2D->Play(L"DISABLED", true);


}

void CButtonScript::lateupdate()
{
}

void CButtonScript::OnCollisionEnter(CGameObject* _OtherObject)
{

}

void CButtonScript::OnCollision(CGameObject* _OtherObject)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Inventory");

	CGameObject* pObj = pLayer->FindObj(L"Inventory");
	CInventoryScript* pScript = (CInventoryScript*)pObj->GetScriptByName(L"CInventoryScript");


	if (KEY_TAP(KEY::LBTN))
	{
		if (m_bEnable == false)
			m_bEnable = true;

		if (GetOwner()->GetName() == L"Equip")
			pScript->SetItemType(ITEM_TYPE::EQUIP);
		else if (GetOwner()->GetName() == L"Consume")
			pScript->SetItemType(ITEM_TYPE::CONSUME);
		else if (GetOwner()->GetName() == L"Etc")
			pScript->SetItemType(ITEM_TYPE::ETC);


		UpdateButton(m_bEnable);

		
	}

}

void CButtonScript::OnCollisionExit(CGameObject* _OtherObject)
{

}



void CButtonScript::UpdateButton(bool _bEnable)
{
	m_bEnable = _bEnable;
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Inventory");

	CGameObject* pObj = pLayer->FindObj(L"Inventory");
	CInventoryScript* pScript = (CInventoryScript*)pObj->GetScriptByName(L"CInventoryScript");


	if (m_bEnable)
	{
		if (GetOwner()->GetName() == L"Equip")
		{
			//pScript->UpdateButton(ITEM_TYPE::EQUIP);
			pScript->RegisterEquip();

		}
		else if (GetOwner()->GetName() == L"Consume")
		{
			//pScript->UpdateButton(ITEM_TYPE::CONSUME);
			pScript->RegisterConsume();

		}
		else if (GetOwner()->GetName() == L"Etc")
		{
			//pScript->UpdateButton(ITEM_TYPE::ETC);
			pScript->RegisterEtc();
		}
	}
	else
	{

	}

}