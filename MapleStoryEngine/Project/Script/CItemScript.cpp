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

#include "CSceneSaveLoad.h"

#include "CRigidBodyScript.h"
#include "CQuickSlotScript.h"

#include "CSceneSaveload.h"
#include "CSceneStartScript.h"


static Ptr<CSound>     m_pBgm;

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
	, m_bQuickSlot(false)
	, m_EnterQuickSlotName(L"")
	, m_pClone_QS(nullptr)
	, m_fEatTimer(0.f)
	, m_bEaten(false)
	, m_pInfoObjADdress(nullptr)
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
	, m_bQuickSlot(_origin.m_bQuickSlot)
	, m_EnterQuickSlotName(_origin.m_EnterQuickSlotName)
	, m_pClone_QS(nullptr)
	, m_fEatTimer(0.f)
	, m_bEaten(_origin.m_bEaten)
	, m_pInfoObjADdress(nullptr)


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
	if (m_iCnt < 1)
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
		CGameObject* inventory = pLayer->FindObj(L"Inventory");
		CInventoryScript* pInvenScript = (CInventoryScript*)inventory->GetScriptByName(L"CInventoryScript");

		// Event - Delete 
		CGameObject* pDelObj = GetOwner();
		int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

		CSceneMgr::GetInst()->DisconnectParent(pDelObj);
		CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
		GetOwner()->Destroy();

		pInvenScript->DeleteItem(m_eType, m_iInventoryIdx_row, m_iInventoryIdx_col);
		pInvenScript->SetMove(true);

		if (m_pQSAddress != nullptr)
			m_pQSAddress->ClearObj();

		return;

	}
	CNumberScript* pNumScript = (CNumberScript*)GetOwner()->FindChildObj(L"Number")->GetScriptByName(L"CNumberScript");

	if (pNumScript != nullptr)
	{
		pNumScript->UpdateNumber(m_iCnt);

	}

}

void CItemScript::UseThisItem()

{
	if (m_pBgm == nullptr)
		m_pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Use.mp3", L"sound\\Use.mp3");

	if(m_pBgm != nullptr)
		m_pBgm->Play(1, 0.6f, true);

	// Player 에게 적용 
	CGameObject* pPlayer = CSceneSaveLoad::pMainPlayer;
	CPlayerScript* playerScript = (CPlayerScript*)pPlayer->GetScriptByName(L"CPlayerScript");

	playerScript->AddHp(m_fHpRaise);
	playerScript->AddMp(m_fMpRaise);

}

void CItemScript::CheckItemCnt()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
	CGameObject* inventory = pLayer->FindObj(L"Inventory");
	CInventoryScript* pInvenScript = (CInventoryScript*)inventory->GetScriptByName(L"CInventoryScript");


	if (m_iCnt < 1)
	{
		// Event - Delete 
		CGameObject* pDelObj = GetOwner();
		int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

		CSceneMgr::GetInst()->DisconnectParent(pDelObj);
		CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
		GetOwner()->Destroy();

		pInvenScript->DeleteItem(m_eType, m_iInventoryIdx_row, m_iInventoryIdx_col);
		pInvenScript->SetMove(true);

	}
	else
	{
		SubCnt();
	}

}

void CItemScript::DeleteThisItem()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
	CGameObject* inventory = pLayer->FindObj(L"Inventory");
	CInventoryScript* pInvenScript = (CInventoryScript*)inventory->GetScriptByName(L"CInventoryScript");

	// Event - Delete 
	CGameObject* pDelObj = GetOwner();
	int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

	CSceneMgr::GetInst()->DisconnectParent(pDelObj);
	CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
	GetOwner()->Destroy();

	pInvenScript->DeleteItem(m_eType, m_iInventoryIdx_row, m_iInventoryIdx_col);
	pInvenScript->SetMove(true);
}

void CItemScript::NotifyToInventory()
{


}

void CItemScript::start()
{
	m_eSpotType = ITEM_SPOT::DEFAULT;
	m_bOnGround = false;

	if (GetOwner()->GetName() == L"PowerElixirPortion")
	{
		m_fHpRaise = 9999999.f;
		m_fMpRaise = 9999999.f;

	}
	m_pPickUpItemBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\PickUpItem.mp3", L"sound\\PickUpItem.mp3");

}

