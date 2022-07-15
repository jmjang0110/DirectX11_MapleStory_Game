#include "pch.h"
#include "CAIScript.h"

#include "CStateScript.h"
#include "CIdleStateScript.h"	
#include "CTraceStateScript.h"
#include "CAttackStateScript.h"
#include "CDeadStateSCript.h"
#include "CPatrolStateScript.h"


CAIScript::CAIScript()
	: CScript((int)SCRIPT_TYPE::AISCRIPT)
	, m_pCurState(nullptr)
{
	SetName(CScriptMgr::GetScriptName(this));


}

CAIScript::CAIScript(const CAIScript& _origin)
	: CScript((int)SCRIPT_TYPE::AISCRIPT)
	, m_pCurState(_origin.m_pCurState)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CAIScript::~CAIScript()
{
	Safe_Del_Map(m_mapState);

}



void CAIScript::AddState(CStateScript* _pState)
{
	if (_pState == nullptr)
		return;

	_pState->SetOwner(GetOwner());
	MONSTER_STATE eMonState = _pState->GetMonsterStateType();
	m_mapState.insert(make_pair(eMonState, _pState));
	_pState->m_pAI = this;

	//CStateScript* pStateScript = GetStateScript(eMonState);
	//assert(!pStateScript);


}

CStateScript* CAIScript::GetStateScript(MONSTER_STATE _eState)
{
	map<MONSTER_STATE, CStateScript*>::iterator iter = m_mapState.find(_eState);
	if (iter == m_mapState.end())
		return nullptr;


	return iter->second;


}

void CAIScript::SetCurState(MONSTER_STATE _eState)
{
	m_pCurState = GetStateScript(_eState);
	m_pCurState->Enter();

	assert(m_pCurState);

}

void CAIScript::ChangeState(MONSTER_STATE _eNextState)
{
	CStateScript* pNextState = GetStateScript(_eNextState);

	if(m_pCurState->GetMonsterStateType() == _eNextState)
		return;

	m_pCurState->Exit();
	m_pCurState = pNextState;
	m_pCurState->Enter();


}

void CAIScript::start()
{
	Safe_Del_Map(m_mapState);

	AddState(new CIdleStateScript);
	AddState(new CTraceStateScript);
	AddState(new CAttackStateScript);
	AddState(new CDeadStateScript);
	AddState(new CPatrolStateScript);

	SetCurState(MONSTER_STATE::IDLE);
}

void CAIScript::update()
{
	m_pCurState->update();

}

void CAIScript::lateupdate()
{
	m_pCurState->lateupdate();

}

void CAIScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CAIScript::OnCollision(CGameObject* _OtherObject)
{
}

void CAIScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CAIScript::SaveToScene(FILE* _pFile)
{

}

void CAIScript::LoadFromScene(FILE* _pFile)
{

}
