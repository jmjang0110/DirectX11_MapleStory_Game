#include "pch.h"
#include "CMobGroundScript.h"

#include <Engine/CTransform.h>


CMobGroundScript::CMobGroundScript()
	: CScript((int)SCRIPT_TYPE::MOBGROUNDSCRIPT)
	, m_vScale(Vec3(0.f, 0.f, 0.f))
	, m_vPos(Vec3(0.f, 0.f, 0.f))
	, m_iMaxMonsterCnt(5)
	, m_iCurMosterCnt(0)


{
	SetName(CScriptMgr::GetScriptName(this));

}
CMobGroundScript::CMobGroundScript(const CMobGroundScript& _origin)
	: CScript((int)SCRIPT_TYPE::MOBGROUNDSCRIPT)
	, m_vScale(_origin.m_vScale)
	, m_vPos(_origin.m_vPos)
	, m_iMaxMonsterCnt(_origin.m_iMaxMonsterCnt)
	, m_iCurMosterCnt(_origin.m_iCurMosterCnt)



{
	SetName(CScriptMgr::GetScriptName(this));

}

CMobGroundScript::~CMobGroundScript()
{
}




void CMobGroundScript::start()
{
	m_vScale = GetOwner()->Transform()->GetWorldScale();
	m_vPos = GetOwner()->Transform()->GetRelativePos();

	 // 가로 700 px 당 한마리 생성 
	if (m_vScale.x >= 2100.f)
		m_iMaxMonsterCnt = (int)m_vScale.x / 700;
	else if (m_vScale.x <= 1000.f)
		m_iMaxMonsterCnt = (int)m_vScale.x / 300;
	else
		m_iMaxMonsterCnt = (int)m_vScale.x / 500;

}

void CMobGroundScript::update()
{
	
}

void CMobGroundScript::lateupdate()
{
}




void CMobGroundScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CMobGroundScript::OnCollision(CGameObject* _OtherObject)
{
}

void CMobGroundScript::OnCollisionExit(CGameObject* _OtherObject)
{
}




void CMobGroundScript::SaveToScene(FILE* _pFile)
{
}

void CMobGroundScript::LoadFromScene(FILE* _pFile)
{
}


