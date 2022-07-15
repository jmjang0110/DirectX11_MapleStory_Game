#include "pch.h"
#include "CBackGroundScript.h"


CBackGroundScript::CBackGroundScript()
	: CScript((int)SCRIPT_TYPE::BACKGROUNDSCRIPT)
	, m_fSpeed(100.f)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CBackGroundScript::CBackGroundScript(const CBackGroundScript& _origin)
	: CScript((int)SCRIPT_TYPE::BACKGROUNDSCRIPT)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CBackGroundScript::~CBackGroundScript()
{
}


void CBackGroundScript::start()
{
}

void CBackGroundScript::update()
{

	//Vec3 vPos = Transform()->GetRelativePos();
	//float fSpeed = m_fSpeed;

	//if (KEY_PRESSED(KEY::UP))
	//{
	//	vPos.y += DT * fSpeed;
	//}

	//if (KEY_PRESSED(KEY::DOWN))
	//{
	//	vPos.y -= DT * fSpeed;
	//}

	//if (KEY_PRESSED(KEY::LEFT))
	//{
	//	vPos.x -= DT * fSpeed;
	//}

	//if (KEY_PRESSED(KEY::RIGHT))
	//{
	//	vPos.x += DT * fSpeed;
	//}

	//Transform()->SetRelativePos(vPos);

}

void CBackGroundScript::lateupdate()
{
}

void CBackGroundScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CBackGroundScript::OnCollision(CGameObject* _OtherObject)
{
}

void CBackGroundScript::OnCollisionExit(CGameObject* _OtherObject)
{
}


void CBackGroundScript::SaveToScene(FILE* _pFile)
{
}

void CBackGroundScript::LoadFromScene(FILE* _pFile)
{
}

