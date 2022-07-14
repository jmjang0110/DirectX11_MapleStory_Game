#include "pch.h"
#include "CStateScript.h"

CStateScript::CStateScript()
	: CScript((int)SCRIPT_TYPE::STATESCRIPT)
{

}


CStateScript::CStateScript(int _type)
	:CScript((int)_type)
{

}

CStateScript::CStateScript(const CStateScript& _origin)
	: CScript((int)SCRIPT_TYPE::STATESCRIPT)

{

}

CStateScript::~CStateScript()
{

}


void CStateScript::Enter()
{
}

void CStateScript::Exit()
{
}



void CStateScript::start()
{
}

void CStateScript::update()
{
}

void CStateScript::lateupdate()
{
}

void CStateScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CStateScript::OnCollision(CGameObject* _OtherObject)
{
}

void CStateScript::OnCollisionExit(CGameObject* _OtherObject)
{
}


