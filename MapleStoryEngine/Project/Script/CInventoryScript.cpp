#include "pch.h"
#include "CInventoryScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CPrefab.h>


#include "CSceneSaveLoad.h"
#include "CButtonScript.h"
#include "CItemScript.h"
#include "CNumberScript.h"


CInventoryScript::CInventoryScript()
	: CScript((int)SCRIPT_TYPE::INVENTORYSCRIPT)
	, m_eEnabledTabType(ITEM_TYPE::NONE)
	, m_Cursor_StartPos(0.f)
	, m_Cursor_Diff(0.f)
	, m_Cursor_CurPos(0.f)
	, m_Cursor_PrevDiff(0.f)
	, m_bMove(true)

{
	SetName(CScriptMgr::GetScriptName(this));

	//for (int i = 0; i < INVENTORY_ROW; ++i)
	//{
	//	m_Equip[i][INVENTORY_COL] = { nullptr, };
	//	m_Consume[i][INVENTORY_COL] = { nullptr, };
	//	m_Etc[i][INVENTORY_COL] = { nullptr, };
	//}
	// »ó´ëÁÂÇ¥ 
	m_vStartPos = Vec2(-315.f, 137.f);
	m_vInterval = Vec2(42.f, -42.f);


}

CInventoryScript::CInventoryScript(const CInventoryScript& _origin)
	: CScript((int)SCRIPT_TYPE::INVENTORYSCRIPT)
	, m_eEnabledTabType(ITEM_TYPE::NONE)
	, m_Cursor_StartPos(0.f)
	, m_Cursor_Diff(0.f)
	, m_Cursor_CurPos(0.f)
	, m_Cursor_PrevDiff(0.f)
	, m_bMove(true)
{
	SetName(CScriptMgr::GetScriptName(this));
	//for (int i = 0; i < INVENTORY_ROW; ++i)
	//{
	//	m_Equip[i][INVENTORY_COL] = { nullptr, };
	//	m_Consume[i][INVENTORY_COL] = { nullptr, };
	//	m_Etc[i][INVENTORY_COL] = { nullptr, };
	//}

	m_vStartPos = Vec2(-315.f, 137.f);
	m_vInterval = Vec2(42.f, -42.f);

}


CInventoryScript::~CInventoryScript()
{

}



void CInventoryScript::DeleteItem(ITEM_TYPE _Type, int row, int col)
{
	switch (_Type)
	{
	case ITEM_TYPE::EQUIP:
		m_Equip[row][col] = nullptr;
		break;
	case ITEM_TYPE::CONSUME:
		m_Consume[row][col] = nullptr;
		break;
	case ITEM_TYPE::ETC:
		m_Etc[row][col] = nullptr;
		break;

	}
}

void CInventoryScript::start()
{


}

