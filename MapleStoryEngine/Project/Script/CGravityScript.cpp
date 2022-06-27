#include "pch.h"
#include "CGravityScript.h"

#include "CRigidBodyScript.h"	


CGravityScript::CGravityScript()
	:CScript((int)SCRIPT_TYPE::GRAVITYSCRIPT)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CGravityScript::~CGravityScript()
{
}



void CGravityScript::start()
{
}

void CGravityScript::update()
{
}

void CGravityScript::lateupdate()
{
}

void CGravityScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CGravityScript::SaveToScene(FILE* _pFile)
{
}

void CGravityScript::LoadFromScene(FILE* _pFile)
{
}
