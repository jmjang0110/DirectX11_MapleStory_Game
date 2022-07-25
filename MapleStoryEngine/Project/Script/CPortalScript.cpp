#include "pch.h"
#include "CPortalScript.h"

#include "CSceneSaveLoad.h"
#include <Engine/CTransform.h>



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
	if (GetOwner()->GetName() == L"Osolgil2VicportTown")
	{
		m_wCurScene_RelativePath = L"Scene\\Osolgil.scene";
		m_wNextScene_RelativePath = L"Scene\\VicportTown.scene";

		m_vNextSceneStartPos = Vec2(2424, -914);

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
		// 로딩할 Scene 파일의 경로 계산
		wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
		strFilePath += m_wNextScene_RelativePath;

		m_pNextScene = CSceneSaveLoad::LoadScene(strFilePath);
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

		CSceneMgr::GetInst()->SceneChangeEvent(pCurScene, m_pNextScene);
		SavePlayerToPrefab();

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



void CPortalScript::SaveToScene(FILE* _pFile)
{
}

void CPortalScript::LoadFromScene(FILE* _pFile)
{


}


