#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pMissilePrefab(nullptr)
	, m_fSpeed(10.f)
	, m_fBurnStrength(0.f)
	, m_bBurn(false)
{
	//SetName(L"CPlayerScript");
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);


	m_fMass = 1.f;
	m_vAccel = Vec3(1.f, 1.f, 1.f);



}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::start()
{
	//m_pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
}

void CPlayerScript::update()
{
	//// 힘의 크기 
	float fForce = m_vForce.Length();

	if (0.f != fForce)
	{
		// 힘의 방향 
		m_vForce.Normalize();

		// 가속도의 크기 F = M * A
		float fAccel = fForce / m_fMass;

		// 가속도 
		m_vAccel = m_vForce * fAccel;

		// 속도
		m_vVelocity += m_vAccel * DT;
	}

	//

	// 속도에 따른 이동 
	

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
		m_vForce += Vec3(-200.f, 0.f, 0.f);

	if (KEY_PRESSED(KEY::RIGHT))
		m_vForce += Vec3(200.f, 0.f, 0.f);
	if (KEY_PRESSED(KEY::UP))
		m_vForce += Vec3(0.f, 200.f, 0.f);
	if (KEY_PRESSED(KEY::DOWN))
		m_vForce += Vec3(0.f, -200.f, 0.f);

	Move();

	//if (KEY_PRESSED(KEY::LEFT))
	//	vPos.x -= DT * 100.f;

	//if (KEY_PRESSED(KEY::RIGHT))
	//	vPos.x += DT * 100.f;

	//if (KEY_PRESSED(KEY::UP))	
	//	vPos.y += DT * 100.f;

	//if (KEY_PRESSED(KEY::DOWN))
	//	vPos.y -= DT * 100.f;	

	//Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.z += DT * XM_2PI;
		Transform()->SetRelativeRotation(vRot);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		//GetOqwner()->Destroy();
		//GetOwner()->GetChild().at(0)->Destroy();

		//GetOwner()->GetChild().at(0)->Destroy();
		//GetOwner()->Destroy();

		//GetOwner()->GetChild().at(0)->Destroy();
		//GetOwner()->Destroy();

		/*if (nullptr != m_pMissilePrefab)
		{
			CGameObject* pMissileObject = m_pMissilePrefab->Instantiate();

			Vec3 vMissilePos = Transform()->GetRelativePos();
			vMissilePos.y += Transform()->GetRelativeScale().y / 2.f;

			CSceneMgr::GetInst()->SpawnObject(pMissileObject, vMissilePos, L"Missile", 0);
		}		*/
	}

	if (KEY_TAP(KEY::B))
	{
		m_bBurn = true;
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		Vec4 vColor(1.f, 0.75f, 0.5f, 0.f);
		pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
	}

	Burnning();

	//m_vForce = Vec3(0.f, 0.f, 0.f);

}

void CPlayerScript::lateupdate()
{

}


void CPlayerScript::Burnning()
{
	if (!m_bBurn)
		return;

	m_fBurnStrength += DT * (1.f / 3.f);

	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fBurnStrength);
}




void CPlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Missile")
	{
		_OtherObject->Destroy();
	}
}
    


void CPlayerScript::Move()
{

	// 이동 속력
	float fSpeed = m_vVelocity.Length();

	if (0.f != fSpeed)
	{
		// 이동 방향 
		Vec3 vDir = m_vVelocity;
		
		vDir.Normalize();
		
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

		vPos += m_vVelocity * DT;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}



}
