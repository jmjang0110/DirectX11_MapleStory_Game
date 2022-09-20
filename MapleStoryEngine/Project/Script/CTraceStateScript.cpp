#include "pch.h"
#include "CTraceStateScript.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>

#include "CMonsterScript.h"
#include "CAIScript.h"
#include "CIdleStateScript.h"
#include "CBossMonsterScript.h"

#include "CBasicBallScript.h"
#include "CSkillnearScript.h"

CTraceStateScript::CTraceStateScript()
	: CStateScript((int)SCRIPT_TYPE::TRACESTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::TRACE);

}



CTraceStateScript::CTraceStateScript(const CTraceStateScript& _origin)
	: CStateScript((int)SCRIPT_TYPE::TRACESTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::TRACE);

}

CTraceStateScript::~CTraceStateScript()
{
}


void CTraceStateScript::Enter()
{
	/*CMonsterScript* pMonScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");
	if (pMonScript != nullptr)
	{
		MONSTER_DIRECTION eDir = pMonScript->GetDir();
		
	}*/

	m_fRangeToAttack = 300.f;

}

void CTraceStateScript::Exit()
{
}

void CTraceStateScript::start()
{
}

void CTraceStateScript::update()
{
	// Get GameObject "player"
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Player");
	if (pLayer == nullptr)
		return;

	CGameObject* pPlayer = pLayer->FindObj(L"player");
	if (pPlayer == nullptr)
		return;

	// Calculate Diff -  Monster <-> player 
	CMonsterScript* pMonScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");


	if (pMonScript != nullptr)
	{
		Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();
		Vec3 vMonsterPos = GetOwner()->Transform()->GetRelativePos();

		Vec2 vDiff = Vec2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y - vMonsterPos.y);
		float fLen = vDiff.Length();

		tMonsterInfo tMonInfo = pMonScript->GetMonsterInfo();

		// Move 
		vDiff.Normalize();
		Vec2 vMove = vDiff * DT * tMonInfo.fSpeed;
		vMonsterPos.x += vMove.x;
		GetOwner()->Transform()->SetRelativePos(vMonsterPos);


		// Monster <-> player ( out of Range )
		if (fLen > tMonInfo.fRecogRange)
		{
			// Change State 
			GetAI()->ChangeState(MONSTER_STATE::IDLE);
		}

		if (fLen <= m_fRangeToAttack)
		{
			// Change State
			GetAI()->ChangeState(MONSTER_STATE::ATT);
		}


		if (tMonInfo.fHP <= 0.f)
		{
			GetAI()->ChangeState(MONSTER_STATE::DEAD);
		}

	}
	
	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");

	if (pBossScript != nullptr)
	{
		// Calculate Diff -  Boss <-> player 
		Vec3 vBossPos = GetOwner()->Transform()->GetRelativePos();

		Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();
		Vec2 vDiff = Vec2(vPlayerPos.x - vBossPos.x, vPlayerPos.y - vBossPos.y);
		float fLen = vDiff.Length();


		tBossMonInfo tBossInfo = pBossScript->GetMonsterInfo();

		// Move 
		vDiff.Normalize();
		Vec2 vMove2player = vDiff * DT * tBossInfo.fSpeed;
		vBossPos.x += vMove2player.x;
		GetOwner()->Transform()->SetRelativePos(vBossPos);


		// Monster <-> player ( out of Range )
		if (fLen > tBossInfo.fRecogRange)
		{
			// Change State 
			GetAI()->ChangeState(MONSTER_STATE::IDLE);
		}

		if (fLen <= m_fRangeToAttack)
		{
			// Change State
			GetAI()->ChangeState(MONSTER_STATE::ATT);
		}

		if (tBossInfo.fHP <= 0.f)
		{
			GetAI()->ChangeState(MONSTER_STATE::DEAD);
		}

	}

	




}

void CTraceStateScript::lateupdate()
{
}

void CTraceStateScript::OnCollisionEnter(CGameObject* _OtherObject)
{


}

void CTraceStateScript::OnCollision(CGameObject* _OtherObject)
{
}

void CTraceStateScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CTraceStateScript::SaveToScene(FILE* _pFile)
{
}

void CTraceStateScript::LoadFromScene(FILE* _pFile)
{
}

