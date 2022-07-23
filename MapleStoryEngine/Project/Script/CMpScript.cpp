#include "pch.h"
#include "CMpScript.h"
#include <Engine/CAnimator2D.h>
#include <Engine/CLight2D.h>
#include <Engine/CCollider2D.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CAnimation2D.h>

#include "CRigidBodyScript.h"
#include "CBasicBallScript.h"
#include "CDoubleJumpScript.h"
#include "CSkillScript.h"
#include "CSkillnearScript.h"
#include "CInventoryScript.h"
#include "CPlayerScript.h"
CMpScript::CMpScript()
	: CScript((int)SCRIPT_TYPE::MPSCRIPT)
	, m_fMaxMp(10000.f)
	, m_fMp(0.f)
{
	SetName(CScriptMgr::GetScriptName(this));


}

CMpScript::CMpScript(const CMpScript& _origin)
	: CScript((int)SCRIPT_TYPE::MPSCRIPT)
	, m_fMaxMp(10000.f)
	, m_fMp(0.f)
{
	SetName(CScriptMgr::GetScriptName(this));


}

CMpScript::~CMpScript()
{
}

void CMpScript::start()
{
}

void CMpScript::update()
{
	if (m_pPlayer == nullptr)
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Player");
		m_pPlayer = pLayer->FindObj(L"player");
	}
	// Child Of MainBar Object 
	static Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

	CPlayerScript* playerScript = (CPlayerScript*)m_pPlayer->GetScriptByName(L"CPlayerScript");
	m_fMp = playerScript->GetMp();


	// Exp Scale Max Å©±â : Resolution.x - 15.f
	float Ratio = (m_fMp / m_fMaxMp);

	if (m_fMp >= m_fMaxMp)
	{
		m_fMp = m_fMaxMp;
		Ratio = 1.f;
	}

	m_fDest = (171.f) * Ratio;

	vScale.x += DT * 200.f;
	if (vScale.x >= m_fDest)
		vScale.x = m_fDest;


	//static int dir = -1;

	//vScale.x += dir * DT * 10.f;
	//if (vScale.x <= 0.f)
	//	dir = 1;
	//else if (vScale.x >= 171.f)
	//	dir = -1;

	vPos.x = 0.f - 55.f;
	vPos.y = 0.f - 26.f;
	vPos.z = -1.f;

	vPos.x += (vScale.x / 2);

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeScale(vScale);

}

void CMpScript::lateupdate()
{
}

void CMpScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CMpScript::OnCollision(CGameObject* _OtherObject)
{
}

void CMpScript::OnCollisionExit(CGameObject* _OtherObject)
{
}
