#include "pch.h"
#include "CConsumeScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>


CConsumeScript::CConsumeScript()
	: CScript((int)SCRIPT_TYPE::CONSUMESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CConsumeScript::CConsumeScript(const CConsumeScript& _origin)
	: CScript((int)SCRIPT_TYPE::CONSUMESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CConsumeScript::~CConsumeScript()
{
}


void CConsumeScript::start()
{
}

void CConsumeScript::update()
{
}

void CConsumeScript::lateupdate()
{
}

void CConsumeScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CConsumeScript::OnCollision(CGameObject* _OtherObject)
{
}

void CConsumeScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