void CInventoryScript::update()
{
	static bool test = false;
	static int cnt = 0;
	static CPrefab* pPrefab2 = nullptr;

	/*if (cnt == 0)
	{
		vector<CGameObject*> pChild = GetOwner()->GetChild();
		for (int i = 0; i < pChild.size(); ++i)
		{
			if (pChild[i]->GetName() == L"RedPortion")
			{
				CGameObject* pDelObj = pChild[i];
				int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();
				CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);

				pChild[i]->Destroy();

			}
		}
	}*/

	if (test == false)
	{
	
		wstring strPrefabKey = L"prefab\\RedPortion.pref";
		wstring strContent = CPathMgr::GetInst()->GetContentPath();
		wstring FullPath = strContent + strPrefabKey;

		pPrefab2 = new CPrefab;
		pPrefab2->Load(FullPath);

		test = true;

	}


	cnt++;
	if (cnt <= 2)
	{


		CGameObject* pObj = pPrefab2->Instantiate();
		CNumberScript* pNumScript = (CNumberScript*)CScriptMgr::GetScript(L"CNumberScript");
		CItemScript* pItemScript = (CItemScript*)CScriptMgr::GetScript(L"CItemScript");

		pObj->AddComponent((CComponent*)pItemScript);
		pObj->AddComponent((CComponent*)pNumScript);

		//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		//pCurScene->AddObject(pObj, GetOwner()->GetLayerIndex());

		pNumScript->Init(NUMBER_TYPE::ITEM);
		pNumScript->UpdateNumbber(1);
		pItemScript->setItemType(ITEM_TYPE::CONSUME, CONSUME_TYPE::HP);

		// test pItemScript
		pItemScript->SetHpRaise(10000.f);
		pItemScript->SetMpRaise(10000.f);



		pObj->Deactivate();
		PushItem(ITEM_TYPE::CONSUME, pObj);


	}
	if (cnt == 21)
	{
		SAFE_DELETE(pPrefab2);

	}

	// Update Pos 
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Default");
	CGameObject* pCamera = pLayer->FindObj(L"MainCamera");

	static Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 CameraPos = pCamera->Transform()->GetRelativePos();

	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	Vec3 vPos = CameraPos;
	vPos.x += (m_Cursor_Diff.x + m_Cursor_PrevDiff.x);
	vPos.y += (m_Cursor_Diff.y + m_Cursor_PrevDiff.y);

	vPos.z += 100.f;
	GetOwner()->Transform()->SetRelativePos(vPos);

	

}

void CInventoryScript::lateupdate()
{


}

void CInventoryScript::OnCollisionEnter(CGameObject* _OtherObject)
{



}

void CInventoryScript::OnCollision(CGameObject* _OtherObject)
{

	if (m_bMove == false)
		return;

	// find Camera Obj !!  
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Default");
	CGameObject* pCursorObj = nullptr;
	pCursorObj = pLayer->FindObj(L"Cursor");
	m_Cursor_CurPos = pCursorObj->Transform()->GetRelativePos();
	CGameObject* pCamera = pLayer->FindObj(L"MainCamera");
	Vec3 CameraPos = pCamera->Transform()->GetRelativePos();


	if (KEY_TAP(KEY::LBTN))
	{
		m_Cursor_StartPos.x = m_Cursor_CurPos.x;
		m_Cursor_StartPos.y = m_Cursor_CurPos.y;

		m_Cursor_PrevDiff = m_Cursor_Diff;
		m_Cursor_Diff.x = 0.f;
		m_Cursor_Diff.y = 0.f;


	}

	if (KEY_PRESSED(KEY::LBTN))
	{

		m_Cursor_Diff.x = m_Cursor_CurPos.x - m_Cursor_StartPos.x ;
		m_Cursor_Diff.y = m_Cursor_CurPos.y - m_Cursor_StartPos.y;

	}

	if (KEY_AWAY(KEY::LBTN))
	{
		m_Cursor_Diff.x = m_Cursor_Diff.x + m_Cursor_PrevDiff.x;
		m_Cursor_Diff.y = m_Cursor_Diff.y + m_Cursor_PrevDiff.y;

		m_Cursor_PrevDiff.x = 0.f;
		m_Cursor_PrevDiff.y = 0.f;


	}


}

void CInventoryScript::OnCollisionExit(CGameObject* _OtherObject)
{



}

void CInventoryScript::RegisterEquip()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (int i = 0; i < INVENTORY_ROW; ++i)
	{
		for (int k = 0; k < INVENTORY_COL; ++k)
		{
			if (m_Equip[i][k] != nullptr)
				m_Equip[i][k]->Activate();
		}
	}

	DeRegisterConsume();
	DeRegisterEtc();

}

void CInventoryScript::RegisterConsume()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();


	for (int i = 0; i < INVENTORY_ROW; ++i)
	{
		for (int k = 0; k < INVENTORY_COL; ++k)
		{
			if(m_Consume[i][k] != nullptr)
				m_Consume[i][k]->Activate();
		}
	}
	DeRegisterEtc();
	DeRegisterEquip();

}

