#include "pch.h"
#include "CHpScript.h"

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



#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"
CHpScript::CHpScript()
	: CScript((int)SCRIPT_TYPE::HPSCRIPT)
	, m_fMaxHp(70000.f)
	, m_fHp(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CHpScript::CHpScript(const CHpScript& _origin)
	: CScript((int)SCRIPT_TYPE::HPSCRIPT)
	, m_fMaxHp(70000.f)
	, m_fHp(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CHpScript::~CHpScript()
{


}

void CHpScript::start()
{
	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//CLayer* pLayer = pCurScene->GetLayer(L"Player");

	m_pPlayer = CSceneSaveLoad::pMainPlayer;// pLayer->FindObj(L"player");
	CPlayerScript* playerScript = (CPlayerScript*)CSceneSaveLoad::pMainPlayer->GetScriptByName(L"CPlayerScript");

	m_fMaxHp = playerScript->GetMaxHp();
	m_fHp = playerScript->GetHp();




}

void CHpScript::update()
{
	if (m_pPlayer == nullptr)
	{
		//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		//CLayer* pLayer = pCurScene->GetLayer(L"Player");
		m_pPlayer = CSceneSaveLoad::pMainPlayer;//pLayer->FindObj(L"player");
	}

	// Child Of MainBar Object 
	static Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();


	CPlayerScript* playerScript = (CPlayerScript*)m_pPlayer->GetScriptByName(L"CPlayerScript");
	m_fHp = playerScript->GetHp();
	m_fMaxHp = playerScript->GetMaxHp();

	// Exp Scale Max Å©±â : Resolution.x - 15.f
	float Ratio = (m_fHp / m_fMaxHp);

	if (m_fHp >= m_fMaxHp)
	{
		m_fHp = m_fMaxHp;
		Ratio = 1.f;
	}

	if (m_fHp <= 0.f)
	{
		m_fHp = 0.f;
		playerScript->SetHP(0.f);
	}
	m_fDest = (171.f) * Ratio;

	vScale.x += DT * 200.f;
	if (vScale.x >= m_fDest)
		vScale.x = m_fDest;


	vPos.x = 0.f - 55.f;
	vPos.y = 0.f - 10.f;
	vPos.z = -1.f;

	vPos.x += (vScale.x / 2);

	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeScale(vScale);

}

void CHpScript::lateupdate()
{


}

void CHpScript::OnCollisionEnter(CGameObject* _OtherObject)
{


}

void CHpScript::OnCollision(CGameObject* _OtherObject)
{


}

void CHpScript::OnCollisionExit(CGameObject* _OtherObject)
{


}