void CItemScript::update()
{
	if (m_bEaten == true && m_bQuickSlot == false)
	{
		m_fEatTimer += DT;
		if (m_fEatTimer >= 1.f)
		{
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
			CGameObject* pInventory = pLayer->FindObj(L"Inventory");
			CInventoryScript* pInventoryScript = (CInventoryScript*)pInventory->GetScriptByName(L"CInventoryScript");


			GetOwner()->DeleteScript(L"CGravityScript");
			GetOwner()->DeleteScript(L"CRigidBodyScript");


			CSceneMgr::GetInst()->DeRegisterObjInLayer(GetOwner(), GetOwner()->GetLayerIndex());
			pInventoryScript->PushItem(GetOwner()->GetName(), m_eType, GetOwner());
			//pCurScene->AddObject(GetOwner(), pLayer->GetLayerIdx());

			m_fEatTimer = 0.f;
			m_bOnGround = false;
			m_bEaten = false;
			m_eSpotType = ITEM_SPOT::DEFAULT;
			return;


		}

		if (m_eSpotType == ITEM_SPOT::JUMP)
		{
			CRigidBodyScript* prigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");
			if (prigid != nullptr)
			{
				m_eSpotType = ITEM_SPOT::LAND;
				prigid->AddVelocity(Vec3(0.f, 200.f, 0.f));
				m_bOnGround = true;

			}
		}

		Vec3 vPlayerPos = CSceneSaveLoad::pMainPlayer->Transform()->GetRelativePos();
		Vec3 vItemPos = GetOwner()->Transform()->GetRelativePos();

		Vec3 vDiff = vPlayerPos - vItemPos;

		float fSpeed = 5.f;
		vItemPos.x += vDiff.x * DT * fSpeed;
		vItemPos.y += vDiff.y * DT * fSpeed;
		GetOwner()->Transform()->SetRelativePos(vItemPos);


	}

	else if (m_bEaten == false)
	{
		if (m_eSpotType == ITEM_SPOT::JUMP)
		{

			CRigidBodyScript* prigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");
			if (prigid != nullptr)
			{
				m_eSpotType = ITEM_SPOT::LAND;
				prigid->AddVelocity(Vec3(0.f, 100.f, 0.f));
				m_bOnGround = true;
			}

		}

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
		CGameObject* inventory = pLayer->FindObj(L"Inventory");
		CInventoryScript* pInvenScript = (CInventoryScript*)inventory->GetScriptByName(L"CInventoryScript");


		// Double Clicked Situation
		if (m_iClickedCnt >= 2)
		{
			m_iClickedCnt = 0;

			// Use This Item to Player 
			UseThisItem();
			CheckItemCnt();
			//m_pBgm->Stop();

			if (m_pClone_QS != nullptr)
			{
				CItemScript* pItemScript = (CItemScript*)m_pClone_QS->GetScriptByName(L"CItemScript");
				if (pItemScript != nullptr)
				{
					pItemScript->CheckItemCnt();
				}
			}

			pInvenScript->SetMove(true);

		}


		// Item 이 Inventory 에 있는 상황 
		if (GetOwner()->GetLayerIndex() == pCurScene->GetLayer(L"Inventory")->GetLayerIdx())
		{

			// UpdatePos 
			//m_vStartPos = Vec2(-315.f, 137.f);
			//m_vInterval = Vec2(42.f, -42.f);
			Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
			Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
			Vec3 vInvenPos = GetOwner()->GetAncestor()->Transform()->GetRelativePos();

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
			GetOwner()->Transform()->SetRelativePos(vPos);

		}
	}







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
	//m_bCollide_Cursor = true;


	if (_OtherObject->GetName() == L"Cursor")
	{
		// Info 
		wstring InfoName = GetOwner()->GetName() + L"_Info";
		CPrefab* pPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(InfoName);

		if (pPrefab != nullptr && m_iInventoryIdx_col != -1)
		{
			CGameObject* pInfoObj = pPrefab->Instantiate();

			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			pCurScene->AddObject(pInfoObj, L"Inventory");
			CSceneMgr::GetInst()->AddChild(GetOwner(), pInfoObj);
			m_pInfoObjADdress = pInfoObj;

		}
	}


}

