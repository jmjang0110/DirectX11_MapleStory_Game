#include "pch.h"
#include "CDoubleJumpScript.h"

#include "CScriptMgr.h"


#include <Engine/CAnimator2D.h>
#include <Engine/CLight2D.h>
#include <Engine/CCollider2D.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CEventMgr.h>

#include "CRigidBodyScript.h"
#include "CBasicBallScript.h"

CDoubleJumpScript::CDoubleJumpScript()
	: CScript((int)SCRIPT_TYPE::DOUBLEJUMPSCRIPT)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CDoubleJumpScript::CDoubleJumpScript(const CDoubleJumpScript& _origin)
	: CScript((int)SCRIPT_TYPE::DOUBLEJUMPSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CDoubleJumpScript::~CDoubleJumpScript()
{
}



void CDoubleJumpScript::Enter()
{
}

void CDoubleJumpScript::Exit()
{
}

void CDoubleJumpScript::start()
{
}

void CDoubleJumpScript::update()
{
	if (GetOwner() == nullptr)
		return;
	if (GetOwner()->Animator2D() == nullptr)
		return;

	CAnimator2D* pAnimator2D = GetOwner()->Animator2D();
	CAnimation2D* pAnim = pAnimator2D->GetCurAnim();

	if (pAnim != nullptr)
	{
		if (pAnim->IsFinish() == true)
		{
			tEventInfo info = {};

			info.eType = EVENT_TYPE::DEREGISTER_OBJ_IN_LAYER;
			info.lParam = (DWORD_PTR)GetOwner();
			info.wParam = (DWORD_PTR)GetOwner()->GetLayerIndex();

			CEventMgr::GetInst()->AddEvent(info);

			GetOwner()->Destroy();
			return;
		}
	}


}

void CDoubleJumpScript::lateupdate()
{
}

void CDoubleJumpScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CDoubleJumpScript::OnCollision(CGameObject* _OtherObject)
{
}

void CDoubleJumpScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

