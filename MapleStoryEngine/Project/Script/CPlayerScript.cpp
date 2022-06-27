#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"

#include "CScriptMgr.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CLight2D.h>

#include "CRigidBodyScript.h"


CPlayerScript::CPlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pMissilePrefab(nullptr)
	, m_fSpeed(10.f)
	, m_fBurnStrength(0.f)
	, m_bBurn(false)
	, m_eDir(PLAYER_DIRECTION::RIGHT)
	, m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::IDLE)

{
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	SetName(CScriptMgr::GetScriptName(this));


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
	static Vec3 PrevPos = Vec3(0.f, 0.f, 0.f);
	Update_Move();
	Update_Gravity();

	Update_State();
	//Update_Animation();
	// 이동 방향에 따라 CLight2D 의 Dir 을 바꾼다. 
	vector<CGameObject*> vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (nullptr != vecChild[i]->GetComponent(COMPONENT_TYPE::LIGHT2D))
		{
			Vec3 vDir = GetOwner()->Transform()->GetRelativePos() - PrevPos;
			if (vDir != Vec3(0.f, 0.f, 0.f))
				vecChild[i]->Light2D()->SetLightDir(vDir);

		}
	}
	PrevPos = GetOwner()->Transform()->GetRelativePos();
	m_ePrevState = m_eCurState;
}


//
//void CPlayerScript::update()
//{
//	static Vec3 PrevPos = Vec3(0.f, 0.f, 0.f);
//
//
//
//	
//
//	m_ePrevState = m_eCurState;
//
//
//	if (KEY_PRESSED(KEY::LEFT))
//	{
//		m_vForce += Vec3(-400.f, 0.f, 0.f);
//		m_eDir = PLAYER_DIRECTION::LEFT;
//		m_eCurState = PLAYER_STATE::WALK;
//
//
//	}
//
//	if (KEY_PRESSED(KEY::RIGHT))
//	{
//		m_vForce += Vec3(400.f, 0.f, 0.f);
//		m_eDir = PLAYER_DIRECTION::RIGHT;
//		m_eCurState = PLAYER_STATE::WALK;
//
//	}
//
//	if (KEY_PRESSED(KEY::UP))
//	{
//		m_vForce += Vec3(0.f, 400.f, 0.f);
//		m_eDir = PLAYER_DIRECTION::UP;
//		m_eCurState = PLAYER_STATE::WALK;
//
//	}
//
//	if (KEY_PRESSED(KEY::DOWN))
//	{
//		m_vForce += Vec3(0.f, -400.f, 0.f);
//		m_eDir = PLAYER_DIRECTION::DOWN;
//		m_eCurState = PLAYER_STATE::WALK;
//
//	}
//
//
//	if (KEY_AWAY(KEY::LEFT) || KEY_AWAY(KEY::RIGHT) 
//		|| KEY_AWAY(KEY::UP) || KEY_AWAY(KEY::DOWN))
//	{
//		m_eCurState = PLAYER_STATE::IDLE;
//
//	}
//
//
//
//	if (KEY_TAP(KEY::LALT))
//		int i = 0;
//
//
//	//// 힘의 크기 
//	float fForce = m_vForce.Length();
//
//	if (0.f != fForce)
//	{
//		// 힘의 방향 
//		m_vForce.Normalize();
//
//		// 가속도의 크기 F = M * A
//		float fAccel = fForce / m_fMass;
//
//		// 가속도 
//		m_vAccel = m_vForce * fAccel;
//
//		// 속도
//		m_vVelocity += m_vAccel * DT;
//
//		
//	}
//
//	// 마찰력에 의한 반대방햐응로의 가속도  적용 
//	if (!m_vVelocity.IsZero())
//	{
//		Vec3 vFricDir = -m_vVelocity;
//		vFricDir.Normalize();
//
//		Vec3 vFriction = vFricDir * m_fFricCoeff * DT;
//		if (m_vVelocity.Length() < vFriction.Length())
//		{
//			// 마찰 가속도가 본개 속도보디 더 큰 경우 
//			m_vVelocity = Vec3(0.f, 0.f, 0.f); // 멈춘다.
//
//		}
//		else
//		{
//			// 마찰력 적용 
//			m_vVelocity += vFriction;
//		}
//	}
//	
//	// 속도제한 검사
//	if (m_fMaxSpeed < m_vVelocity.Length())
//	{
//		m_vVelocity.Normalize();
//		m_vVelocity *= m_fMaxSpeed;
//
//	}
//	
//
//	Vec3 vPos = Transform()->GetRelativePos();
//
//	
//
//	Move();
//	// 힘 초기화
//	m_vForce = Vec3(0.f, 0.f, 0.f);
//
//
//
//	/*if (KEY_PRESSED(KEY::Z))
//	{
//		Vec3 vRot = Transform()->GetRelativeRotation();
//		vRot.z += DT * XM_2PI;
//		Transform()->SetRelativeRotation(vRot);
//	}
//
//	if (KEY_TAP(KEY::SPACE))
//	{
//		GetOqwner()->Destroy();
//		GetOwner()->GetChild().at(0)->Destroy();
//
//		GetOwner()->GetChild().at(0)->Destroy();
//		GetOwner()->Destroy();
//
//		GetOwner()->GetChild().at(0)->Destroy();
//		GetOwner()->Destroy();
//
//		if (nullptr != m_pMissilePrefab)
//		{
//			CGameObject* pMissileObject = m_pMissilePrefab->Instantiate();
//
//			Vec3 vMissilePos = Transform()->GetRelativePos();
//			vMissilePos.y += Transform()->GetRelativeScale().y / 2.f;
//
//			CSceneMgr::GetInst()->SpawnObject(pMissileObject, vMissilePos, L"Missile", 0);
//		}		
//	}
//
//	if (KEY_TAP(KEY::B))
//	{
//		m_bBurn = true;
//		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
//		Vec4 vColor(1.f, 0.75f, 0.5f, 0.f);
//		pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
//	}
//
//	Burnning();
//	*/
//
//
//	// ===== Todo ========
//	// 이동 방향에 따라 CLight2D 의 Dir 을 바꾼다. 
//	vector<CGameObject*> vecChild = GetOwner()->GetChild();
//	for (int i = 0; i < vecChild.size(); ++i)
//	{
//		if (nullptr != vecChild[i]->GetComponent(COMPONENT_TYPE::LIGHT2D))
//		{
//			Vec3 vDir = GetOwner()->Transform()->GetRelativePos() - PrevPos;
//			if(vDir != Vec3(0.f, 0.f ,0.f))
//				vecChild[i]->Light2D()->SetLightDir(vDir);
//
//		}
//	}
//	PrevPos = GetOwner()->Transform()->GetRelativePos();
//
//}

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
	int i = 0;

	//if (_OtherObject->GetName() == L"Missile")
	//{
	//	_OtherObject->Destroy();
	//}
}




