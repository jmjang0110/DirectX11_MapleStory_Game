#include "pch.h"
#include "CTraceStateScript.h"
#include "CMonsterScript.h"

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
}

void CTraceStateScript::Exit()
{
}

void CTraceStateScript::start()
{
}

void CTraceStateScript::update()
{
	// 타게팅 된 Player 를 쫓아간다. 
	

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

	Vec2 vMove = vDiff* DT * 10.f;
	Vec3 move = Vec3(vMove.x, vMove.y, vMonsterPos.z);
	GetOwner()->Transform()->SetRelativePos(move);

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

