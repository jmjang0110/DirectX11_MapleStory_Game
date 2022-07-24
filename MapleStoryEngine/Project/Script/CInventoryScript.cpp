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
	// 상대좌표 
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

	Safe_Del_Map(m_Equip);
	Safe_Del_Map(m_Consume);
	Safe_Del_Map(m_Etc);
	SAFE_DELETE(m_pNumPrefab);


}




void CInventoryScript::start()
{
	CGameObject* equip = GetOwner()->FindChildObj(L"Equip");
	m_pEquipBtnScript = (CButtonScript*)equip->GetScriptByName(L"CButtonScript");
	m_pEquipBtnScript->SetBtnClickedFunc(this, (CLIKCED)&CInventoryScript::ShowEquip);



	CGameObject* consume = GetOwner()->FindChildObj(L"Consume");
	m_pConsumeBtnScript = (CButtonScript*)consume->GetScriptByName(L"CButtonScript");
	m_pConsumeBtnScript->SetBtnClickedFunc(this, (CLIKCED)&CInventoryScript::ShowConsume);



	CGameObject* etc = GetOwner()->FindChildObj(L"Etc");
	m_pEtcBtnScript = (CButtonScript*)etc->GetScriptByName(L"CButtonScript");
	m_pEtcBtnScript->SetBtnClickedFunc(this, (CLIKCED)&CInventoryScript::ShowEtc);


	// Num Obj
	wstring strPrefabKey_Num = L"prefab\\Number.pref";
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring FullPath_num = strContent + strPrefabKey_Num;

	m_pNumPrefab = nullptr;
	m_pNumPrefab = new CPrefab;
	m_pNumPrefab->Load(FullPath_num);





	CPrefab* pPrefab2 = nullptr;
	wstring strPrefabKey = L"prefab\\RedPortion.pref";
	strContent = CPathMgr::GetInst()->GetContentPath();
	wstring FullPath = strContent + strPrefabKey;
	pPrefab2 = new CPrefab;
	pPrefab2->Load(FullPath);




	// test portion obj 
	CGameObject* pObj = pPrefab2->Instantiate();
	CItemScript* pItemScript = (CItemScript*)CScriptMgr::GetScript(L"CItemScript");
	pObj->AddComponent((CComponent*)pItemScript);

	pItemScript->setItemType(ITEM_TYPE::CONSUME, CONSUME_TYPE::HP);
	pItemScript->SetHpRaise(10000.f);
	pItemScript->SetMpRaise(10000.f);

	pObj->Deactivate();







	PushItem(L"TestPortion", ITEM_TYPE::CONSUME, pObj);

	SAFE_DELETE(pPrefab2);

}

