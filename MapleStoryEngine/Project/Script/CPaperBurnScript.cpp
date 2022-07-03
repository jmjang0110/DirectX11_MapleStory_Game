#include "pch.h"
#include "CPaperBurnScript.h"

CPaperBurnScript::CPaperBurnScript()
	: CScript((int)SCRIPT_TYPE::PAPERBURNSCRIPT)
	, m_fBurnStrength(0.f)
	, m_bBurn(false)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CPaperBurnScript::CPaperBurnScript(const CPaperBurnScript& _origin)
	: CScript((int)SCRIPT_TYPE::PAPERBURNSCRIPT)
	, m_fBurnStrength(_origin.m_fBurnStrength)
	, m_bBurn(_origin.m_bBurn)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CPaperBurnScript::~CPaperBurnScript()
{


}

void CPaperBurnScript::start()
{
	m_bBurn = false;
	m_fBurnStrength = 0.f;


}

void CPaperBurnScript::update()
{
	if (KEY_TAP(KEY::B))
	{
		m_bBurn = true;
	}

}

void CPaperBurnScript::lateupdate()
{
	Burnning();


}



void CPaperBurnScript::Burnning()
{
	if (!m_bBurn)
		return;

	m_fBurnStrength += DT * (1.f / 3.f);

	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fBurnStrength);
}




// FIle Save 
void CPaperBurnScript::SaveToScene(FILE* _pFile)
{
}

// File Load 
void CPaperBurnScript::LoadFromScene(FILE* _pFile)
{
}



// Collision Check Enter 
void CPaperBurnScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CPaperBurnScript::OnCollision(CGameObject* _OtherObject)
{
}

void CPaperBurnScript::OnCollisionExit(CGameObject* _OtherObject)
{
}