void CInventoryScript::RegisterEtc()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (int i = 0; i < INVENTORY_ROW; ++i)
	{
		for (int k = 0; k < INVENTORY_COL; ++k)
		{
			if (m_Etc[i][k] != nullptr)
				m_Etc[i][k]->Activate();
		}
	}
	DeRegisterConsume();
	DeRegisterEquip();


}

void CInventoryScript::DeRegisterEquip()
{
	CGameObject* pObj  = GetOwner()->FindChildObj(L"Equip");
	CButtonScript* pScript = (CButtonScript*)pObj->GetScriptByName(L"CButtonScript");
	pScript->SetEnable(false);


	for (int i = 0; i < INVENTORY_ROW; ++i)
	{
		for (int k = 0; k < INVENTORY_COL; ++k)
		{
			if (m_Equip[i][k] != nullptr)
			{
				m_Equip[i][k]->Deactivate();


			}
		}
	}

}

void CInventoryScript::DeRegisterConsume()
{
	CGameObject* pObj = GetOwner()->FindChildObj(L"Consume");
	CButtonScript* pScript = (CButtonScript*)pObj->GetScriptByName(L"CButtonScript");
	pScript->SetEnable(false);
	for (int i = 0; i < INVENTORY_ROW; ++i)
	{
		for (int k = 0; k < INVENTORY_COL; ++k)
		{
			if (m_Consume[i][k] != nullptr)
			{
				m_Consume[i][k]->Deactivate();
			}
		}
	}
}

void CInventoryScript::DeRegisterEtc()
{
	CGameObject* pObj = GetOwner()->FindChildObj(L"Etc");
	CButtonScript* pScript = (CButtonScript*)pObj->GetScriptByName(L"CButtonScript");
	pScript->SetEnable(false);
	for (int i = 0; i < INVENTORY_ROW; ++i)
	{
		for (int k = 0; k < INVENTORY_COL; ++k)
		{
			if (m_Etc[i][k] != nullptr)
			{
				m_Etc[i][k]->Deactivate();


			}
		}
	}

}

void CInventoryScript::AllDeregister(CGameObject* _deObj)
{
	vector<CGameObject*> child = _deObj->GetChild();

	for (int i = 0; i < child.size(); ++i)
	{
		vector<CGameObject*> chchild = child[i]->GetChild();
		for (int k = 0; k < chchild.size(); ++k)
			AllDeregister(chchild[k]);

		child[i]->Deactivate();


	}

}

void CInventoryScript::AllRegister(CGameObject* _reObj)
{
	vector<CGameObject*> child = _reObj->GetChild();

	for (int i = 0; i < child.size(); ++i)
	{
		vector<CGameObject*> chchild = child[i]->GetChild();
		for (int k = 0; k < chchild.size(); ++k)
			AllRegister(chchild[k]);

		child[i]->Activate();
	}

	UpdateButton(m_eEnabledTabType);

}