void CPlayerScript::Update_State()
{

	if (KEY_TAP(KEY::LEFT))
	{
		m_eCurState = PLAYER_STATE::WALK;
		m_eDir = PLAYER_DIRECTION::LEFT;
	}
	if (KEY_TAP(KEY::RIGHT))
	{
		m_eCurState = PLAYER_STATE::WALK;
		m_eDir = PLAYER_DIRECTION::RIGHT;
	}
	if (KEY_TAP(KEY::UP))
	{
		m_eCurState = PLAYER_STATE::WALK;
		m_eDir = PLAYER_DIRECTION::UP;
	}
	if (KEY_TAP(KEY::DOWN))
	{
		m_eCurState = PLAYER_STATE::WALK;
		m_eDir = PLAYER_DIRECTION::DOWN;
	}

	CRigidBodyScript* pRigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");
	if (nullptr != pRigid)
	{
		float fSpeed = pRigid->GetVeclocity().Length();
		if (0.f == fSpeed)
			m_eCurState = PLAYER_STATE::IDLE;	

	}
//if (KEY_AWAY(KEY::LEFT) && KEY_AWAY(KEY::RIGHT))
//	m_eCurState = PLAYER_STATE::IDLE;
}

void CPlayerScript::Update_Move()
{
	CRigidBodyScript* pRigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");
	if (nullptr != pRigid)
	{
		// KEY_TAP
		if (KEY_TAP(KEY::LEFT))
		{
			m_eDir = PLAYER_DIRECTION::LEFT;
			pRigid->AddVelocity(Vec3(-100.f, 0.f, 0.f));		// 속도 즉시 증가 

		}
		if (KEY_TAP(KEY::RIGHT))
		{
			m_eDir = PLAYER_DIRECTION::RIGHT;
			pRigid->AddVelocity(Vec3(100.f, 0.f, 0.f));

		}
		if (KEY_TAP(KEY::UP))
		{
			m_eDir = PLAYER_DIRECTION::UP;
			pRigid->AddVelocity(Vec3(0.f, 100.f, 0.f));

		}
		if (KEY_TAP(KEY::DOWN))
		{
			m_eDir = PLAYER_DIRECTION::DOWN;
			pRigid->AddVelocity(Vec3(0.f, -100.f, 0.f));

		}


		// KEY_PRESSED 
		if (KEY_PRESSED(KEY::LEFT))
		{
			pRigid->AddForce(Vec3(-200.f, 0.f, 0.f));
		}

		if (KEY_PRESSED(KEY::RIGHT))
		{
			pRigid->AddForce(Vec3(200.f, 0.f, 0.f));
		}

		if (KEY_PRESSED(KEY::UP))
		{
			pRigid->AddForce(Vec3(0.f, 200.f, 0.f));
		}

		if (KEY_PRESSED(KEY::DOWN))
		{
			pRigid->AddForce(Vec3(0.f, -200.f, 0.f));
		}



	}

}

void CPlayerScript::Update_Animation()
{
	if (m_ePrevState == m_eCurState)
		return;

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		GetOwner()->Animator2D()->Play(L"", false);
	}

		break;
	case PLAYER_STATE::WALK:
	{
		if (PLAYER_DIRECTION::LEFT == m_eDir)
		{

		}
		else if (PLAYER_DIRECTION::RIGHT == m_eDir)
		{

		}
	}

		break;
	case PLAYER_STATE::ATTACK:

		break;
	case PLAYER_STATE::ALERT:

		break;
	case PLAYER_STATE::DEAD:

		break;
	}

}

void CPlayerScript::Update_Gravity()
{
}





void CPlayerScript::SaveToScene(FILE* _pFile)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	fread(&m_fSpeed, sizeof(float), 1, _pFile);
}

