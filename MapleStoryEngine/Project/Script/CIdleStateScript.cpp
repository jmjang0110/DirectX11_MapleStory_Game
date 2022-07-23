#include "pch.h"
#include "CIdleStateScript.h"
#include "CStateScript.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CMonsterScript.h"
#include "CAIScript.h"
#include "CTraceStateScript.h"
#include "CBossMonsterScript.h"



CIdleStateScript::CIdleStateScript()
	: CStateScript((int)SCRIPT_TYPE::IDLESTATESCRIPT)
	, m_fTimer(0.f)


{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::IDLE);

}

CIdleStateScript::CIdleStateScript(const CIdleStateScript& _origin)
	: CStateScript((int)SCRIPT_TYPE::IDLESTATESCRIPT)
	, m_fTimer(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::IDLE);

}

CIdleStateScript::~CIdleStateScript()
{
}


void CIdleStateScript::Enter()
{
}

void CIdleStateScript::Exit()
{
}

void CIdleStateScript::start()
{

}

void CIdleStateScript::update()
{

	m_fTimer += DT;
	
	// Idle State 가 4초 이상 진행되면 Patrol State 로 전환 
	if (m_fTimer >= 4.f)
	{
		// Change State Idle -> Patrol 
		GetAI()->ChangeState(MONSTER_STATE::PATROL);
		m_fTimer = 0.f;


	}

	// Get GameObject "player"
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Player");
	if (pLayer == nullptr)
		return;

	vector<CGameObject*> Objs = pLayer->GetRootObjects();
	CGameObject* pPlayer = nullptr;

	for (int i = 0; i < Objs.size(); ++i)
	{
		if (Objs[i]->GetName() == L"player")
		{
 			pPlayer = Objs[i];
			break;
		}
	}

	if (pPlayer == nullptr)
		return;

	// Calculate Diff - Monster <-> player 
	CMonsterScript* pMonScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");
	if (pMonScript != nullptr)
	{
		Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();			// Player Pos 
		CGameObject* Monster = GetOwner();

		Vec3 vMonsterPos = GetOwner()->Transform()->GetRelativePos();		// Monster Pos 

		Vec2 vDiff = Vec2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y - vMonsterPos.y);
		float fLen = vDiff.Length();

		if (pMonScript == nullptr)
			return;

		tMonsterInfo tMonInfo = pMonScript->GetMonsterInfo();
		// Monster <-> player - Out of Monster's RecogRange 
		if (fLen < pMonScript->GetMonsterInfo().fRecogRange)
		{
			// Change State Idle -> Trace 
			GetAI()->ChangeState(MONSTER_STATE::TRACE);
		}

	}
	

	// Calculate Diff -  Boss <-> player 
	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	if (pBossScript != nullptr)
	{
		
		Vec3 vBossPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();

		Vec2 vDiff = Vec2(vPlayerPos.x - vBossPos.x, vPlayerPos.y - vBossPos.y);
		float fLen = vDiff.Length();

		if (pBossScript == nullptr)
			return;
		tBossMonInfo tBossInfo = pBossScript->GetMonsterInfo();

		// Monster <-> player ( out of Range )
		if (fLen < tBossInfo.fRecogRange)
		{
			// Change State 
			GetAI()->ChangeState(MONSTER_STATE::TRACE);
		}
	}
}

void CIdleStateScript::lateupdate()
{
}

void CIdleStateScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CIdleStateScript::OnCollision(CGameObject* _OtherObject)
{
}

void CIdleStateScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CIdleStateScript::SaveToScene(FILE* _pFile)
{
}

void CIdleStateScript::LoadFromScene(FILE* _pFile)
{
}

