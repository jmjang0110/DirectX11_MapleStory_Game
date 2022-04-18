#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"



CPlayerScript::CPlayerScript()
	: m_pMissilePrefab(nullptr)
	, m_fSpeed(0.5f)
	, m_bJump(false)
{
	m_sJumpTool.velocity = 7.f;
	m_sJumpTool.mass = 2.f;
	m_sJumpTool.Force = 0.f;


}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::start()
{
	m_pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
}

void CPlayerScript::update()
{
	
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * 100.f;

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * 100.f;

	// 점프 기능 
	if (KEY_TAP(KEY::SPACE))
	{
		m_bJump = true;

	}

	if (m_bJump == true)
		Jump(vPos);



	Transform()->SetPos(vPos);

	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.z += DT * XM_2PI;
		Transform()->SetRotation(vRot);
	}
	/*
	if (KEY_TAP(KEY::SPACE))
	{
		if (nullptr != m_pMissilePrefab)
		{
			CGameObject* pMissileObject = m_pMissilePrefab->Instantiate();

			Vec3 vMissilePos = Transform()->GetPos();
			vMissilePos.y += Transform()->GetScale().y / 2.f;

			CSceneMgr::GetInst()->SpawnObject(pMissileObject, vMissilePos, L"Missile", 0);
		}		
	}*/
}

void CPlayerScript::lateupdate()
{

}

void CPlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Missile")
	{
		int a = 0;
	}

	// C++ RTTI 를 이용한 타입 구분
	if (typeid(CMissileScript).hash_code() == typeid(*_OtherObject->GetScript()).hash_code())
	{
		int a = 0;
	}

	CMissileScript* pMissileScript = dynamic_cast<CMissileScript*>(_OtherObject->GetScript());
	if (pMissileScript)
	{
		int a = 0;
	}
}



void CPlayerScript::Jump(Vec3& _vPos)
{

	if (m_sJumpTool.velocity > 0.f)
	{
		m_sJumpTool.Force = 0.5f * m_sJumpTool.mass * (m_sJumpTool.velocity * m_sJumpTool.velocity);
	}
	else if (m_sJumpTool.velocity <= 0.f)
	{
		m_sJumpTool.Force = -0.5f * m_sJumpTool.mass * (m_sJumpTool.velocity * m_sJumpTool.velocity);
	}

	m_sJumpTool.velocity -= DT * m_fSpeed;
	_vPos.y -= m_sJumpTool.velocity;


}


