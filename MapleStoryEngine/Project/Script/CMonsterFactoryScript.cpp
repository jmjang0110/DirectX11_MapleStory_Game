#include "pch.h"
#include "CMonsterFactoryScript.h"



CMonsterFactoryScript::CMonsterFactoryScript()
	:CScript((int)SCRIPT_TYPE::MONSTERFACTORYSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CMonsterFactoryScript::CMonsterFactoryScript(const CMonsterFactoryScript& _origin)
	: CScript((int)SCRIPT_TYPE::MONSTERFACTORYSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CMonsterFactoryScript::~CMonsterFactoryScript()
{
}



void CMonsterFactoryScript::start()
{
}

void CMonsterFactoryScript::update()
{
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

