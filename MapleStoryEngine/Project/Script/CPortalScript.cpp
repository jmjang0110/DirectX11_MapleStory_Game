#include "pch.h"
#include "CPortalScript.h"

#include "CSceneSaveLoad.h"
#include <Engine/CTransform.h>

#include "CSceneStartScript.h"



CPortalScript::CPortalScript()
	: CScript((int)SCRIPT_TYPE::PORTALSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CPortalScript::CPortalScript(const CPortalScript& _origin)
	: CScript((int)SCRIPT_TYPE::PORTALSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CPortalScript::~CPortalScript()
{
}


void CPortalScript::start()
{
	wstring PortalName = GetOwner()->GetName();
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	if (PortalName == L"Osolgil2VicportTown")
	{
		m_wCurScene_RelativePath = L"scene\\Osolgil.scene";
		m_wNextScene_RelativePath = L"scene\\VicportTown.scene";
		m_vNextSceneStartPos = Vec2(2424, -914);
	}

	if (pCurScene->GetName() == L"ClockTowerBottomFloor")
	{
		// ClockTowerBottomFloor -> ClockTowerOriginalFloor
		if (PortalName == L"FromClockTowerOriginFloor")
		{
			m_wCurScene_RelativePath = L"scene\\ClockTowerBottomFloor.scene";
			m_wNextScene_RelativePath = L"scene\\ClockTowerOriginFloor.scene";
			m_vNextSceneStartPos = Vec2(-900, -270);
		}

		// ClockTowerOriginalFloor -> ClockTowerBottomFloor
		if (PortalName == L"FromToyCastle")
		{
			m_wCurScene_RelativePath = L"scene\\ClockTowerOriginFloor.scene";
			m_wNextScene_RelativePath = L"scene\\ToyCastle.scene";
			m_vNextSceneStartPos = Vec2(-424, -765);

		}
	}
	

	if (pCurScene->GetName() == L"ClockTowerOriginFloor")
	{
		// ClockTowerBottomFloor -> ClockTowerOriginalFloor
		if (PortalName == L"FromClockTowerBottomFloor")
		{
			m_wCurScene_RelativePath = L"scene\\ClockTowerOriginFloor.scene";
			m_wNextScene_RelativePath = L"scene\\ClockTowerBottomFloor.scene";
			m_vNextSceneStartPos = Vec2(550, -400);
		}
	}


	if (pCurScene->GetName() == L"ToyCastle")
	{
		if (PortalName == L"FromClockTowerBottomFloor")
		{
			m_wCurScene_RelativePath = L"scene\\ToyCastle.scene";
			m_wNextScene_RelativePath = L"scene\\ClockTowerBottomFloor.scene";
			m_vNextSceneStartPos = Vec2(200, 400);
		}
	}
	if (pCurScene->GetName() == L"MapleLoad")
	{
		if (PortalName == L"ToShipToToyCastle")
		{
			m_wCurScene_RelativePath = L"scene\\MapleLoad.scene";
			m_wNextScene_RelativePath = L"scene\\ShipToToyCastle.scene";
			m_vNextSceneStartPos = Vec2(200, 400);
		}
	}


}

void CPortalScript::update()
{
	

}

void CPortalScript::lateupdate()
{
}

void CPortalScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CPortalScript::OnCollision(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() != L"player")
		return;


	if (KEY_TAP(KEY::UP))
	{
		// Change Scene !!
		// 로딩할 Scene 파일의 경로 계산
		wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
		strFilePath += m_wNextScene_RelativePath;

		m_pNextScene = CSceneSaveLoad::LoadScene(strFilePath);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		CSceneMgr::GetInst()->SceneChangeEvent(pCurScene, m_pNextScene);


		SavePlayerToPrefab();
		SaveInventoryToPrefab();
		SaveQuickSlotToPrefab();

		
		CSceneSaveLoad::pSceneMgrScript->End();
		CSceneMgr::GetInst()->StartSceneEvent(m_pNextScene);

	}

}

void CPortalScript::OnCollisionExit(CGameObject* _OtherObject)
{
}



void CPortalScript::SavePlayerToPrefab()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Player");
	CGameObject* pPlayerObj = pLayer->FindObj(L"player");

	// NextScene 에서 Player 의 위치를 미리 조정 
	Vec3 vPos = pPlayerObj->Transform()->GetRelativePos();
	vPos.x = m_vNextSceneStartPos.x;
	vPos.y = m_vNextSceneStartPos.y;
	pPlayerObj->Transform()->SetRelativePos(vPos);




	// RelativePath 저장은 CPRefab Save 에서 해주고 있음 
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring wstrResKey = L"prefab\\" + pPlayerObj->GetName() + L".pref";
	wstring FullPath = strContent + wstrResKey;


	CPrefab* pPref = new CPrefab;;// (pProtoObj);
	CGameObject* pCloneObj = pPlayerObj->Clone();

	pPref->SetProto(pCloneObj);

	// Prefab 추가
	if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey))
	{
		// Prefab 추가 
		CResMgr::GetInst()->AddRes<CPrefab>(wstrResKey, pPref);

	}
	// Prefab 이 이미 있을 경우 
	else
	{
		RES_TYPE pResType = CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey)->GetResType();
		if (RES_TYPE::PREFAB == pResType)
		{
			// 기존 Prefab 을 지우고 갱신한다 
			CResMgr::GetInst()->UpdatePrefabRes<CPrefab>(wstrResKey, pPref);

		}

	}
}

