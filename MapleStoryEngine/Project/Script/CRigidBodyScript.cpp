#include "pch.h"
#include "CRigidBodyScript.h"

#include <Engine/CTimeMgr.h>

#include <Engine/CTransform.h>


CRigidBodyScript::CRigidBodyScript()
	: CScript((int)SCRIPT_TYPE::RIGIDBODYSCRIPT)
	, m_fMass(1.f)
	, m_fFricCoeff(100.f)
	, m_fMaxSpeed(200.f)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CRigidBodyScript::~CRigidBodyScript()
{

}

void CRigidBodyScript::start()
{

}

void CRigidBodyScript::update()
{

}

void CRigidBodyScript::lateupdate()
{
	if (0.f != m_vForce.Length())
	{
		// Velocity 
		m_vAccel = m_vForce / m_fMass;
		m_vVelocity += m_vAccel * DT;					// 가속도에 따른 속도 변화 
	}

	if (!m_vVelocity.IsZero())
	{
		Vec3 vFricDir = -m_vVelocity;
		vFricDir.Normalize();
		Vec3 vFriction = vFricDir * m_fFricCoeff * DT;  // 마찰력 계산

		if (m_vVelocity.Length() < vFriction.Length())
		{
			m_vVelocity = Vec3(0.f, 0.f, 0.f);			// 마찰력에 의한 정지 
		}
		else
		{
			m_vVelocity += vFriction;					// 마찰력 적용  
		}
	}

	if (m_fMaxSpeed < m_vVelocity.Length())
	{
		m_vVelocity.Normalize();
		m_vVelocity *= m_fMaxSpeed;						// 최대 속력 제한 
	}

	// Move 
	float fSpeed = m_vVelocity.Length();				// 이동 속력 
	if (0.f != fSpeed)
	{
		Vec3 vDir = m_vVelocity;
		vDir.Normalize();								// 이동 방향 

		Vec3 vPos = Transform()->GetRelativePos();
		vPos += m_vVelocity * DT;						// vPos += vDir * fSpeed * DT;
		Transform()->SetRelativePos(vPos);
	}


	m_vForce = Vec3(0.f, 0.f, 0.f);						// 힘 초기화 

}

void CRigidBodyScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CRigidBodyScript::SaveToScene(FILE* _pFile)
{
}

void CRigidBodyScript::LoadFromScene(FILE* _pFile)
{
}

