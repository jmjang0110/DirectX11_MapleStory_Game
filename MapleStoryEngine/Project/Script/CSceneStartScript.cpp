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
}



void CSceneStartScript::start()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CSceneSaveLoad::LoadMainPlayer(pCurScene);



}

void CSceneStartScript::update()
{
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

