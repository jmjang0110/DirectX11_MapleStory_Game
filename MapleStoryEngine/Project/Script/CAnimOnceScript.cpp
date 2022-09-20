#include "pch.h"
#include "CAnimOnceScript.h"

#include <Engine/CAnimation2D.h>
#include <Engine/CAnimator2D.h>


CAnimOnceScript::CAnimOnceScript()
	: CScript((int)SCRIPT_TYPE::ANIMONCESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CAnimOnceScript::CAnimOnceScript(const CAnimOnceScript& _origin) 
	: CScript((int)SCRIPT_TYPE::ANIMONCESCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CAnimOnceScript::~CAnimOnceScript()
{
}



void CAnimOnceScript::start()
{
}

void CAnimOnceScript::update()
{
	if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish() == true)
	{
		// Event 
		CGameObject* pDelObj = GetOwner();
		int pDelObj_LayerIdx = GetOwner()->GetLayerIndex();

		if (GetOwner()->GetParent() != nullptr)
		{
			CSceneMgr::GetInst()->DisconnectParent(pDelObj);
		}

		CSceneMgr::GetInst()->DeRegisterObjInLayer(pDelObj, pDelObj_LayerIdx);
		GetOwner()->Destroy();

	}
}

void CAnimOnceScript::lateupdate()
{
}



void CAnimOnceScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CAnimOnceScript::OnCollision(CGameObject* _OtherObject)
{
}

void CAnimOnceScript::OnCollisionExit(CGameObject* _OtherObject)
{
}




void CAnimOnceScript::SaveToScene(FILE* _pFile)
{
}

void CAnimOnceScript::LoadFromScene(FILE* _pFile)
{
}

