#include "pch.h"
#include "CRigidBodyScript.h"
#include "CPlayerScript.h" 
#include "CGravityScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>


CRigidBodyScript::CRigidBodyScript()
	: CScript((int)SCRIPT_TYPE::RIGIDBODYSCRIPT)
	, m_vForce(Vec3(0.f, 0.f, 0.f))
	, m_vAccel(Vec3(0.f, 0.f, 0.f))
	, m_vAccelA(Vec3(0.f, 0.f, 0.f))
	, m_vVelocity(Vec3(0.f, 0.f, 0.f))
	, m_vMaxVelocity(Vec3(150.f, 400.f, 0.f))
	, m_fMass(1.f)
	, m_fFricCoeff(200.f)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CRigidBodyScript::CRigidBodyScript(const CRigidBodyScript& _origin)
	: CScript((int)SCRIPT_TYPE::RIGIDBODYSCRIPT)
	, m_vForce(Vec3(0.f, 0.f, 0.f))
	, m_vAccel(Vec3(0.f, 0.f, 0.f))
	, m_vAccelA(Vec3(0.f, 0.f, 0.f))
	, m_vVelocity(Vec3(0.f, 0.f, 0.f))
	, m_vMaxVelocity(Vec3(150.f, 400.f, 0.f))
	, m_fMass(1.f)
	, m_fFricCoeff(200.f)
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


	if (0.f != m_vForce.Length())
	{
		// Velocity 
		m_vAccel = m_vForce / m_fMass;
	}

	m_vAccel += m_vAccelA;								// 추가 가속도 ex) 중력 
	m_vVelocity += m_vAccel * DT;						// 가속도에 따른 속도 변화 

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

  
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		m_vVelocity.x = (m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);	// 최대 속도  제한 (x축)
	}
	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	{
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);	// 최대 속도  제한 (y축)
	}
	if (abs(m_vMaxVelocity.z) < abs(m_vVelocity.z))
	{
		m_vVelocity.z = (m_vVelocity.z / abs(m_vVelocity.z)) * abs(m_vMaxVelocity.z);	// 최대 속도  제한 (z축)
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
	m_vAccel = Vec3(0.f, 0.f, 0.f);						// 가속도 초기화 
	m_vAccelA = Vec3(0.f, 0.f, 0.f);					// 추가 가속도 초기화
}

void CRigidBodyScript::lateupdate()
{
	

}

void CRigidBodyScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CRigidBodyScript::OnCollision(CGameObject* _OtherObject)
{
}

void CRigidBodyScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CRigidBodyScript::SaveToScene(FILE* _pFile)
{
}

void CRigidBodyScript::LoadFromScene(FILE* _pFile)
{
}

