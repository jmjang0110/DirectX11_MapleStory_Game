#include "pch.h"
#include "CPatrolStateScript.h"

CPatrolStateScript::CPatrolStateScript()
	: CStateScript((int)SCRIPT_TYPE::PATROLSTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::PATROL);

}

CPatrolStateScript::CPatrolStateScript(const CPatrolStateScript& _origin)
	: CStateScript((int)SCRIPT_TYPE::PATROLSTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::PATROL);

}
CPatrolStateScript::~CPatrolStateScript()
{
}

void CPatrolStateScript::Enter()
{
}

void CPatrolStateScript::Exit()
{
}

void CPatrolStateScript::start()
{
}

void CPatrolStateScript::update()
{
}

void CPatrolStateScript::lateupdate()
{
}

void CPatrolStateScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CPatrolStateScript::OnCollision(CGameObject* _OtherObject)
{
}

void CPatrolStateScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CPatrolStateScript::SaveToScene(FILE* _pFile)
{
}

void CPatrolStateScript::LoadFromScene(FILE* _pFile)
{
}


