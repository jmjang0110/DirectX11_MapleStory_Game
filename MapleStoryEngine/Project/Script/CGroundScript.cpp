#include "pch.h"
#include "CGroundScript.h"

#include "CPlayerScript.h"
#include "CRigidBodyScript.h"

#include <Engine/CCollider2D.h>


CGroundScript::CGroundScript()
	: CScript((int)SCRIPT_TYPE::GROUNDSCRIPT)

{
}

CGroundScript::CGroundScript(const CGroundScript& _origin)
	: CScript((int)SCRIPT_TYPE::GROUNDSCRIPT)

{
}

CGroundScript::~CGroundScript()
{
}

void CGroundScript::start()
{

}

void CGroundScript::update()
{
}

void CGroundScript::lateupdate()
{
}



void CGroundScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	// Player <-> Ground  Collide Check
	CPlayerScript* playerScript = (CPlayerScript *)_OtherObject->GetScriptByName(L"CPlayerScript");
	CRigidBodyScript* rigidBodyScript = (CRigidBodyScript*)_OtherObject->GetScriptByName(L"CRigidBodyScript");


	if (nullptr != playerScript)
	{
		
		Vec3 vObjPos = _OtherObject->Transform()->GetRelativePos();
		Vec2 vOvjScale = _OtherObject->Collider2D()->GetOffsetScale();

		Vec3 vGroundPos = Transform()->GetRelativePos();
		Vec2 vGroundScale = Collider2D()->GetOffsetScale();

		// °ãÄ£ ±æÀÌ 
		float fLen = abs(vObjPos.y - vGroundPos.y);
		float fValue = (vOvjScale.y / 2.f + vGroundScale.y / 2.f) - fLen;

		vObjPos.y += fValue;
		_OtherObject->Transform()->SetRelativePos(vObjPos);
		playerScript->SetOnGround(true);

		if (rigidBodyScript != nullptr)
		{
			Vec3 vForce = rigidBodyScript->GetForce();
			vForce.y = 0.f;
			Vec3 vVelocity = rigidBodyScript->GetVeclocity();
			vVelocity.y = 0.f;

			rigidBodyScript->SetForce(vForce);
			rigidBodyScript->SetVelocity(vVelocity);

		}

	}
	


}

void CGroundScript::OnCollision(CGameObject* _OtherObject)
{
	// Player <-> Ground  Collide Check
	CPlayerScript* playerScript = (CPlayerScript*)_OtherObject->GetScriptByName(L"CPlayerScript");

	if (nullptr != playerScript)
	{

		Vec3 vObjPos = _OtherObject->Transform()->GetRelativePos();
		Vec2 vOvjScale = _OtherObject->Collider2D()->GetOffsetScale();

		Vec3 vGroundPos = Transform()->GetRelativePos();
		Vec2 vGroundScale = Collider2D()->GetOffsetScale();

		// °ãÄ£ ±æÀÌ 
		float fLen = abs(vObjPos.y - vGroundPos.y);
		float fValue = (vOvjScale.y / 2.f + vGroundScale.y / 2.f) - fLen;

		vObjPos.y += fValue ;
		_OtherObject->Transform()->SetRelativePos(vObjPos);
		playerScript->SetOnGround(true);


		
	}



}

void CGroundScript::OnCollisionExit(CGameObject* _OtherObject)
{
	// Player <-> Ground  Collide Check
	CPlayerScript* playerScript = (CPlayerScript*)_OtherObject->GetScriptByName(L"CPlayerScript");
	if (nullptr != playerScript)
	{
		playerScript->SetOnGround(false);

	}
}

void CGroundScript::SaveToScene(FILE* _pFile)
{
}

void CGroundScript::LoadFromScene(FILE* _pFile)
{
}