void CItemScript::OnCollision(CGameObject* _OtherObject)
{

	if (_OtherObject->GetName() == L"BluePortion")
		int i = 0;

	static Vec2 OnMoveItemIdx = Vec2(-1.f, -1.f);

	// Cursor <-> Item 
	if (_OtherObject->GetName() == L"Cursor")
	{

		// Inventory Layer 
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
		CGameObject* pInventory = pLayer->FindObj(L"Inventory");
		CInventoryScript* pInventoryScript = (CInventoryScript*)pInventory->GetScriptByName(L"CInventoryScript");
		pInventoryScript->SetMove(false);


		if (KEY_TAP(KEY::LBTN) && OnMoveItemIdx == Vec2(-1.f, -1.f))
		{
			OnMoveItemIdx = Vec2(m_iInventoryIdx_col, m_iInventoryIdx_row);
			m_bCollide_Cursor = true;
			pInventoryScript->SetMove(false);
			Ptr<CSound> pDragBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\DragStart.mp3", L"sound\\DragStart.mp3");
			pDragBgm->Play(1, 0.6f, true);

			m_iClickedCnt++;
		}

		if (KEY_PRESSED(KEY::LBTN) && (OnMoveItemIdx == Vec2(m_iInventoryIdx_col, m_iInventoryIdx_row)))
		{
			m_bCollide_Cursor = true;
			pInventoryScript->SetMove(false);

		}

		if (KEY_AWAY(KEY::LBTN))
		{
			OnMoveItemIdx = Vec2(-1.f, -1.f);
			m_bCollide_Cursor = false;
			pInventoryScript->SetMove(true);

			Ptr<CSound> pDragBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\DragEnd.mp3", L"sound\\DragEnd.mp3");
			pDragBgm->Play(1, 0.6f, true);



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


		// Item 줍기 
		if (KEY_TAP(KEY::Z))
		{
			m_bEaten = true;
			if(m_pPickUpItemBgm == nullptr)
				m_pPickUpItemBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\PickUpItem.mp3", L"sound\\PickUpItem.mp3");
			
			if (m_pPickUpItemBgm_Start == false)
			{
				m_pPickUpItemBgm_Start = true;
				m_pPickUpItemBgm->Play(1, 0.7f, true);
			}

		}
	}

		



	if (m_pInfoObjADdress != nullptr)
	{
		if (KEY_AWAY(KEY::LBTN))
		{
			CSceneMgr::GetInst()->DeRegisterObjInLayer(m_pInfoObjADdress, m_pInfoObjADdress->GetLayerIndex());
			m_pInfoObjADdress->Destroy();
			m_pInfoObjADdress = nullptr;

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
	m_bCollide_Cursor = false;


	if (m_pInfoObjADdress != nullptr)
	{
		CSceneMgr::GetInst()->DeRegisterObjInLayer(m_pInfoObjADdress, m_pInfoObjADdress->GetLayerIndex());
		m_pInfoObjADdress->Destroy();
		m_pInfoObjADdress = nullptr;

	}


}

void CItemScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);


	fwrite(&m_bQuickSlot, sizeof(bool), 1, _pFile);
	SaveWStringToFile(m_EnterQuickSlotName, _pFile);


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

	fread(&m_bQuickSlot, sizeof(bool), 1, _pFile);
	LoadWStringFromFile(m_EnterQuickSlotName, _pFile);


	fread(&m_eType, sizeof(ITEM_TYPE), 1, _pFile);
	fread(&m_Consume_Type, sizeof(CONSUME_TYPE), 1, _pFile);
	fread(&m_iClickedCnt, sizeof(UINT), 1, _pFile);

	fread(&m_fHpRaise, sizeof(float), 1, _pFile);
	fread(&m_fMpRaise, sizeof(float), 1, _pFile);
	fread(&m_fAttackRaise, sizeof(float), 1, _pFile);
	fread(&m_iCnt, sizeof(int), 1, _pFile);
}



