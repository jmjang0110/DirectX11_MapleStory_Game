#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"

#include "CScriptMgr.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CLight2D.h>

#include "CRigidBodyScript.h"


CPlayerScript::CPlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	//, m_pMissilePrefab(nullptr)
	, m_fSpeed(10.f)
	, m_eDir(PLAYER_DIRECTION::RIGHT)
	, m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::IDLE)
	, m_bOnGround(false)

{

	SetName(CScriptMgr::GetScriptName(this));


	ClearScriptParam();
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	AddScriptParam("Direction", SCRIPTPARAM_TYPE::INT, &m_eDir);
	AddScriptParam("PlayerOnGround", SCRIPTPARAM_TYPE::INT, &m_bOnGround);

}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin)
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(_origin.m_fSpeed)
	, m_eDir(_origin.m_eDir)
	, m_eCurState(_origin.m_eCurState)
	, m_ePrevState(_origin.m_ePrevState)
	, m_bOnGround(false)

{
	SetName(CScriptMgr::GetScriptName(this));


	ClearScriptParam();
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	AddScriptParam("Direction", SCRIPTPARAM_TYPE::INT, &m_eDir);
	AddScriptParam("PlayerOnGround", SCRIPTPARAM_TYPE::INT, &m_bOnGround);

}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::start()
{
	//m_pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
	m_eDir = PLAYER_DIRECTION::RIGHT;
	m_eCurState = PLAYER_STATE::IDLE;
	m_ePrevState = PLAYER_STATE::IDLE;

	ClearScriptParam();
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	AddScriptParam("Direction", SCRIPTPARAM_TYPE::INT, &m_eDir);
	AddScriptParam("PlayerOnGround", SCRIPTPARAM_TYPE::INT, &m_bOnGround);
}

void CPlayerScript::update()
{
	
	Update_Move();
	Update_Gravity();

	Update_State();
	//Update_Animation();
	
	m_ePrevState = m_eCurState;
}


//
//void CPlayerScript::update()
//{
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

void CPlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	int i = 0;

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

	if (KEY_TAP(KEY::SPACE))
	{
		m_eCurState = PLAYER_STATE::JUMP;
		m_eDir = PLAYER_DIRECTION::UP;
	}

	
	CRigidBodyScript* pRigid = (CRigidBodyScript*)GetOwner()->GetScriptByName(L"CRigidBodyScript");
	if (nullptr != pRigid)
	{
		float fSpeed = pRigid->GetVelocity().Length();
		if (0.f == fSpeed)
			m_eCurState = PLAYER_STATE::IDLE;	

	}
//if (KEY_AWAY(KEY::LEFT) && KEY_AWAY(KEY::RIGHT))
//	m_eCurState = PLAYER_STATE::IDLE;
}

void CPlayerScript::Update_Move()
{
	if (KEY_TAP(KEY::ENTER))
	{
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 0.f, vPos.z));
	}


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
			if (false == m_bOnGround) // 땅위에 있는 상태에서는 더이상 내려가지 못하게 막는다. 
			{
				m_eDir = PLAYER_DIRECTION::DOWN;
				pRigid->AddVelocity(Vec3(0.f, -100.f, 0.f));

			}
			
			
		}
		if (KEY_TAP(KEY::SPACE))
		{			
			if (nullptr != pRigid)
			{
				// pRigid->AddVelocity(Vec3(pRigid->GetVelocity().x , 400.f, 0.f)); // 앞으로 확감=
				pRigid->AddVelocity(Vec3(0.f, 400.f, 0.f));

			}
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
			if (false == m_bOnGround) 
			{
				pRigid->AddForce(Vec3(0.f, -200.f, 0.f));
			}
		
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
	case PLAYER_STATE::JUMP:

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

	CScript::SaveToScene(_pFile);

	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_bOnGround, sizeof(bool), 1, _pFile);


}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	CScript::LoadFromScene(_pFile);


	fread(&m_fSpeed, sizeof(float), 1, _pFile);
	fread(&m_bOnGround, sizeof(bool), 1, _pFile);

}

