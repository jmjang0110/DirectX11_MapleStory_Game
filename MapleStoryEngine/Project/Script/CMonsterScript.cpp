#include "pch.h"
#include "CMonsterScript.h"

#include "CAIScript.h"



CMonsterScript::CMonsterScript()
	: CScript((int)SCRIPT_TYPE::MONSTERSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CMonsterScript::CMonsterScript(const CMonsterScript& _origin)
	: CScript((int)SCRIPT_TYPE::MONSTERSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CMonsterScript::~CMonsterScript()
{
}





void CMonsterScript::start()
{
}

void CMonsterScript::update()
{
}

void CMonsterScript::lateupdate()
{
}

void CMonsterScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CMonsterScript::OnCollision(CGameObject* _OtherObject)
{
}

void CMonsterScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CMonsterScript::SaveToScene(FILE* _pFile)
{
}

void CMonsterScript::LoadFromScene(FILE* _pFile)
{
}

