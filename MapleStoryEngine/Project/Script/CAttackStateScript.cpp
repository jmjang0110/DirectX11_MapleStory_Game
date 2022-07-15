#include "pch.h"
#include "CAttackStateScript.h"


CAttackStateScript::CAttackStateScript()
	: CStateScript((int)SCRIPT_TYPE::ATTACKSTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::ATT);
}


CAttackStateScript::CAttackStateScript(const CAttackStateScript& _origin)
	: CStateScript((int)SCRIPT_TYPE::ATTACKSTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::ATT);
}

CAttackStateScript::~CAttackStateScript()
{
}


void CAttackStateScript::Enter()
{
}

void CAttackStateScript::Exit()
{
}

void CAttackStateScript::start()
{
}

void CAttackStateScript::update()
{
}

void CAttackStateScript::lateupdate()
{
}

void CAttackStateScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CAttackStateScript::OnCollision(CGameObject* _OtherObject)
{
}

void CAttackStateScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CAttackStateScript::SaveToScene(FILE* _pFile)
{
}

void CAttackStateScript::LoadFromScene(FILE* _pFile)
{
}


