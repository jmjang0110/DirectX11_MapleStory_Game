#include "pch.h"
#include "CItemScript.h"


#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CPlayerScript.h"
#include "CInventoryScript.h"
#include "CNumberScript.h"


CItemScript::CItemScript()
	: CScript((int)SCRIPT_TYPE::ITEMSCRIPT)
	, m_iClickedCnt(0)
	, m_fHpRaise(0.f)
	, m_fMpRaise(0.f)
	, m_fAttackRaise(0.f)
	, m_fTimer(0.f)
	, m_iInventoryIdx_row(-1)
	, m_iInventoryIdx_col(-1)
	, m_Cursor_StartPos(Vec3(0.f, 0.f, 0.f))
	, m_Cursor_Diff(Vec3(0.f, 0.f, 0.f))
	, m_Cursor_CurPos(Vec3(0.f, 0.f, 0.f))
	, m_Cursor_PrevDiff(Vec3(0.f, 0.f, 0.f))
	, m_bCollide_Cursor(false)
	, m_iCnt(0)
	, m_eType(ITEM_TYPE::NONE)
	, m_Consume_Type(CONSUME_TYPE::NONE)


{
	SetName(CScriptMgr::GetScriptName(this));


}

CItemScript::CItemScript(const CItemScript& _origin)
	: CScript((int)SCRIPT_TYPE::ITEMSCRIPT)
	, m_iClickedCnt(_origin.m_iClickedCnt)
	, m_fHpRaise(_origin.m_fHpRaise)
	, m_fMpRaise(_origin.m_fMpRaise)
	, m_fAttackRaise(_origin.m_fAttackRaise)
	, m_fTimer(_origin.m_fTimer)
	, m_iInventoryIdx_row(_origin.m_iInventoryIdx_row)
	, m_iInventoryIdx_col(_origin.m_iInventoryIdx_col)
	, m_Cursor_StartPos(_origin.m_Cursor_StartPos)
	, m_Cursor_Diff(_origin.m_Cursor_Diff)
	, m_Cursor_CurPos(_origin.m_Cursor_CurPos)
	, m_Cursor_PrevDiff(_origin.m_Cursor_PrevDiff)
	, m_bCollide_Cursor(_origin.m_bCollide_Cursor)
	, m_iCnt(_origin.m_iCnt)
	, m_eType(_origin.m_eType)
	, m_Consume_Type(_origin.m_Consume_Type)



{
	SetName(CScriptMgr::GetScriptName(this));

}


CItemScript::~CItemScript()
{
}


void CItemScript::AddCnt()
{
	m_iCnt++;
	CNumberScript* pNumScript = (CNumberScript*)GetOwner()->FindChildObj(L"Number")->GetScriptByName(L"CNumberScript");

	if (pNumScript != nullptr)
	{
		pNumScript->UpdateNumber(m_iCnt);

	}

}

void CItemScript::SubCnt()
{
	m_iCnt--;
	CNumberScript* pNumScript = (CNumberScript*)GetOwner()->FindChildObj(L"Number")->GetScriptByName(L"CNumberScript");

	if (pNumScript != nullptr)
	{
		pNumScript->UpdateNumber(m_iCnt);

	}
}

void CItemScript::start()
{
}

void CItemScript::update()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// Double Clicked Situation
	if (m_iClickedCnt >= 2)
	{
		m_iClickedCnt = 0;

		CGameObject* test = GetOwner();


		CLayer* pLayer = pCurScene->GetLayer(L"Player");
		CGameObject* pPlayer = pLayer->FindObj(L"player");
		CPlayerScript* playerScript = (CPlayerScript*)pPlayer->GetScriptByName(L"CPlayerScript");
		pLayer = pCurScene->GetLayer(L"Inventory");
		CGameObject* inventory = pLayer->FindObj(L"Inventory");
		CInventoryScript* pscript = (CInventoryScript*)inventory->GetScriptByName(L"CInventoryScript");


		playerScript->AddHp(m_fHpRaise);
		playerScript->AddMp(m_fMpRaise);
		//playerScript->AddExp(m_fExpRaise)



		if (m_iCnt <= 0)
		{
			// Event - Delete 
			CGameObject* pDelObj = GetOwner();
			int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

			CSceneMgr::GetInst()->DisconnectParent(pDelObj);
			CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
			GetOwner()->Destroy();

			pscript->DeleteItem(m_eType, m_iInventoryIdx_row, m_iInventoryIdx_col);
			pscript->SetMove(true);

		}
		else
		{
			SubCnt();
		}

		pscript->SetMove(true);

	}


	// UpdatePos 
	//m_vStartPos = Vec2(-315.f, 137.f);
	//m_vInterval = Vec2(42.f, -42.f);

	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vInvenPos = GetOwner()->GetAncestor()->Transform()->GetRelativePos();


	if (GetOwner()->GetLayerIndex() == pCurScene->GetLayer(L"Inventory")->GetLayerIdx())
	{
		// Inventory Relative pos - Item  
		vPos.x = -315.f + m_iInventoryIdx_col * 42.f;
		vPos.y = 137.f + m_iInventoryIdx_row * 42.f;

		if (m_bCollide_Cursor == true)
		{
			pCurScene = CSceneMgr::GetInst()->GetCurScene();
			CLayer* pLayer = pCurScene->GetLayer(L"Default");
			CGameObject* pCursorObj = nullptr;
			pCursorObj = pLayer->FindObj(L"Cursor");
			m_Cursor_CurPos = pCursorObj->Transform()->GetRelativePos();

			vPos.x = m_Cursor_CurPos.x - vInvenPos.x;
			vPos.y = m_Cursor_CurPos.y - vInvenPos.y;

		}
	}

	


	GetOwner()->Transform()->SetRelativePos(vPos);


}