void CPortalScript::SaveInventoryToPrefab()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Inventory");
	CGameObject* pPlayerObj = pLayer->FindObj(L"Inventory");


	// RelativePath 저장은 CPRefab Save 에서 해주고 있음 
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring wstrResKey = L"prefab\\" + pPlayerObj->GetName() + L".pref";
	wstring FullPath = strContent + wstrResKey;


	CPrefab* pPref = new CPrefab;;// (pProtoObj);
	CGameObject* pCloneObj = pPlayerObj->Clone();

	pPref->SetProto(pCloneObj);

	// Prefab 추가
	if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey))
	{
		// Prefab 추가 
		CResMgr::GetInst()->AddRes<CPrefab>(wstrResKey, pPref);

	}
	// Prefab 이 이미 있을 경우 
	else
	{
		RES_TYPE pResType = CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey)->GetResType();
		if (RES_TYPE::PREFAB == pResType)
		{
			// 기존 Prefab 을 지우고 갱신한다 
			CResMgr::GetInst()->UpdatePrefabRes<CPrefab>(wstrResKey, pPref);

		}

	}

}

void CPortalScript::SaveQuickSlotToPrefab()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"UI");
	CGameObject* pPlayerObj = pLayer->FindObj(L"MainBar");


	// RelativePath 저장은 CPRefab Save 에서 해주고 있음 
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring wstrResKey = L"prefab\\" + pPlayerObj->GetName() + L".pref";
	wstring FullPath = strContent + wstrResKey;


	CPrefab* pPref = new CPrefab;;// (pProtoObj);
	CGameObject* pCloneObj = pPlayerObj->Clone();

	pPref->SetProto(pCloneObj);

	// Prefab 추가
	if (nullptr == CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey))
	{
		// Prefab 추가 
		CResMgr::GetInst()->AddRes<CPrefab>(wstrResKey, pPref);

	}
	// Prefab 이 이미 있을 경우 
	else
	{
		RES_TYPE pResType = CResMgr::GetInst()->FindRes<CPrefab>(wstrResKey)->GetResType();
		if (RES_TYPE::PREFAB == pResType)
		{
			// 기존 Prefab 을 지우고 갱신한다 
			CResMgr::GetInst()->UpdatePrefabRes<CPrefab>(wstrResKey, pPref);

		}

	}

}



void CPortalScript::SaveToScene(FILE* _pFile)
{
}

void CPortalScript::LoadFromScene(FILE* _pFile)
{


}


