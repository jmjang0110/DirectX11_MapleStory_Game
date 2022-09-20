#include "pch.h"
#include "CQuickSlotScript.h"

#include "CItemScript.h"
#include "CInventoryScript.h"




CQuickSlotScript::CQuickSlotScript()
	: CScript((int)SCRIPT_TYPE::QUICKSLOTSCRIPT)
	, m_wProtoName(L"")
	, m_bExistedItem(false)
	, m_pCloneObj(nullptr)
	, m_pProtoObj(nullptr)

{
	SetName(CScriptMgr::GetScriptName(this));

}



CQuickSlotScript::CQuickSlotScript(const CQuickSlotScript& _origin)
	: CScript((int)SCRIPT_TYPE::QUICKSLOTSCRIPT)
	, m_wProtoName(_origin.m_wProtoName)
	, m_bExistedItem(_origin.m_bExistedItem)
	, m_pCloneObj(nullptr)
	, m_pProtoObj(nullptr)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CQuickSlotScript::~CQuickSlotScript()
{
}





void CQuickSlotScript::start()
{
	//m_pProtoObj = nullptr;
	//m_pCloneObj = nullptr;

	wstring OwnerName = GetOwner()->GetName();

	if (OwnerName == L"QuickSlot_Box1")
		m_eKey = KEY::_1;

	if (OwnerName == L"QuickSlot_Box2")
		m_eKey = KEY::_2;

	if (OwnerName == L"QuickSlot_Box3")
		m_eKey = KEY::_3;

	if (OwnerName == L"QuickSlot_Box4")
		m_eKey = KEY::_4;

	// 이전에 Child로 이미 있었다면 삭제 
	vector<CGameObject*> pChild = GetOwner()->GetChild();
	for (int i = 0; i < pChild.size(); ++i)
	{
		CSceneMgr::GetInst()->DisconnectParent(pChild[i]);
		pChild[i]->Destroy();

	}

	// 이전에 저장된 Proto가 있었다면 
	if (m_wProtoName != L"")
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
		CGameObject* pInventory = pLayer->FindObj(L"Inventory");
		CInventoryScript* pInvenScript = (CInventoryScript*)pInventory->GetScriptByName(L"CInventoryScript");

		// setting
		if (pInvenScript != nullptr)
		{


			m_pProtoObj = pInvenScript->GetItemByName(m_wProtoName);
			m_wProtoName = m_pProtoObj->GetName();
			m_pCloneObj = m_pProtoObj->Clone();

			CItemScript* pItemScript = (CItemScript*)m_pProtoObj->GetScriptByName(L"CItemScript");
			pItemScript->SetCloneObjAddress(m_pCloneObj);

			CGameObject* pItem = m_pCloneObj;
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			pCurScene->AddObject(pItem, GetOwner()->GetLayerIndex());
			GetOwner()->AddChild(pItem);
			pItem->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));


			pItemScript->SetQuickSlotName(GetOwner()->GetName());
			pItemScript->SetIsEnterQuickSlot(true);
			pItemScript->m_pProto = nullptr; // 자신이 proto 임 
			pItemScript->m_pQSAddress = this;


			CItemScript* pCloneItemScript = (CItemScript*)m_pCloneObj->GetScriptByName(L"CItemScript");
			if (pCloneItemScript != nullptr)
			{
				pCloneItemScript->m_pProto = m_pProtoObj;
				pCloneItemScript->m_pClone_QS = nullptr;
			}
		}

	}


}

void CQuickSlotScript::update()
{
	if (m_pProtoObj == nullptr || m_pCloneObj == nullptr)
		return;

	if (KEY_TAP(m_eKey))
	{

		// Update item in QuickSlot
		CItemScript* pItemScript = (CItemScript*)m_pCloneObj->GetScriptByName(L"CItemScript");
		pItemScript->UseThisItem();
		pItemScript->CheckItemCnt();

		if (pItemScript->GetCnt() <= 0)
		{
			m_pCloneObj = nullptr;
			m_wProtoName = L"";
			m_bExistedItem = false;
		}

		// Update item in Inventory 
		pItemScript = (CItemScript*)m_pProtoObj->GetScriptByName(L"CItemScript");
		pItemScript->CheckItemCnt();
		if (pItemScript->GetCnt() <= 0)
		{
			m_pProtoObj = nullptr;
			m_wProtoName = L"";
			m_bExistedItem = false;

		}
	}

}

