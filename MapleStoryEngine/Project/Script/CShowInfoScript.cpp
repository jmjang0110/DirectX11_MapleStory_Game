#include "pch.h"
#include "CShowInfoScript.h"



CShowInfoScript::CShowInfoScript()
	: CScript((int)SCRIPT_TYPE::SHOWINFOSCRIPT)

{

	SetName(CScriptMgr::GetScriptName(this));

}

CShowInfoScript::CShowInfoScript(const CShowInfoScript& _origin)
	: CScript((int)SCRIPT_TYPE::SHOWINFOSCRIPT)


{
	SetName(CScriptMgr::GetScriptName(this));

}

CShowInfoScript::~CShowInfoScript()
{
}

void CShowInfoScript::start()
{
}

void CShowInfoScript::update()
{
}

void CShowInfoScript::lateupdate()
{
}

void CShowInfoScript::OnCollisionEnter(CGameObject* _OtherObject)
{


}

void CShowInfoScript::OnCollision(CGameObject* _OtherObject)
{


}

void CShowInfoScript::OnCollisionExit(CGameObject* _OtherObject)
{


}

void CShowInfoScript::SaveToScene(FILE* _pFile)
{


}

void CShowInfoScript::LoadFromScene(FILE* _pFile)
{


}
