#include "pch.h"
#include "CIdleStateScript.h"
#include "CStateScript.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

#include "CMonsterScript.h"



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
	// Player 의 위치를 받는다. 
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

	Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();
	Vec3 vMonsterPos = GetOwner()->Transform()->GetRelativePos();

	Vec2 vDiff = Vec2(vPlayerPos.x - vMonsterPos.x, vPlayerPos.y = vMonsterPos.y);
	float fLen = vDiff.Length();


	CMonsterScript* pMonScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");
	if (pMonScript == nullptr)
		return;

	tMonsterInfo tMonInfo = pMonScript->GetMonsterInfo();


	// 플레이어가 몬스터의 인식범위 안으로 진입 
	if (fLen < pMonScript->GetMonsterInfo().fRecogRange)
	{

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

