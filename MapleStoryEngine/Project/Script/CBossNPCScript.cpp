#include "pch.h"
#include "CBossNPCScript.h"


#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"
#include "CBossHPBarScript.h"
CBossNPCScript::CBossNPCScript()
	: CScript((int)SCRIPT_TYPE::BOSSNPCSCRIPT)
	, m_fTimer(0.f)
	, m_iClickedCnt(0)
	, m_bCreateBossMonster(false)
	, m_bEndBossMonster(false)
	, m_fChangeSceneTimer(0)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CBossNPCScript::CBossNPCScript(const CBossNPCScript& _origin)
	: CScript((int)SCRIPT_TYPE::BOSSNPCSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CBossNPCScript::~CBossNPCScript()
{
}

void CBossNPCScript::start()
{
	m_bCreateBossMonster = false;
	m_fChangeSceneTimer = 0;

}

void CBossNPCScript::update()
{
	////if (m_iClickedCnt == 0)
	//	return;

	m_fTimer += DT;
	if (m_fTimer >= 1.5f)
	{
		m_fTimer = 0.f;
		m_iClickedCnt = 0;
	}

	if (m_bCreateBossMonster == true && m_bEndBossMonster == true)
	{
		m_fChangeSceneTimer += DT;
		if (m_fChangeSceneTimer >= 8.f)
		{
			// Change Scene !!
		// 로딩할 Scene 파일의 경로 계산
			wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
			strFilePath += L"scene\\EndScene.scene";

			CScene* pNextScene = CSceneSaveLoad::LoadScene(strFilePath);
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

			CSceneMgr::GetInst()->SceneChangeEvent(pCurScene, pNextScene);


			CSceneSaveLoad::pSceneMgrScript->End();
			CSceneMgr::GetInst()->StartSceneEvent(pNextScene);
		}
	}


}

void CBossNPCScript::lateupdate()
{
}

void CBossNPCScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CBossNPCScript::OnCollision(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Cursor")
	{
		if (KEY_TAP(KEY::LBTN))
		{
			m_iClickedCnt += 1;

			if (m_iClickedCnt >= 2)
			{
				if (m_bCreateBossMonster == false)
				{
					m_iClickedCnt = 0;
					m_bCreateBossMonster = true;

					// Create Boss Monster 
					CPrefab* pMobPrefab = CSceneSaveLoad::pSceneMgrScript->GetMonsterPrefab(L"Papulatus");
					// Create Mob 
					CGameObject* pMob = pMobPrefab->Instantiate();
					pMob->start();

					CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
					pCurScene->AddObject(pMob, L"Monster");


					// Boss Hp Bar 
					CPrefab* pBarPrefab = CSceneSaveLoad::pSceneMgrScript->GetPrefab(L"BossHPBar");
					CGameObject* pMobHPBar = pBarPrefab->Instantiate();
					pMobHPBar->start();

					CBossHPBarScript* pHPBarScript = (CBossHPBarScript*)pMobHPBar->GetScriptByName(L"CBossHPBarScript");
					if (pHPBarScript != nullptr)
						pHPBarScript->SetBossNPCAddress(this);

					pCurScene->AddObject(pMobHPBar, L"UI");

					CSceneSaveLoad::pSceneMgrScript->ChangeBackGroundBgm(L"sound\\TimeAttack.mp3");


				}
			

			}
		}
	}

}

void CBossNPCScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CBossNPCScript::SaveToScene(FILE* _pFile)
{
}

void CBossNPCScript::LoadFromScene(FILE* _pFile)
{
}
