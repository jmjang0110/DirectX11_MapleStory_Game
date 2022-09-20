#include "pch.h"
#include "CRopeScript.h"

#include <Engine/CCollider2D.h>

#include "CPlayerScript.h"
#include "CSceneSaveLoad.h"
#include "CSceneStartScript.h"

CRopeScript::CRopeScript()
	: CScript((int)SCRIPT_TYPE::ROPESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));


}



CRopeScript::CRopeScript(const CRopeScript& _origin)
	: CScript((int)SCRIPT_TYPE::ROPESCRIPT)


{
	SetName(CScriptMgr::GetScriptName(this));


}

CRopeScript::~CRopeScript()
{
}

void CRopeScript::start()
{
}

void CRopeScript::update()
{
}

void CRopeScript::lateupdate()
{
}

void CRopeScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	

}

void CRopeScript::OnCollision(CGameObject* _OtherObject)
{
	CGameObject* pPlayer = CSceneSaveLoad::pMainPlayer;

	Vec3 vRopeScale = GetOwner()->Collider2D()->GetWorldScale();
	Vec3 vRopePos = GetOwner()->Collider2D()->GetWorldPos();
	Vec3 vPlayerColScale = pPlayer->Collider2D()->GetWorldScale();
	Vec3 vPlayerColPos = pPlayer->Collider2D()->GetWorldPos();
	Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();

	if (_OtherObject == pPlayer)
	{
		if (KEY_TAP(KEY::UP) || KEY_PRESSED(KEY::UP))
		{


	
			if (vPlayerColPos.y - vPlayerColScale.y / 2 > vRopePos.y + vRopeScale.y / 2 - 10.f)
			{
				CPlayerScript* pPlayerScript = (CPlayerScript*)pPlayer->GetScriptByName(L"CPlayerScript");
				pPlayerScript->SetState(PLAYER_STATE::IDLE);
				return;

			}



			CPlayerScript* pPlayerScript = (CPlayerScript*)pPlayer->GetScriptByName(L"CPlayerScript");
			pPlayerScript->SetState(PLAYER_STATE::ROPE_MOVE_UP);
			
			//vPos.y = GetOwner()->Collider2D()->GetWorldPos().y;
			vPlayerPos.x = GetOwner()->Collider2D()->GetWorldPos().x;
			pPlayer->Transform()->SetRelativePos(vPlayerPos);

			pPlayerScript->SetOnGround(false);

		}
		if (KEY_TAP(KEY::DOWN) || KEY_PRESSED(KEY::DOWN))
		{
			CPlayerScript* pPlayerScript = (CPlayerScript*)pPlayer->GetScriptByName(L"CPlayerScript");
			pPlayerScript->SetState(PLAYER_STATE::ROPE_MOVE_DOWN);

			Vec3 vPos = pPlayer->Transform()->GetRelativePos();
			//vPos.y = GetOwner()->Collider2D()->GetWorldPos().y;
			vPos.x = GetOwner()->Collider2D()->GetWorldPos().x;
			pPlayer->Transform()->SetRelativePos(vPos);

			pPlayerScript->SetOnGround(false);

		}

		if (KEY_AWAY(KEY::UP) || KEY_AWAY(KEY::DOWN))
		{

		/*	Vec3 vRopeScale = GetOwner()->Collider2D()->GetWorldScale();
			Vec3 vRopePos = GetOwner()->Collider2D()->GetWorldPos();
			Vec3 vPlayerColScale = pPlayer->Collider2D()->GetWorldScale();
			Vec3 vPlayerColPos = pPlayer->Collider2D()->GetWorldPos();
			Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();*/


			float playerDiff = vPlayerColPos.y - (vPlayerColScale.y / 2.f);
			float ropeDiff = (vRopePos.y + (vRopeScale.y / 2.f)) - 10.f;

			if (playerDiff > ropeDiff)
			{
				CPlayerScript* pPlayerScript = (CPlayerScript*)pPlayer->GetScriptByName(L"CPlayerScript");
				pPlayerScript->SetState(PLAYER_STATE::IDLE);
				return;

			}


			CPlayerScript* pPlayerScript = (CPlayerScript*)pPlayer->GetScriptByName(L"CPlayerScript");
 			pPlayerScript->SetState(PLAYER_STATE::ROPE_STOP);

			Vec3 vPos = pPlayer->Transform()->GetRelativePos();
			//vPos.y = GetOwner()->Collider2D()->GetWorldPos().y;
			vPos.x = GetOwner()->Collider2D()->GetWorldPos().x;
			pPlayer->Transform()->SetRelativePos(vPos);
		}


	}



}

void CRopeScript::OnCollisionExit(CGameObject* _OtherObject)
{
	CGameObject* pPlayer = CSceneSaveLoad::pMainPlayer;
	if (_OtherObject == pPlayer)
	{
		CPlayerScript* pPlayerScript = (CPlayerScript*)pPlayer->GetScriptByName(L"CPlayerScript");
 		//pPlayerScript->SetState(PLAYER_STATE::IDLE);

	}



}

void CRopeScript::SaveToScene(FILE* _pFile)
{
}

void CRopeScript::LoadFromScene(FILE* _pFile)
{
}
