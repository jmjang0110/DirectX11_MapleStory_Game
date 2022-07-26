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

	InsertPrefab(L"Damage");
}



void CSceneStartScript::InsertPrefab_RelatedPlayer()
{

	InsertPrefab(L"DoubleJumpEffect");
	InsertPrefab(L"DoubleJumpSubEffect");
	InsertPrefab(L"StormPrepare");
	InsertPrefab(L"Storm");
	InsertPrefab(L"StormEnd");
	InsertPrefab(L"ArrowFlatterPrepare");
	InsertPrefab(L"ArrowFlatter");
	InsertPrefab(L"ArrowFlatterEnd");
	InsertPrefab(L"FinalBlow");
	InsertPrefab(L"BasicBall");
	InsertPrefab(L"ArrowFlatterBall");


}

void CSceneStartScript::InsertPrefab_RelatedMonster(wstring _SceneName)
{
	
	if (_SceneName == L"Osolgil")
	{
		InsertMonsterPrefab(L"GiganticBiking");
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
	m_pOwner = GetOwner();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CSceneSaveLoad::LoadMainPlayer(pCurScene);

	LoadDefaultPrefab();
	InsertPrefab_RelatedPlayer();
	InsertPrefab_RelatedMonster(CSceneMgr::GetInst()->GetCurScene()->GetName());


	CSceneSaveLoad::pSceneManagerObj = GetOwner();
	CSceneSaveLoad::pSceneMgrScript = this;

}

void CSceneStartScript::update()
{
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