void CInventoryScript::PushItem(ITEM_TYPE _type, CGameObject* _item)
{
	bool bPushSuccess = false;
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CItemScript* pItemScript = (CItemScript*)_item->GetScriptByName(L"CItemScript");
	

	switch (_type)
	{

	case ITEM_TYPE::EQUIP:
	{
		for (int i = 0; i < INVENTORY_ROW; ++i)
		{
			for (int k = 0; k < INVENTORY_COL; ++k)
			{
				if (m_Equip[i][k] == nullptr)
				{
					m_Equip[i][k] = _item;
					float x = m_vStartPos.x + m_vInterval.x * k;
					float y = m_vStartPos.y + m_vInterval.y * i;
					_item->Transform()->SetRelativePos(Vec3(x, y, -1.f));
					_item->Transform()->SetIgnoreParentScale(true);
					pCurScene->AddObject(_item, L"Inventory");
					CSceneMgr::GetInst()->AddChild(GetOwner(), _item);

					pItemScript->m_iInventoryIdx_row = i;
					pItemScript->m_iInventoryIdx_col=  k;


					bPushSuccess = true;
					break;
				}
			}
			if (bPushSuccess)
				break;
		}


	}
	break;
	case ITEM_TYPE::CONSUME:
	{
		bool firstPush = false;

		for (int i = 0; i < INVENTORY_ROW; ++i)
		{
			for (int k = 0; k < INVENTORY_COL; ++k)
			{
				if (m_Consume[i][k] == nullptr)
					continue;

				if (m_Consume[i][k] != nullptr 
					&& m_Consume[i][k]->GetName() == _item->GetName())
				{
					CItemScript* itemScript = (CItemScript*)m_Consume[i][k]->GetScriptByName(L"CItemScript");
					itemScript->AddCnt();

				}
			}
		}

		if (firstPush == false)
		{
			for (int i = 0; i < INVENTORY_ROW; ++i)
			{
				for (int k = 0; k < INVENTORY_COL; ++k)
				{
					if (m_Consume[i][k] == nullptr)
					{
						m_Consume[i][k] = _item;
						float x = m_vStartPos.x + m_vInterval.x * k;
						float y = m_vStartPos.y + m_vInterval.y * i;
						_item->Transform()->SetRelativePos(Vec3(x, y, -1.f));
						_item->Transform()->SetIgnoreParentScale(true);

						_item->UpdateLayerIdx(GetOwner()->GetLayerIndex());
						GetOwner()->AddChild(_item);

						//pCurScene->AddObject(_item, L"Inventory");

						//CSceneMgr::GetInst()->DeRegisterObjInLayer(_item, GetOwner()->GetLayerIndex());
						//CSceneMgr::GetInst()->AddChild(GetOwner(), _item);

						pItemScript->m_iInventoryIdx_row = i;
						pItemScript->m_iInventoryIdx_col = k;
						bPushSuccess = true;
						break;
					}
				}
				if (bPushSuccess)
					break;
			}
		}
		

	}
	break;
	case ITEM_TYPE::ETC:
	{
		for (int i = 0; i < INVENTORY_ROW; ++i)
		{
			for (int k = 0; k < INVENTORY_COL; ++k)
			{
				if (m_Etc[i][k] == nullptr)
				{
					m_Etc[i][k] = _item;
					float x = m_vStartPos.x + m_vInterval.x * k;
					float y = m_vStartPos.y + m_vInterval.y * i;
					_item->Transform()->SetRelativePos(Vec3(x, y, -1.f));
					_item->Transform()->SetIgnoreParentScale(true);
					pCurScene->AddObject(_item, L"Inventory");
					CSceneMgr::GetInst()->AddChild(GetOwner(), _item);

					pItemScript->m_iInventoryIdx_row = i;
					pItemScript->m_iInventoryIdx_col = k;
					bPushSuccess = true;
					break;
				}
			}
			if (bPushSuccess)
				break;
		}

	}
	break;
	}

}

void CInventoryScript::UpdateButton(ITEM_TYPE _type)
{
	CGameObject* Equip = GetOwner()->FindChildObj(L"Equip");
	CGameObject* Consume= GetOwner()->FindChildObj(L"Consume");
	CGameObject* Etc = GetOwner()->FindChildObj(L"Etc");

	switch (_type)
	{
	case ITEM_TYPE::EQUIP:
	{
		RegisterEquip();
		//DeRegisterConsume();
		//DeRegisterEtc();


	}
		break;
	case ITEM_TYPE::CONSUME:
	{
		//DeRegisterEquip();
		RegisterConsume();
		//DeRegisterEtc();

	}
		break;
	case ITEM_TYPE::ETC:
	{
		//DeRegisterEquip();
		//DeRegisterConsume();
		RegisterEtc();

	}
		break;

	}

}
