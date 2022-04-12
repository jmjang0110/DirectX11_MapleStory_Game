#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
	: m_pMissilePrefab(nullptr)
	, m_fSpeed(0.5f)
{

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
	Vec3 vPos = Transform()->GetPos();

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * 100.f;

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * 100.f;

	if (KEY_PRESSED(KEY::UP))	
		vPos.y += DT * 100.f;

	if (KEY_PRESSED(KEY::DOWN))
		vPos.y -= DT * 100.f;

	Transform()->SetPos(vPos);

	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRotation();
		vRot.z += DT * XM_2PI;
		Transform()->SetRotation(vRot);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		if (nullptr != m_pMissilePrefab)
		{
			CGameObject* pMissileObject = m_pMissilePrefab->Instantiate();

			Vec3 vMissilePos = Transform()->GetPos();
			vMissilePos.y += Transform()->GetScale().y / 2.f;

			CSceneMgr::GetInst()->SpawnObject(pMissileObject, vMissilePos, L"Missile", 0);
		}		
	}
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