void CItemScript::lateupdate()
{

}

void CItemScript::OnCollisionEnter(CGameObject* _OtherObject)
{


	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
	CGameObject* pInventory = pLayer->FindObj(L"Inventory");
	CInventoryScript* pInventoryScript = (CInventoryScript*)pInventory->GetScriptByName(L"CInventoryScript");

	pInventoryScript->SetMove(FALSE);

}

void CItemScript::OnCollision(CGameObject* _OtherObject)
{
	static Vec2 OnMoveItemIdx = Vec2(-1.f, -1.f);

	// Cursor <-> Item 
	if (_OtherObject->GetName() == L"Cursor")
	{
		// Inventory Layer 
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
		CGameObject* pInventory = pLayer->FindObj(L"Inventory");
		CInventoryScript* pInventoryScript = (CInventoryScript*)pInventory->GetScriptByName(L"CInventoryScript");


		if (KEY_TAP(KEY::LBTN) && OnMoveItemIdx == Vec2(-1.f, -1.f))
		{
			OnMoveItemIdx = Vec2(m_iInventoryIdx_col, m_iInventoryIdx_row);
			m_bCollide_Cursor = true;
			pInventoryScript->SetMove(false);

			m_iClickedCnt++;
		}

		if (KEY_PRESSED(KEY::LBTN) && (OnMoveItemIdx == Vec2(m_iInventoryIdx_col, m_iInventoryIdx_row)))
		{
			m_bCollide_Cursor = true;
			//pInventoryScript->SetMove(false);

		}

		if (KEY_AWAY(KEY::LBTN))
		{
			OnMoveItemIdx = Vec2(-1.f, -1.f);
			m_bCollide_Cursor = false;
			pInventoryScript->SetMove(true);

			// item - item test
			if (GetOwner()->GetName() == _OtherObject->GetName())
			{
				CItemScript* pItemSCript = (CItemScript*)GetOwner()->GetScriptByName(L"CItemScript");
				pItemSCript->AddCnt();
				// Event - Delete 
				CGameObject* pDelObj = _OtherObject;
				int pDelObj_LayerIdx = _OtherObject->GetLayerIndex();

				CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
				_OtherObject->Destroy();
			}

		}
	}
	

	// Player <-> Item
	if (_OtherObject->GetName() == L"player")
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
		CGameObject* pInventory = pLayer->FindObj(L"Inventory");
		CInventoryScript* pInventoryScript = (CInventoryScript*)pInventory->GetScriptByName(L"CInventoryScript");


		// Item ащ╠Б 
		if (KEY_TAP(KEY::Z))
		{
			CSceneMgr::GetInst()->DeRegisterObjInLayer(GetOwner(), GetOwner()->GetLayerIndex());
			pInventoryScript->PushItem(GetOwner()->GetName(), m_eType, GetOwner());
			//pCurScene->AddObject(GetOwner(), pLayer->GetLayerIdx());
		}


	}

}

void CItemScript::OnCollisionExit(CGameObject* _OtherObject)
{

	m_iClickedCnt = 0;
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
	CGameObject* pInventory = pLayer->FindObj(L"Inventory");
	CInventoryScript* pInventoryScript = (CInventoryScript*)pInventory->GetScriptByName(L"CInventoryScript");

	pInventoryScript->SetMove(TRUE);
}

void CItemScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);


	fwrite(&m_eType, sizeof(ITEM_TYPE), 1, _pFile);
	fwrite(&m_Consume_Type, sizeof(CONSUME_TYPE), 1, _pFile);
	fwrite(&m_iClickedCnt, sizeof(UINT), 1, _pFile);

	fwrite(&m_fHpRaise, sizeof(float), 1, _pFile);
	fwrite(&m_fMpRaise, sizeof(float), 1, _pFile);
	fwrite(&m_fAttackRaise, sizeof(float), 1, _pFile);
	fwrite(&m_iCnt, sizeof(int), 1, _pFile);



}

void CItemScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);


	fread(&m_eType, sizeof(ITEM_TYPE), 1, _pFile);
	fread(&m_Consume_Type, sizeof(CONSUME_TYPE), 1, _pFile);
	fread(&m_iClickedCnt, sizeof(UINT), 1, _pFile);

	fread(&m_fHpRaise, sizeof(float), 1, _pFile);
	fread(&m_fMpRaise, sizeof(float), 1, _pFile);
	fread(&m_fAttackRaise, sizeof(float), 1, _pFile);
	fread(&m_iCnt, sizeof(int), 1, _pFile);
}



