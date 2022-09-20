#include "pch.h"
#include "CSceneStartScript.h"

#include "CSceneSaveLoad.h"



CSceneStartScript::CSceneStartScript()
	: CScript((int)SCRIPT_TYPE::SCENESTARTSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CSceneStartScript::CSceneStartScript(const CSceneStartScript& _origin)
	: CScript((int)SCRIPT_TYPE::SCENESTARTSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CSceneStartScript::~CSceneStartScript()
{
	Safe_Del_Map(m_MapPrefab);
	Safe_Del_Map(m_MapMonsterPrefab);

	if (pSound_Bgm != nullptr)
		pSound_Bgm->Stop();

	CSceneSaveLoad::pSceneManagerObj = nullptr;
	CSceneSaveLoad::pSceneMgrScript = nullptr;

}



void CSceneStartScript::InsertPrefab(wstring _name)
{
	wstring strPrefabKey = L"prefab\\" + _name;
	strPrefabKey += L".pref";
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring FullPath = strContent + strPrefabKey;

	CPrefab* NewPrefab = new CPrefab;
	NewPrefab->Load(FullPath);
	assert(NewPrefab);

	m_MapPrefab.insert(make_pair(_name, NewPrefab));

}


void CSceneStartScript::LoadDefaultPrefab()
{
	// Damage
	InsertPrefab(L"Damage");


	// Portion 
	InsertPrefab(L"RedPortion");

	InsertPrefab(L"BluePortion");
	InsertPrefab(L"OrangePortion");
	InsertPrefab(L"WhitePortion");
	InsertPrefab(L"ElixirPortion");
	InsertPrefab(L"PowerElixirPortion");
	InsertPrefab(L"MpElixirPortion");

	InsertPrefab(L"RedPortion_Info");

	InsertPrefab(L"BluePortion_Info");
	//InsertPrefab(L"OrangePortion_Info");
	InsertPrefab(L"WhitePortion_Info");
	InsertPrefab(L"ElixirPortion_Info");
	InsertPrefab(L"PowerElixirPortion_Info");
	InsertPrefab(L"MpElixirPortion_Info");

}



void CSceneStartScript::InsertPrefab_RelatedPlayer()
{

	InsertPrefab(L"DoubleJumpEffect");
	InsertPrefab(L"DoubleJumpSubEffect");

	InsertPrefab(L"StormPrepare");
	InsertPrefab(L"Storm");
	InsertPrefab(L"StormEnd");
	InsertPrefab(L"StormHit");


	InsertPrefab(L"ArrowFlatterPrepare");
	InsertPrefab(L"ArrowFlatter");
	InsertPrefab(L"ArrowFlatterEnd");

	InsertPrefab(L"FinalBlow");

	InsertPrefab(L"BasicBall");
	InsertPrefab(L"ArrowFlatterBall");


	InsertPrefab(L"LargeSkill");
	InsertPrefab(L"LargeSkillHit");


	InsertPrefab(L"TimeOutSkill");
	InsertPrefab(L"GiganticBall");
	InsertPrefab(L"GiganticBallHit");
	InsertPrefab(L"TanatosBall");
	InsertPrefab(L"TanatosBallHit");

	InsertPrefab(L"PapulatusSkill");
	InsertPrefab(L"PapulatusSkillHit");

	InsertPrefab(L"RevivalEffect");
	InsertPrefab(L"DieNotice");

	InsertPrefab(L"Number");
	InsertPrefab(L"LevelUpEffect");
	InsertPrefab(L"LevelUp2");


	InsertPrefab(L"BossHPBar");






}

void CSceneStartScript::InsertPrefab_RelatedMonster(wstring _SceneName)
{
	
	if (_SceneName == L"Osolgil")
	{
		InsertMonsterPrefab(L"GiganticBiking");
		InsertMonsterPrefab(L"Papulatus");
	}
	if (_SceneName == L"ClockTowerBottomFloor")
	{
		InsertMonsterPrefab(L"GiganticBiking");
		InsertMonsterPrefab(L"Papulatus");
	}
	if (_SceneName == L"ClockTowerOriginFloor")
	{
		InsertMonsterPrefab(L"Tanatos");
		InsertMonsterPrefab(L"Papulatus");
	}

}

void CSceneStartScript::InsertMonsterPrefab(wstring _name)
{

	wstring strPrefabKey = L"prefab\\" + _name;
	strPrefabKey += L".pref";
	wstring strContent = CPathMgr::GetInst()->GetContentPath();
	wstring FullPath = strContent + strPrefabKey;

	CPrefab* NewPrefab = new CPrefab;
	NewPrefab->Load(FullPath);
	assert(NewPrefab);

	m_MapMonsterPrefab.insert(make_pair(_name, NewPrefab));

}

CPrefab* CSceneStartScript::GetPrefab(wstring _name)
{
	CPrefab* _FindPref = m_MapPrefab.find(_name)->second;
	return _FindPref;

}


void CSceneStartScript::start()
{
	m_pCurScene = CSceneMgr::GetInst()->GetCurScene();

	CSceneSaveLoad::pSceneManagerObj = GetOwner();
	CSceneSaveLoad::pSceneMgrScript = this;
	m_pOwner = GetOwner();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	pCurScene->SetEditMode(false);

	// Only Maple Start Scene 
	if (pCurScene->GetName() == L"Wizet")
	{
		m_fTimer = 0.f;

		pSound_Bgm = CResMgr::GetInst()->Load<CSound>(L"sound\\WzLogo.mp3", L"sound\\WzLogo.mp3");
		pSound_Bgm->Play(0, 0.5f);

		
		return;
	}
	// Only Maple End Scene 
	if (pCurScene->GetName() == L"EndScene")
	{
		m_fTimer = 0.f;

		pSound_Bgm = CResMgr::GetInst()->Load<CSound>(L"sound\\WzLogo.mp3", L"sound\\WzLogo.mp3");
		pSound_Bgm->Play(0, 0.5f);


		return;
	}

	if (pCurScene->GetName() == L"Test")
	{
		return;
	}

	LoadDefaultPrefab();
	InsertPrefab_RelatedPlayer();
	InsertPrefab_RelatedMonster(CSceneMgr::GetInst()->GetCurScene()->GetName());





	// Load [ player / Inventory / MainBar ]
	CSceneSaveLoad::LoadMainPlayer(pCurScene);
	CSceneSaveLoad::LoadInventory(pCurScene);
	CSceneSaveLoad::LoadMainBar(pCurScene);

	CSceneMgr::GetInst()->StartSceneEvent(pCurScene);




	 if (pCurScene->GetName() == L"Osolgil")
	{
		pSound_Bgm = CResMgr::GetInst()->Load<CSound>(L"sound\\AboveTheTreetops.mp3", L"sound\\AboveTheTreetops.mp3");
		pSound_Bgm->Play(0, 0.5f);

	}
	 else if (pCurScene->GetName() == L"MapleLoad")
	 {
		 pSound_Bgm = CResMgr::GetInst()->Load<CSound>(L"sound\\TheBeginnigofTheAdventure.mp3", L"sound\\TheBeginnigofTheAdventure.mp3");
		 pSound_Bgm->Play(0, 0.5f);
	 }
	else if (pCurScene->GetName() == L"ClockTowerBottomFloor")
	{
		pSound_Bgm = CResMgr::GetInst()->Load<CSound>(L"sound\\FairyTale.mp3", L"sound\\FairyTale.mp3");
		pSound_Bgm->Play(0, 0.5f);

	}
	else if (pCurScene->GetName() == L"ClockTowerOriginFloor")
	{
		pSound_Bgm = CResMgr::GetInst()->Load<CSound>(L"sound\\FairyTalediffvers.mp3", L"sound\\FairyTalediffvers.mp3");
		pSound_Bgm->Play(0, 0.5f);

	}

	else if (pCurScene->GetName() == L"ShipToToyCastle")
	{
		pSound_Bgm = CResMgr::GetInst()->Load<CSound>(L"sound\\HighEnough.mp3", L"sound\\HighEnough.mp3");
		pSound_Bgm->Play(0, 0.5f);
 
	}
	else if (pCurScene->GetName() == L"ToyCastle")
	{
		pSound_Bgm = CResMgr::GetInst()->Load<CSound>(L"sound\\FantasticThinking.mp3", L"sound\\FantasticThinking.mp3");
		pSound_Bgm->Play(0, 0.5f);

		Ptr<CSound> pDragBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\whistle.mp3", L"sound\\whistle.mp3");
		pDragBgm->Play(1, 0.6f, true);

	}




}

void CSceneStartScript::End()
{

	if(pSound_Bgm != nullptr)
		pSound_Bgm->Stop();

	Vec3 vPos;
	if (CSceneSaveLoad::pMainPlayer != nullptr)
		vPos = CSceneSaveLoad::pMainPlayer->Transform()->GetRelativePos();


	Vec3 vCamPos = CSceneSaveLoad::pMainCamera->Transform()->GetRelativePos();
	vCamPos.x = vPos.x;
	vCamPos.y = vPos.y;
	CSceneSaveLoad::pMainCamera->Transform()->SetRelativePos(vCamPos);


}


void CSceneStartScript::update()
{
	m_fTimer += DT;
	if (m_pCurScene->GetName() == L"Wizet")
	{

		// wIzet -> Maple Load 
		if (m_fTimer >= 6.0f)
		{
			// Change Scene !!
			// 로딩할 Scene 파일의 경로 계산
			wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
			strFilePath += L"scene\\MapleLoad.scene";

			CScene* pNextScene = CSceneSaveLoad::LoadScene(strFilePath);
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CSceneMgr::GetInst()->SceneChangeEvent(pCurScene, pNextScene);
			CSceneSaveLoad::pSceneMgrScript->End();
			CSceneMgr::GetInst()->StartSceneEvent(pNextScene);
		}
	}

	if (m_fTimer >= 1.f)
		CSound::UpdateFMOD();



	if (KEY_TAP(KEY::P))
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		bool EditMode = pCurScene->GetEditMode();
		pCurScene->SetEditMode(!EditMode);
	}
	

}

CPrefab* CSceneStartScript::GetMonsterPrefab(wstring _name)
{
	CPrefab* _FindPref = m_MapMonsterPrefab.find(_name)->second;
	return _FindPref;
}



CPrefab* CSceneStartScript::GetMonsterPrefab(int idx)
{
	int prefabSize = m_MapMonsterPrefab.size();

	if (idx < prefabSize)
	{
		map<wstring, CPrefab*>::iterator iter = m_MapMonsterPrefab.begin();
		for (int i = 0; i < idx; i++)
			iter++;
		
		return iter->second;
	}
}

void CSceneStartScript::SavePlayerToPrefab(Vec2 _vnextScenePlayerpos)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Player");
	CGameObject* pPlayerObj = pLayer->FindObj(L"player");

	// NextScene 에서 Player 의 위치를 미리 조정 
	Vec3 vPos = pPlayerObj->Transform()->GetRelativePos();
	vPos.x = _vnextScenePlayerpos.x;
	vPos.y = _vnextScenePlayerpos.y;
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

void CSceneStartScript::SaveInventoryToPrefab()
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

void CSceneStartScript::SaveQuickSlotToPrefab()
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

void CSceneStartScript::ChangeBackGroundBgm(wstring _BgnName)
{
	pSound_Bgm->Stop();
	pSound_Bgm = CResMgr::GetInst()->Load<CSound>(_BgnName, _BgnName);

	pSound_Bgm->Play(0, 0.5f);

}





void CSceneStartScript::lateupdate()
{
}

void CSceneStartScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CSceneStartScript::OnCollision(CGameObject* _OtherObject)
{
}

void CSceneStartScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CSceneStartScript::SaveToScene(FILE* _pFile)
{
}

void CSceneStartScript::LoadFromScene(FILE* _pFile)
{
}

