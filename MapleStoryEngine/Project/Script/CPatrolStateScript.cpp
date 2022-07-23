#include "pch.h"
#include "CPatrolStateScript.h"


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


CPatrolStateScript::CPatrolStateScript()
	: CStateScript((int)SCRIPT_TYPE::PATROLSTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::PATROL);

}

CPatrolStateScript::CPatrolStateScript(const CPatrolStateScript& _origin)
	: CStateScript((int)SCRIPT_TYPE::PATROLSTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::PATROL);

}
CPatrolStateScript::~CPatrolStateScript()
{
}

void CPatrolStateScript::Enter()
{
	m_fTimer = 0.f;
	// Setting PAtrol dist 
	m_fPatrolDist = rand() % 500;
	m_fMove = 0.f;

	// Setting Dir 
	m_iDir = rand() % 2;
	if (m_iDir == 0)
		m_iDir = -1;

	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	if (pBossScript != nullptr)
	{
		if (m_iDir == -1)
			pBossScript->SetDir(BOSS_DIRECTION::LEFT);
		else if (m_iDir == 1)
			pBossScript->SetDir(BOSS_DIRECTION::RIGHT);

	}

}

void CPatrolStateScript::Exit()
{
	m_fTimer = 0.f;

}

void CPatrolStateScript::start()
{
}

void CPatrolStateScript::update()
{
	m_fTimer += DT;

	if (m_fMove >= m_fPatrolDist)
	{
		m_fTimer = 0.f;
		GetAI()->ChangeState(MONSTER_STATE::IDLE);
	}

	// Boss  Patrol
	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	if (pBossScript != nullptr)
	{
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Player");
		CGameObject* pPlayer = pLayer->FindObj(L"player");

		tBossMonInfo tBossInfo = pBossScript->GetMonsterInfo();
		Vec3 vBossPos = GetOwner()->Transform()->GetRelativePos();

		float fMove =  DT * tBossInfo.fSpeed;
		vBossPos.x += fMove * m_iDir;
		m_fMove += fMove; // ´©Àû 

		GetOwner()->Transform()->SetRelativePos(vBossPos);


		// Calculate Diff -  Boss <-> player 
		Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();
		Vec2 vDiff = Vec2(vPlayerPos.x - vBossPos.x, vPlayerPos.y - vBossPos.y);
		float fLen = vDiff.Length();

		if (pBossScript == nullptr)
			return;


		// Monster <-> player ( out of Range )
		if (fLen < tBossInfo.fRecogRange)
		{
			// Change State 
			GetAI()->ChangeState(MONSTER_STATE::TRACE);
		}

	}


}

void CPatrolStateScript::lateupdate()
{
}

void CPatrolStateScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CPatrolStateScript::OnCollision(CGameObject* _OtherObject)
{
}

void CPatrolStateScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CPatrolStateScript::SaveToScene(FILE* _pFile)
{
}

void CPatrolStateScript::LoadFromScene(FILE* _pFile)
{
}


