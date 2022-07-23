#include "pch.h"
#include "CWallScript.h"



#include "CPlayerScript.h"
#include "CRigidBodyScript.h"
#include "CGravityScript.h"
#include "CMonsterScript.h"
#include "CBossMonsterScript.h"

#include <Engine/CCollider2D.h>

CWallScript::CWallScript()
	: CScript((int)SCRIPT_TYPE::WALLSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CWallScript::CWallScript(const CWallScript& _origin)
	: CScript((int)SCRIPT_TYPE::WALLSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));

}

CWallScript::~CWallScript()
{

}



void CWallScript::start()
{
}

void CWallScript::update()
{
}

void CWallScript::lateupdate()
{
}

void CWallScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	CMonsterScript* monsterScript = (CMonsterScript*)_OtherObject->GetScriptByName(L"CMonsterScript");
	CBossMonsterScript* BossMobScript = (CBossMonsterScript*)_OtherObject->GetScriptByName(L"CBossMonsterScript");

	// Collider 기준
	Vec3 vOtherObjColSize = _OtherObject->Collider2D()->GetWorldScale();
	Vec3 vOtherObjColPos = _OtherObject->Collider2D()->GetWorldPos();

	Vec3 vWallSize = GetOwner()->Collider2D()->GetWorldScale();
	Vec3 vWallPos = GetOwner()->Collider2D()->GetWorldPos();

	float fDiff = vOtherObjColPos.x - vWallPos.x;
	float fDiff2 = (vOtherObjColSize.x / 2) + (vWallSize.x / 2);
	if (fDiff < 0)
		fDiff2 *= -1;


	Vec3 vPos = _OtherObject->Transform()->GetRelativePos();
	vPos.x = vWallPos.x + fDiff2;
	_OtherObject->Transform()->SetRelativePos(vPos);

	if (BossMobScript != nullptr)
		BossMobScript->SetPrevPos(vPos);



}

void CWallScript::OnCollision(CGameObject* _OtherObject)
{
	CMonsterScript* monsterScript = (CMonsterScript*)_OtherObject->GetScriptByName(L"CMonsterScript");
	CBossMonsterScript* BossMobScript = (CBossMonsterScript*)_OtherObject->GetScriptByName(L"CBossMonsterScript");

	// Collider 기준
	Vec3 vOtherObjColSize = _OtherObject->Collider2D()->GetWorldScale();
	Vec3 vOtherObjColPos = _OtherObject->Collider2D()->GetWorldPos();

	Vec3 vWallSize = GetOwner()->Collider2D()->GetWorldScale();
	Vec3 vWallPos = GetOwner()->Collider2D()->GetWorldPos();

	float fDiff = vOtherObjColPos.x - vWallPos.x;
	float fDiff2 = (vOtherObjColSize.x / 2) + (vWallSize.x / 2);
	if (fDiff < 0)
		fDiff2 *= -1;


	Vec3 vPos = _OtherObject->Transform()->GetRelativePos();
	vPos.x = vWallPos.x + fDiff2;
	_OtherObject->Transform()->SetRelativePos(vPos);


	if (BossMobScript != nullptr)
		BossMobScript->SetPrevPos(vPos);

}

void CWallScript::OnCollisionExit(CGameObject* _OtherObject)
{

}

void CWallScript::SaveToScene(FILE* _pFile)
{
}

void CWallScript::LoadFromScene(FILE* _pFile)
{
}

