#include "pch.h"
#include "CDeadStateScript.h"

CDeadStateScript::CDeadStateScript()
	: CStateScript((int)SCRIPT_TYPE::DEADSTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::DEAD);

}

CDeadStateScript::CDeadStateScript(const CDeadStateScript& _origin)
	: CStateScript((int)SCRIPT_TYPE::DEADSTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::DEAD);
}

CDeadStateScript::~CDeadStateScript()
{
}



void CDeadStateScript::Enter()
{
}

void CDeadStateScript::Exit()
{
}

void CDeadStateScript::start()
{
}

void CDeadStateScript::update()
{
}

void CDeadStateScript::lateupdate()
{
}

void CDeadStateScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CDeadStateScript::OnCollision(CGameObject* _OtherObject)
{
}

void CDeadStateScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CDeadStateScript::SaveToScene(FILE* _pFile)
{
}

void CDeadStateScript::LoadFromScene(FILE* _pFile)
{
}

