#include "pch.h"
#include "CHitScript.h"
#include "CAIScript.h"

#include "CBossMonsterScript.h"
#include "CMonsterScript.h"


CHitScript::CHitScript()
	: CStateScript((int)SCRIPT_TYPE::HITSCRIPT)


{

	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::HIT);

}

CHitScript::CHitScript(const CHitScript& _origin)
	: CStateScript((int)SCRIPT_TYPE::HITSCRIPT)


{
	SetName(CScriptMgr::GetScriptName(this));
	SetMonsterState(MONSTER_STATE::HIT);

}

CHitScript::~CHitScript()
{
}




void CHitScript::Enter()
{
	m_fTimer = 0.f;

}

void CHitScript::Exit()
{
}

void CHitScript::start()
{
}



void CHitScript::update()
{
	m_fTimer += DT;

	CBossMonsterScript* pBossScript = (CBossMonsterScript*)GetOwner()->GetScriptByName(L"CBossMonsterScript");
	CMonsterScript* pMonScript = (CMonsterScript*)GetOwner()->GetScriptByName(L"CMonsterScript");

	if (pMonScript != nullptr)
	{
		tMonsterInfo tMonInfo = pMonScript->GetMonsterInfo();

		if (m_fTimer >= 0.4f)
		{
			GetAI()->ChangeState(MONSTER_STATE::IDLE);

		}

		if (tMonInfo.fHP <= 0.f)
		{
			GetAI()->ChangeState(MONSTER_STATE::DEAD);
		}
	}

	if (pBossScript != nullptr)
	{
		tBossMonInfo tBossInfo = pBossScript->GetMonsterInfo();

		if (m_fTimer >= 0.4f)
		{
			GetAI()->ChangeState(MONSTER_STATE::IDLE);

		}

		if (tBossInfo.fHP <= 0.f)
		{
			Ptr<CSound> pBgm = CResMgr::GetInst()->Load<CSound>(L"sound\\Papulatus\\Die.mp3", L"sound\\Papulatus\\Die.mp3");
			pBgm->Play(1, 1.f, true);

			GetAI()->ChangeState(MONSTER_STATE::DEAD);
		}


	}


}

void CHitScript::lateupdate()
{

}



void CHitScript::OnCollisionEnter(CGameObject* _OtherObject)
{

}

void CHitScript::OnCollision(CGameObject* _OtherObject)
{

}

void CHitScript::OnCollisionExit(CGameObject* _OtherObject)
{

}

void CHitScript::SaveToScene(FILE* _pFile)
{

}

void CHitScript::LoadFromScene(FILE* _pFile)
{

}

