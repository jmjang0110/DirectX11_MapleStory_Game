#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
	//: m_pMissilePrefab(nullptr)
	: m_fSpeed(0.5f)
	, m_fBurnStrength(0.f)
	, m_bBurn(false)
{
	SetName(L"CPlayerScript");


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

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * 100.f;

	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * 100.f;

	if (KEY_PRESSED(KEY::UP))	
		vPos.y += DT * 100.f;

	if (KEY_PRESSED(KEY::DOWN))
		vPos.y -= DT * 100.f;	

	Transform()->SetRelativePos(vPos);

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

void CPlayerScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);

	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fBurnStrength, sizeof(float), 1, _pFile);
	fwrite(&m_bBurn, sizeof(bool), 1, _pFile);

}

void CPlayerScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);

	fread(&m_fSpeed, sizeof(float), 1, _pFile);
	fread(&m_fBurnStrength, sizeof(float), 1, _pFile);
	fread(&m_bBurn, sizeof(bool), 1, _pFile);
}

