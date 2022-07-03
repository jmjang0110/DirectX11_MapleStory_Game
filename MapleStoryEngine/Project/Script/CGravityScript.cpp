#include "pch.h"
#include "CGravityScript.h"

#include "CRigidBodyScript.h"	
#include "CPlayerScript.h"
#include <Engine/CCollider2D.h>



CGravityScript::CGravityScript()
	:CScript((int)SCRIPT_TYPE::GRAVITYSCRIPT)
	, m_bGround(false)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CGravityScript::CGravityScript(const CGravityScript& _origin)
	: CScript((int)SCRIPT_TYPE::GRAVITYSCRIPT)
	, m_bGround(_origin.m_bGround)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CGravityScript::~CGravityScript()
{

}



void CGravityScript::SetOnGround(bool _b)
{
	CPlayerScript* playerScript = (CPlayerScript*)GetOwner()->GetScriptByName(L"CPlayerScript");
	CRigidBodyScript* rigidBodyScript = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");


	m_bGround = _b;
	if (m_bGround)
	{
		if (nullptr != rigidBodyScript)
		{
			Vec3 vVelocity = rigidBodyScript->GetVelocity();
			rigidBodyScript->SetVelocity(Vec3(vVelocity.x, 0.f, vVelocity.z));
		}
		
	}

}

void CGravityScript::start()
{
	m_bGround = false;
	Collider2D()->Reset();

}

void CGravityScript::update()
{
	CPlayerScript* playerScript = (CPlayerScript*)GetOwner()->GetScriptByName(L"CPlayerScript");
	CRigidBodyScript* rigidBodyScript = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");

	if (nullptr != rigidBodyScript)
	{
		if (nullptr != playerScript)
		{
			bool bPlayerOnGround = playerScript->GetOnGround();
			if(bPlayerOnGround == false)
				rigidBodyScript->SetAccelAlpha(Vec3(0.f, -1000.f, 0.f));
		}
		else
		{
			if (m_bGround == false)
				rigidBodyScript->SetAccelAlpha(Vec3(0.f, -1000.f, 0.f));

		}
	}

	
}

void CGravityScript::lateupdate()
{

}

void CGravityScript::OnCollisionEnter(CGameObject* _OtherObject)
{

}

void CGravityScript::OnCollision(CGameObject* _OtherObject)
{
}

void CGravityScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CGravityScript::SaveToScene(FILE* _pFile)
{

}

void CGravityScript::LoadFromScene(FILE* _pFile)
{

}
