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

	// Calculate Diff -  Monster <-> player 
	Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();
	Vec3 vMonsterPos = GetOwner()->Transform()->GetRelativePos();

	Vec2 vDiff = Vec2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y - vMonsterPos.y);
	float fLen = vDiff.Length();

	CMonsterScript* pMonScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");
	if (pMonScript == nullptr)
		return;
	tMonsterInfo tMonInfo = pMonScript->GetMonsterInfo();

	// Move 
	Vec2 vMove = vDiff * DT * tMonInfo.fSpeed;
	vMonsterPos.x += vMove.x;
	GetOwner()->Transform()->SetRelativePos(vMonsterPos);


	// Monster <-> player ( out of Range )
	if (fLen > tMonInfo.fRecogRange)
	{
		// Change State 
		GetAI()->ChangeState(MONSTER_STATE::IDLE);
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