void CQuickSlotScript::lateupdate()
{
}

void CQuickSlotScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (m_pProtoObj != nullptr && m_pCloneObj != nullptr)
		return;

	CItemScript* pItemScript = (CItemScript*)_OtherObject->GetScriptByName(L"CItemScript");
	if (pItemScript != nullptr)
	{
		if (KEY_PRESSED(KEY::LBTN))
		{
			if (pItemScript->GetQuickSlotName() == L"")
			{
				pItemScript->SetQuickSlotName(GetOwner()->GetName());
			}

		}
	}


}


void CQuickSlotScript::OnCollision(CGameObject* _OtherObject)
{
	if (m_pProtoObj != nullptr && m_pCloneObj != nullptr)
		return;


	CItemScript* pItemScript = (CItemScript*)_OtherObject->GetScriptByName(L"CItemScript");


	if (pItemScript != nullptr)
	{
		// 이미 QuickSlot 에 놓여있는 상태라면 
		if (pItemScript->GetIsEnterQuickSlot() == true)
			return;

		if (KEY_AWAY(KEY::LBTN))
		{
			pItemScript->SetQuickSlotName(GetOwner()->GetName());


			if (pItemScript->GetQuickSlotName() == GetOwner()->GetName())
			{
	


				m_bExistedItem = true;
				m_pProtoObj = _OtherObject;
				m_wProtoName = m_pProtoObj->GetName();
				m_pCloneObj = _OtherObject->Clone();

				wstring InfoName = m_pCloneObj->GetName() + L"_Info";
				if (m_pCloneObj->FindChildObj(InfoName) != nullptr)
				{
					CGameObject* pInfo = m_pCloneObj->FindChildObj(InfoName);
					CSceneMgr::GetInst()->DisconnectParent(pInfo);
					//CSceneMgr::GetInst()->DeRegisterObjInLayer(pInfo, pInfo->GetLayerIndex());
					pInfo->Destroy();
				}


				pItemScript->SetCloneObjAddress(m_pCloneObj);

				CGameObject* pItem = m_pCloneObj;
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
				
				pCurScene->AddObject(pItem, GetOwner()->GetLayerIndex());
				GetOwner()->AddChild(pItem);
				pItem->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));


				pItemScript->SetQuickSlotName(GetOwner()->GetName());
				pItemScript->SetIsEnterQuickSlot(true);
				pItemScript->m_pProto = nullptr; // 자신이 proto 임 
				pItemScript->m_pQSAddress = this;


				CItemScript* pCloneItemScript = (CItemScript*)m_pCloneObj->GetScriptByName(L"CItemScript");
				if (pCloneItemScript != nullptr)
				{
					pCloneItemScript->m_pProto = m_pProtoObj;
					pCloneItemScript->m_pClone_QS = nullptr;
				}

			
			}

		}
	}

}

void CQuickSlotScript::OnCollisionExit(CGameObject* _OtherObject)
{
	if (m_pProtoObj != nullptr && m_pCloneObj != nullptr)
		return;


	CItemScript* pItemScript = (CItemScript*)_OtherObject->GetScriptByName(L"CItemScript");
	if (pItemScript != nullptr)
	{
		if (KEY_PRESSED(KEY::LBTN))
		{
			if (pItemScript->GetQuickSlotName() == GetOwner()->GetName())
			{
				pItemScript->SetQuickSlotName(L"");
			}

		}
	}


}

void CQuickSlotScript::SaveToScene(FILE* _pFile)
{
	CScript::SaveToScene(_pFile);

	//bool bIsProtoExist = false;
	//if (m_pProtoObj != nullptr)
	//	bIsProtoExist = true;

	fwrite(&m_bExistedItem, sizeof(bool), 1, _pFile);
	if (m_bExistedItem == true)
	{
		SaveWStringToFile(m_wProtoName, _pFile);

	}

}

void CQuickSlotScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);
	
	//bool bIsProtoExist = false;
	fread(&m_bExistedItem, sizeof(bool), 1, _pFile);

	if (m_bExistedItem == true)
	{
		LoadWStringFromFile(m_wProtoName, _pFile);

	}
	


}

