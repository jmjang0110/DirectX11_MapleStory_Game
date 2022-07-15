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



CIdleStateScript::CIdleStateScript()
	: CStateScript((int)SCRIPT_TYPE::IDLESTATESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::IDLE);

}

CIdleStateScript::CIdleStateScript(const CIdleStateScript& _origin)
	: CStateScript((int)SCRIPT_TYPE::IDLESTATESCRIPT)

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
	Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();			// Player Pos 
	CGameObject* Monster = GetOwner();

	Vec3 vMonsterPos = GetOwner()->Transform()->GetRelativePos();		// Monster Pos 

	Vec2 vDiff = Vec2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y - vMonsterPos.y);
	float fLen = vDiff.Length();

	CMonsterScript* pMonScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");
	if (pMonScript == nullptr)
		return;

	tMonsterInfo tMonInfo = pMonScript->GetMonsterInfo();
	// Monster <-> player - Out of Monster's RecogRange 
	if (fLen < pMonScript->GetMonsterInfo().fRecogRange)
	{
		// Change State 
		GetAI()->ChangeState(MONSTER_STATE::TRACE);
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