void CInventoryScript::update()
{


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

	/*

	static bool test = false;
	static int cnt = 0;
	static CPrefab* pPrefab2 = nullptr;

	if (cnt == 0)
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
	}

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

	*/
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

		m_Cursor_Diff.x = m_Cursor_CurPos.x - m_Cursor_StartPos.x;
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


void CInventoryScript::ShowEquip(DWORD_PTR _param)
{
	m_pEquipBtnScript->SetEnable(true);
	m_pConsumeBtnScript->SetEnable(false);
	m_pEtcBtnScript->SetEnable(false);

	map<wstring, ItemInven*>::iterator iter = m_Equip.begin();
	for (; iter != m_Equip.end(); ++iter)
	{
		iter->second->obj->Activate();
		iter->second->Num->Activate();


	}
	map<wstring, ItemInven*>::iterator iter2 = m_Consume.begin();
	for (; iter2 != m_Consume.end(); ++iter2)
	{
		iter2->second->obj->Deactivate();
		iter2->second->Num->Deactivate();

	}
	map<wstring, ItemInven*>::iterator iter3 = m_Etc.begin();
	for (; iter3 != m_Etc.end(); ++iter3)
	{
		iter3->second->obj->Deactivate();
		iter3->second->Num->Deactivate();

	}
}

void CInventoryScript::ShowConsume(DWORD_PTR _param)
{
	m_pEquipBtnScript->SetEnable(false);
	m_pConsumeBtnScript->SetEnable(true);
	m_pEtcBtnScript->SetEnable(false);

	map<wstring, ItemInven*>::iterator iter = m_Equip.begin();
	for (; iter != m_Equip.end(); ++iter)
	{
		iter->second->obj->Deactivate();
		iter->second->Num->Deactivate();

	}
	map<wstring, ItemInven*>::iterator iter2 = m_Consume.begin();
	for (; iter2 != m_Consume.end(); ++iter2)
	{
		iter2->second->obj->Activate();
		iter2->second->Num->Activate();

	}
	map<wstring, ItemInven*>::iterator iter3 = m_Etc.begin();
	for (; iter3 != m_Etc.end(); ++iter3)
	{
		iter3->second->obj->Deactivate();
		iter3->second->Num->Deactivate();

	}

}

void CInventoryScript::ShowEtc(DWORD_PTR _param)
{
	m_pEquipBtnScript->SetEnable(false);
	m_pConsumeBtnScript->SetEnable(false);
	m_pEtcBtnScript->SetEnable(true);

	map<wstring, ItemInven*>::iterator iter = m_Equip.begin();
	for (; iter != m_Equip.end(); ++iter)
	{
		iter->second->obj->Deactivate();
		iter->second->Num->Deactivate();

	}
	map<wstring, ItemInven*>::iterator iter2 = m_Consume.begin();
	for (; iter2 != m_Consume.end(); ++iter2)
	{
		iter2->second->obj->Deactivate();
		iter2->second->Num->Deactivate();

	}
	map<wstring, ItemInven*>::iterator iter3 = m_Etc.begin();
	for (; iter3 != m_Etc.end(); ++iter3)
	{
		iter3->second->obj->Activate();
		iter3->second->Num->Activate();

	}
}

void CInventoryScript::ShowOffAllItem()
{
	m_pEquipBtnScript->SetEnable(false);
	m_pConsumeBtnScript->SetEnable(false);
	m_pEtcBtnScript->SetEnable(false);

	map<wstring, ItemInven*>::iterator iter = m_Equip.begin();
	for (; iter != m_Equip.end(); ++iter)
	{
		iter->second->obj->Deactivate();
		iter->second->Num->Deactivate();

	}
	map<wstring, ItemInven*>::iterator iter2 = m_Consume.begin();
	for (; iter2 != m_Consume.end(); ++iter2)
	{
		iter2->second->obj->Deactivate();
		iter2->second->Num->Deactivate();

	}
	map<wstring, ItemInven*>::iterator iter3 = m_Etc.begin();
	for (; iter3 != m_Etc.end(); ++iter3)
	{
		iter3->second->obj->Deactivate();
		iter3->second->Num->Deactivate();

	}

}

void CInventoryScript::PushItem(wstring _name, ITEM_TYPE _type, CGameObject* _item)
{

	switch (_type)
	{
	case ITEM_TYPE::EQUIP:
	{
		ItemInven* itemInven = new ItemInven;
		itemInven->obj = _item;

		bool bFindSpot = false;
		// 들어온 Item 이 Inventory 에 저장되어 있는지 Check 
		map<wstring, ItemInven*>::iterator iter2 = m_Equip.begin();
		for (; iter2 != m_Equip.end(); ++iter2)
		{
			if (iter2->second->obj->GetName() == _name)
			{
				CItemScript* pItemScrtip = (CItemScript*)iter2->second->obj->GetScriptByName(L"CItemScript");
				if (pItemScrtip != nullptr)
				{
					pItemScrtip->AddCnt();
				}
				iter2->second->num += 1;
				bFindSpot = true;
				break;
			}
		
		}
		if (bFindSpot == false)
		{
			for (int i = 0; i < INVENTORY_ROW; ++i)
			{
				for (int k = 0; k < INVENTORY_COL; ++k)
				{
					if (m_EquipSpot[i][k] == false)
					{
						itemInven->row = i;
						itemInven->col = k;
						CItemScript* pscript = (CItemScript*)_item->GetScriptByName(L"CItemScript");
						pscript->m_iInventoryIdx_row = i;
						pscript->m_iInventoryIdx_col = k;

						// Num 
						CGameObject* pNum = m_pNumPrefab->Instantiate();
						pNum->Transform()->SetRelativePos(Vec3(-9.f, -9.f, -1.f));
						CNumberScript* pNumScript = (CNumberScript*)CScriptMgr::GetScript(L"CNumberScript");
						pNum->AddComponent((CComponent*)pNumScript);
						pNumScript->Init(NUMBER_TYPE::ITEM);
						pNumScript->UpdateNumber(10);
						pNum->Deactivate();
						_item->AddChild(pNum);

						itemInven->Num = pNum;



						bFindSpot = true;
						break;
					}
				}
				if (bFindSpot == true)
					break;
		}
		

			m_Equip.insert(make_pair(_name, itemInven));
		
		}



	}
	break;
	case ITEM_TYPE::CONSUME:
	{
		ItemInven* itemInven = new ItemInven;
		itemInven->obj = _item;
		

		bool bFindSpot = false;
		// 들어온 Item 이 Inventory 에 저장되어 있는지 Check 
		map<wstring, ItemInven*>::iterator iter2 = m_Consume.begin();
		for (; iter2 != m_Consume.end(); ++iter2)
		{
			if (iter2->second->obj->GetName() == _name)
			{
				CItemScript* pItemScrtip = (CItemScript*)iter2->second->obj->GetScriptByName(L"CItemScript");
				if (pItemScrtip != nullptr)
				{
					pItemScrtip->AddCnt();
				}
				iter2->second->num += 1;
				bFindSpot = true;
				break;
			}

		}

		if (bFindSpot == false)
		{
			for (int i = 0; i < INVENTORY_ROW; ++i)
			{
				for (int k = 0; k < INVENTORY_COL; ++k)
				{
					if (m_ConsumeSpot[i][k] == false)
					{
						itemInven->row = i;
						itemInven->col = k;
						CItemScript* pscript = (CItemScript*)_item->GetScriptByName(L"CItemScript");
						pscript->SetCnt(10);
						pscript->m_iInventoryIdx_row = i;
						pscript->m_iInventoryIdx_col = k;

						// Num 
						CGameObject* pNum = m_pNumPrefab->Instantiate();
						pNum->Transform()->SetRelativePos(Vec3(-9.f, -9.f, -1.f));
						CNumberScript* pNumScript = (CNumberScript*)CScriptMgr::GetScript(L"CNumberScript");
						pNum->AddComponent((CComponent*)pNumScript);
						pNumScript->Init(NUMBER_TYPE::ITEM);
						pNumScript->UpdateNumber(10);
						pNum->Deactivate();
						_item->AddChild(pNum);

						itemInven->Num = pNum;
						bFindSpot = true;
						break;
					}
				}
				if (bFindSpot == true)
					break;
			}

			m_Consume.insert(make_pair(_name, itemInven));
		}


	}
	break;
	case ITEM_TYPE::ETC:
	{
		ItemInven* itemInven = new ItemInven;
		itemInven->obj = _item;

		bool bFindSpot = false;
		// 들어온 Item 이 Inventory 에 저장되어 있는지 Check 
		map<wstring, ItemInven*>::iterator iter2 = m_Etc.begin();
		for (; iter2 != m_Etc.end(); ++iter2)
		{
			if (iter2->second->obj->GetName() == _name)
			{
				CItemScript* pItemScrtip = (CItemScript*)iter2->second->obj->GetScriptByName(L"CItemScript");
				if (pItemScrtip != nullptr)
				{
					pItemScrtip->AddCnt();
				}
				iter2->second->num += 1;
				bFindSpot = true;
				break;
			}

		}
		for (int i = 0; i < INVENTORY_ROW; ++i)
		{
			for (int k = 0; k < INVENTORY_COL; ++k)
			{
				if (m_EtcSpot[i][k] == false)
				{
					itemInven->row = i;
					itemInven->col = k;
					CItemScript* pscript = (CItemScript*)_item->GetScriptByName(L"CItemScript");
					pscript->m_iInventoryIdx_row = i;
					pscript->m_iInventoryIdx_col = k;

					// Num 
					CGameObject* pNum = m_pNumPrefab->Instantiate();
					pNum->Transform()->SetRelativePos(Vec3(-9.f, -9.f, -1.f));
					CNumberScript* pNumScript = (CNumberScript*)CScriptMgr::GetScript(L"CNumberScript");
					pNum->AddComponent((CComponent*)pNumScript);
					pNumScript->Init(NUMBER_TYPE::ITEM);
					pNumScript->UpdateNumber(1);
					_item->AddChild(pNum);

					itemInven->Num = pNum;



					bFindSpot = true;
					break;
				}
			}
			if (bFindSpot == true)
				break;
		}

		m_Etc.insert(make_pair(_name, itemInven));
	}
	break;
	}


	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	pCurScene->AddObject(_item, L"Inventory");
	_item->UpdateLayerIdx(GetOwner()->GetLayerIndex());

	CSceneMgr::GetInst()->AddChild(GetOwner(), _item);
}

void CInventoryScript::Show(bool _b)
{
	m_bActivate = _b;

	// Activate 
	if (m_bActivate == true)
	{
		GetOwner()->Activate();
		vector<CGameObject*> child = GetOwner()->GetChild();

		for (int i = 0; i < child.size(); ++i)
		{
			CButtonScript* pBtnScript = (CButtonScript*)child[i]->GetScriptByName(L"CButtonScript");

			if (pBtnScript != nullptr)
				pBtnScript->Show(true);
			else
				break;
		}

	}

	// Deactivate
	else if (m_bActivate == false)
	{
		GetOwner()->Deactivate();
		vector<CGameObject*> child = GetOwner()->GetChild();

		for (int i = 0; i < child.size(); ++i)
		{
			CButtonScript* pBtnScript = (CButtonScript*)child[i]->GetScriptByName(L"CButtonScript");

			if (pBtnScript != nullptr)
				pBtnScript->Show(false);
			else
				break;

		}
		ShowOffAllItem();


	}

}


void CInventoryScript::DeleteItem(ITEM_TYPE _Type, int row, int col)
{
	switch (_Type)
	{
	case ITEM_TYPE::EQUIP:
	{
		map<wstring, ItemInven*>::iterator iter = m_Equip.begin();
		for (; iter != m_Equip.end(); ++iter)
		{
			iter->second->obj->Deactivate();
			iter->second->Num->Deactivate();

		}

	}

	break;
	case ITEM_TYPE::CONSUME:
	{
		map<wstring, ItemInven*>::iterator iter = m_Consume.begin();
		for (; iter != m_Consume.end(); ++iter)
		{
			if (iter->second->row == row && iter->second->col == col)
			{
				iter->second->num -= 1;

				if (iter->second->num <= 0)
				{
					m_Consume.erase(iter);
					break;
				}

			}
		}
	}

	break;
	case ITEM_TYPE::ETC:
	{
		map<wstring, ItemInven*>::iterator iter = m_Etc.begin();
		for (; iter != m_Etc.end(); ++iter)
		{
			iter->second->obj->Activate();
		}
	}

	break;

	}
}

//void CInventoryScript::RegisterEquip()
//{
//	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
//
//	for (int i = 0; i < INVENTORY_ROW; ++i)
//	{
//		for (int k = 0; k < INVENTORY_COL; ++k)
//		{
//			if (m_Equip[i][k] != nullptr)
//				m_Equip[i][k]->Activate();
//		}
//	}
//
//	DeRegisterConsume();
//	DeRegisterEtc();
//
//}
//
//void CInventoryScript::RegisterConsume()
//{
//	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
//
//
//	for (int i = 0; i < INVENTORY_ROW; ++i)
//	{
//		for (int k = 0; k < INVENTORY_COL; ++k)
//		{
//			if(m_Consume[i][k] != nullptr)
//				m_Consume[i][k]->Activate();
//		}
//	}
//	DeRegisterEtc();
//	DeRegisterEquip();
//
//}
//
//void CInventoryScript::RegisterEtc()
//{
//	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
//
//	for (int i = 0; i < INVENTORY_ROW; ++i)
//	{
//		for (int k = 0; k < INVENTORY_COL; ++k)
//		{
//			if (m_Etc[i][k] != nullptr)
//				m_Etc[i][k]->Activate();
//		}
//	}
//	DeRegisterConsume();
//	DeRegisterEquip();
//
//
//}
//
//void CInventoryScript::DeRegisterEquip()
//{
//	CGameObject* pObj  = GetOwner()->FindChildObj(L"Equip");
//	CButtonScript* pScript = (CButtonScript*)pObj->GetScriptByName(L"CButtonScript");
//	pScript->SetEnable(false);
//
//
//	for (int i = 0; i < INVENTORY_ROW; ++i)
//	{
//		for (int k = 0; k < INVENTORY_COL; ++k)
//		{
//			if (m_Equip[i][k] != nullptr)
//			{
//				m_Equip[i][k]->Deactivate();
//
//
//			}
//		}
//	}
//
//}
//
//void CInventoryScript::DeRegisterConsume()
//{
//	CGameObject* pObj = GetOwner()->FindChildObj(L"Consume");
//	CButtonScript* pScript = (CButtonScript*)pObj->GetScriptByName(L"CButtonScript");
//	pScript->SetEnable(false);
//	for (int i = 0; i < INVENTORY_ROW; ++i)
//	{
//		for (int k = 0; k < INVENTORY_COL; ++k)
//		{
//			if (m_Consume[i][k] != nullptr)
//			{
//				m_Consume[i][k]->Deactivate();
//			}
//		}
//	}
//}
//
//void CInventoryScript::DeRegisterEtc()
//{
//	CGameObject* pObj = GetOwner()->FindChildObj(L"Etc");
//	CButtonScript* pScript = (CButtonScript*)pObj->GetScriptByName(L"CButtonScript");
//	pScript->SetEnable(false);
//	for (int i = 0; i < INVENTORY_ROW; ++i)
//	{
//		for (int k = 0; k < INVENTORY_COL; ++k)
//		{
//			if (m_Etc[i][k] != nullptr)
//			{
//				m_Etc[i][k]->Deactivate();
//
//
//			}
//		}
//	}
//
//}
//
//void CInventoryScript::AllDeregister(CGameObject* _deObj)
//{
//	vector<CGameObject*> child = _deObj->GetChild();
//
//	for (int i = 0; i < child.size(); ++i)
//	{
//		vector<CGameObject*> chchild = child[i]->GetChild();
//		for (int k = 0; k < chchild.size(); ++k)
//			AllDeregister(chchild[k]);
//
//		child[i]->Deactivate();
//
//
//	}
//
//}
//
//void CInventoryScript::AllRegister(CGameObject* _reObj)
//{
//	vector<CGameObject*> child = _reObj->GetChild();
//
//	for (int i = 0; i < child.size(); ++i)
//	{
//		vector<CGameObject*> chchild = child[i]->GetChild();
//		for (int k = 0; k < chchild.size(); ++k)
//			AllRegister(chchild[k]);
//
//		child[i]->Activate();
//	}
//
//	UpdateButton(m_eEnabledTabType);
//
//}
//
//void CInventoryScript::PushItem(ITEM_TYPE _type, CGameObject* _item)
//{
//	bool bPushSuccess = false;
//	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
//	CItemScript* pItemScript = (CItemScript*)_item->GetScriptByName(L"CItemScript");
//	
//
//	switch (_type)
//	{
//
//	case ITEM_TYPE::EQUIP:
//	{
//		for (int i = 0; i < INVENTORY_ROW; ++i)
//		{
//			for (int k = 0; k < INVENTORY_COL; ++k)
//			{
//				if (m_Equip[i][k] == nullptr)
//				{
//					m_Equip[i][k] = _item;
//					float x = m_vStartPos.x + m_vInterval.x * k;
//					float y = m_vStartPos.y + m_vInterval.y * i;
//					_item->Transform()->SetRelativePos(Vec3(x, y, -1.f));
//					_item->Transform()->SetIgnoreParentScale(true);
//					pCurScene->AddObject(_item, L"Inventory");
//					CSceneMgr::GetInst()->AddChild(GetOwner(), _item);
//
//					pItemScript->m_iInventoryIdx_row = i;
//					pItemScript->m_iInventoryIdx_col=  k;
//
//
//					bPushSuccess = true;
//					break;
//				}
//			}
//			if (bPushSuccess)
//				break;
//		}
//
//
//	}
//	break;
//	case ITEM_TYPE::CONSUME:
//	{
//		bool firstPush = false;
//
//		for (int i = 0; i < INVENTORY_ROW; ++i)
//		{
//			for (int k = 0; k < INVENTORY_COL; ++k)
//			{
//				if (m_Consume[i][k] == nullptr)
//					continue;
//
//				if (m_Consume[i][k] != nullptr 
//					&& m_Consume[i][k]->GetName() == _item->GetName())
//				{
//					CItemScript* itemScript = (CItemScript*)m_Consume[i][k]->GetScriptByName(L"CItemScript");
//					itemScript->AddCnt();
//
//				}
//			}
//		}
//
//		if (firstPush == false)
//		{
//			for (int i = 0; i < INVENTORY_ROW; ++i)
//			{
//				for (int k = 0; k < INVENTORY_COL; ++k)
//				{
//					if (m_Consume[i][k] == nullptr)
//					{
//						m_Consume[i][k] = _item;
//						float x = m_vStartPos.x + m_vInterval.x * k;
//						float y = m_vStartPos.y + m_vInterval.y * i;
//						_item->Transform()->SetRelativePos(Vec3(x, y, -1.f));
//						_item->Transform()->SetIgnoreParentScale(true);
//
//						_item->UpdateLayerIdx(GetOwner()->GetLayerIndex());
//						GetOwner()->AddChild(_item);
//
//						//pCurScene->AddObject(_item, L"Inventory");
//
//						//CSceneMgr::GetInst()->DeRegisterObjInLayer(_item, GetOwner()->GetLayerIndex());
//						//CSceneMgr::GetInst()->AddChild(GetOwner(), _item);
//
//						pItemScript->m_iInventoryIdx_row = i;
//						pItemScript->m_iInventoryIdx_col = k;
//						bPushSuccess = true;
//						break;
//					}
//				}
//				if (bPushSuccess)
//					break;
//			}
//		}
//		
//
//	}
//	break;
//	case ITEM_TYPE::ETC:
//	{
//		for (int i = 0; i < INVENTORY_ROW; ++i)
//		{
//			for (int k = 0; k < INVENTORY_COL; ++k)
//			{
//				if (m_Etc[i][k] == nullptr)
//				{
//					m_Etc[i][k] = _item;
//					float x = m_vStartPos.x + m_vInterval.x * k;
//					float y = m_vStartPos.y + m_vInterval.y * i;
//					_item->Transform()->SetRelativePos(Vec3(x, y, -1.f));
//					_item->Transform()->SetIgnoreParentScale(true);
//					pCurScene->AddObject(_item, L"Inventory");
//					CSceneMgr::GetInst()->AddChild(GetOwner(), _item);
//
//					pItemScript->m_iInventoryIdx_row = i;
//					pItemScript->m_iInventoryIdx_col = k;
//					bPushSuccess = true;
//					break;
//				}
//			}
//			if (bPushSuccess)
//				break;
//		}
//
//	}
//	break;
//	}
//
//}
//
//void CInventoryScript::UpdateButton(ITEM_TYPE _type)
//{
//	CGameObject* Equip = GetOwner()->FindChildObj(L"Equip");
//	CGameObject* Consume= GetOwner()->FindChildObj(L"Consume");
//	CGameObject* Etc = GetOwner()->FindChildObj(L"Etc");
//
//	switch (_type)
//	{
//	case ITEM_TYPE::EQUIP:
//	{
//		RegisterEquip();
//		//DeRegisterConsume();
//		//DeRegisterEtc();
//
//
//	}
//		break;
//	case ITEM_TYPE::CONSUME:
//	{
//		//DeRegisterEquip();
//		RegisterConsume();
//		//DeRegisterEtc();
//
//	}
//		break;
//	case ITEM_TYPE::ETC:
//	{
//		//DeRegisterEquip();
//		//DeRegisterConsume();
//		RegisterEtc();
//
//	}
//		break;
//
//	}
//
//}
