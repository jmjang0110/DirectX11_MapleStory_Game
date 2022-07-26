#include "pch.h"
#include "CMonsterFactoryScript.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CCollider2D.h>


#include "CAIScript.h"
#include "CTraceStateScript.h"
#include "CIdleStateScript.h"

#include "CBasicBallScript.h"
#include "CDamageScript.h"
#include "CPlayerScript.h"
#include "CBossMonsterScript.h"
#include "CMobGroundScript.h"
#include "CMonsterScript.h"

#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"




CMonsterFactoryScript::CMonsterFactoryScript()
	:CScript((int)SCRIPT_TYPE::MONSTERFACTORYSCRIPT)
	, m_fTimer(0.f)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CMonsterFactoryScript::CMonsterFactoryScript(const CMonsterFactoryScript& _origin)
	: CScript((int)SCRIPT_TYPE::MONSTERFACTORYSCRIPT)
	, m_fTimer(0.f)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CMonsterFactoryScript::~CMonsterFactoryScript()
{
	//Safe_Del_Map(m_MapPrefabMob);

}



void CMonsterFactoryScript::start()
{
	// Scene 에 따라서 PrefabMob Setting 
	/*if (CSceneMgr::GetInst()->GetCurScene()->GetName() == L"Osolgil")
	{
		Safe_Del_Map(m_MapPrefabMob);

		CPrefab*	pPrefab2 = nullptr;
		wstring		strPrefabKey = L"prefab\\GiganticBiking.pref";
		wstring		strContent = CPathMgr::GetInst()->GetContentPath();
		wstring		FullPath = strContent + strPrefabKey;
		pPrefab2	= new CPrefab;
		pPrefab2->Load(FullPath);

		if (pPrefab2 != nullptr)
		{
			m_MapPrefabMob.insert(make_pair(pPrefab2->GetProto()->GetName(), pPrefab2));


		}
	}*/


}

void CMonsterFactoryScript::update()
{
	m_fTimer += DT;
	// 5 초에 한번씩 Update 
	if (m_fTimer >= 5.f)
	{
		m_fTimer = 0.f;

		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pMobGroundLayer = pCurScene->GetLayer(L"MobGround");

		vector<CGameObject*> vecMobGround = pMobGroundLayer->GetRootObjects();

		for (int i = 0; i < vecMobGround.size(); ++i)
		{
			CMobGroundScript* pMobGroundScript = (CMobGroundScript*)vecMobGround[i]->GetScriptByName(L"CMobGroundScript");
			if (pMobGroundScript == nullptr)
				continue;

			int CreateMobCnt = pMobGroundScript->GetPossibleCreateCnt();

			if(pCurScene->GetName() == L"Osolgil")
				CreateMonster(CreateMobCnt, L"GiganticBiking", pMobGroundScript);

		}


	}

}

void CMonsterFactoryScript::lateupdate()
{
}

void CMonsterFactoryScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CMonsterFactoryScript::OnCollision(CGameObject* _OtherObject)
{
}

void CMonsterFactoryScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CMonsterFactoryScript::SaveToScene(FILE* _pFile)
{
}

void CMonsterFactoryScript::LoadFromScene(FILE* _pFile)
{
}




void CMonsterFactoryScript::ChangeScene(wstring CurSceneName)
{
	// Scene 이 변경될 때  

}

void CMonsterFactoryScript::CreateMonster(int _cnt, wstring _MobName, CMobGroundScript* _script)
{
	if (_cnt == 0)
		return;

	//if (m_MapPrefabMob.empty() == true)
	//	return;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// Mob Create Ground Center , sclae 
	Vec3 vCenter = _script->GetPos();
	Vec3 vScale = _script->GetScale();

	CPrefab* pMobPrefab = CSceneSaveLoad::pSceneMgrScript->GetMonsterPrefab(_MobName);



	if (pMobPrefab != nullptr)
	{

		for (int i = 0; i < _cnt; ++i)
		{
			// Create Mob 
			CGameObject* pMob = pMobPrefab->Instantiate();
			Vec3 vPos = pMob->Transform()->GetRelativePos();

			vPos.y = vCenter.y + vScale.y + pMob->Collider2D()->GetWorldScale().y;
			vPos.x = GetRandomNum(vCenter.x - vScale.x / 2, vCenter.x + vScale.x / 2);

			pMob->Transform()->SetRelativePos(vPos);
			pMob->start();
			CMonsterScript* pMScript = (CMonsterScript*)pMob->GetScriptByName(L"CMonsterScript");
			if (pMScript != nullptr)
				pMScript->SetMobGroundScript(_script);

			pCurScene->AddObject(pMob, L"Monster");

			// setting on groundScript ( Created Monster Cnt )
			_script->AddMonsterCnt(_cnt);
		}
		
	}




}

int CMonsterFactoryScript::GetRandomNum(float _min, float _max)
{
	int diff = (int)_max - (int)_min;
	int randNum = rand() % diff;
	return _min + randNum;
}

