#include "pch.h"
#include "CGroundScript.h"

#include "CPlayerScript.h"
#include "CRigidBodyScript.h"
#include "CGravityScript.h"

#include <Engine/CCollider2D.h>


CGroundScript::CGroundScript()
	: CScript((int)SCRIPT_TYPE::GROUNDSCRIPT)
	, m_fDiff(0.f)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CGroundScript::CGroundScript(const CGroundScript& _origin)
	: CScript((int)SCRIPT_TYPE::GROUNDSCRIPT)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CGroundScript::~CGroundScript()
{

}



void CGroundScript::start()
{
	Collider2D()->Reset();


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
	CPlayerScript*		playerScript = (CPlayerScript *)_OtherObject->GetScriptByName(L"CPlayerScript");
	CGravityScript*		gravityScript = (CGravityScript*)_OtherObject->GetScriptByName(L"CGravityScript");
	CRigidBodyScript*	rigidBodyScript = (CRigidBodyScript*)_OtherObject->GetScriptByName(L"CRigidBodyScript");


	Vec3 vObjPos = _OtherObject->Collider2D()->GetWorldPos();
	Vec2 vObjScale = _OtherObject->Collider2D()->GetOffsetScale();

	Vec3 vGroundPos = Collider2D()->GetWorldPos();
	Vec2 vGroundScale = Collider2D()->GetOffsetScale();

	float fAngle = Collider2D()->GetOffsetRotation();

	if (fAngle == 0.f)
	{

		// 겹친 길이 
		float fLen = abs(vObjPos.y - vGroundPos.y);
		float fValue = (vObjScale.y / 2.f + vGroundScale.y / 2.f) - fLen;

		float vObj_SouthLine_y = vObjPos.y - (vObjScale.y / 2);

		float vGround_SountLine_y = vGroundPos.y - (vGroundScale.y / 2);
		float vGround_NorthLine_y = vGroundPos.y + (vGroundScale.y / 2);

		if (vGround_SountLine_y < vObj_SouthLine_y && vObj_SouthLine_y <= vGround_NorthLine_y)
		{
			// 올린다. 
			Vec3 playerPrevPos = Vec3(0.f, 0.f, 0.f);
			Vec3 playerCurPos = Vec3(0.f, 0.f, 0.f);
			if (nullptr != playerScript)
			{
				playerPrevPos = playerScript->GetPrevPos();
				playerCurPos = _OtherObject->Transform()->GetRelativePos();

			}

			// 위 -> 아래로 향할 때 
			if (playerPrevPos.y - playerCurPos.y >= 0.f)
			{
				vObjPos = _OtherObject->Transform()->GetRelativePos();
				vObjPos.y += fValue;
				_OtherObject->Transform()->SetRelativePos(vObjPos);

				if (nullptr != playerScript)
				{
					playerScript->SetOnGround(true);
				}

				if (nullptr != gravityScript)
				{
					gravityScript->SetOnGround(true);
				}

			}

		}

	}
	else
	{
		float vGround_SountLine_y = vGroundPos.y - (vGroundScale.y / 2);
		float vGround_NorthLine_y = vGroundPos.y + (vGroundScale.y / 2);
		float vObj_SouthLine_y = vObjPos.y - (vObjScale.y / 2);

		float fRadian = (fAngle * 3.141592f) / 180.f;

		float Ground_x = (vGroundScale.x / 2.f);
		float Ground_y = 0.f;

		float RotX = Ground_x * cos(fRadian) - Ground_y * sin(fRadian);
		float RotY = Ground_x * sin(fRadian) + Ground_y * cos(fRadian);

		Line	ObjLine;
		Line	GroundLine;
		ObjLine.Init(Vec2(vObjPos.x - (vObjScale.x / 2.f), vObj_SouthLine_y)
			, Vec2(vObjPos.x + (vObjScale.x / 2.f), vObj_SouthLine_y));

		GroundLine.Init(Vec2(RotX * -1 + vGroundPos.x, RotY * -1 + vGroundPos.y)
			, Vec2(RotX + vGroundPos.x, RotY + vGroundPos.y));

		// normal vector
		Vec3 P = vGroundPos;
		Vec3 Q = Vec3(GroundLine.p1.x, GroundLine.p1.y, 1.f) - P;
		Vec3 R = P - Vec3(GroundLine.p2.x, GroundLine.p2.y, -1.f);
		Vec3 PQ = Q - P;
		Vec3 PR = R - P;
		Vec3 Normal = PR.Cross(PQ);
		Normal.Normalize();
		float fLen = vGroundScale.y / 2.f;
		GroundLine.p1.x += fLen * Normal.x;
		GroundLine.p1.y += fLen * Normal.y;
		GroundLine.p2.x += fLen * Normal.x;
		GroundLine.p2.y += fLen * Normal.y;


		if (LineInterSection(ObjLine, GroundLine))
		{
			// 올린다. 
			Vec3 playerPrevPos = Vec3(0.f, 0.f, 0.f);
			Vec3 playerCurPos = Vec3(0.f, 0.f, 0.f);
			if (nullptr != playerScript)
			{
				playerPrevPos = playerScript->GetPrevPos();
				playerCurPos = _OtherObject->Transform()->GetRelativePos();

			}

			// 위 -> 아래로 향할 때 
			if (playerPrevPos.y - playerCurPos.y >= 0.f)
			{
				vObjPos = _OtherObject->Transform()->GetRelativePos();
				Vec3 ColPos = _OtherObject->Collider2D()->GetWorldPos();

				Vec2 ColOffsetPos = _OtherObject->Collider2D()->GetOffsetPos();
				// 직선의 방정식 - Ground 
				float m = (GroundLine.p2.y - GroundLine.p1.y) / (GroundLine.p2.x - GroundLine.p1.x);
				float y;

				// Player Move 
				if (0.f < fAngle && fAngle <= 90.f)
				{
					vObjPos.y = m * (vObjPos.x + (vObjScale.x / 2.f) - GroundLine.p1.x) + GroundLine.p1.y;
					y = m * (vObjPos.x - GroundLine.p1.x) + GroundLine.p1.y;
				}
				else if (-90.f <= fAngle && fAngle < 0.f)
				{
					vObjPos.y = m * (vObjPos.x - (vObjScale.x / 2.f) - GroundLine.p1.x) + GroundLine.p1.y;
					y = m * (vObjPos.x - GroundLine.p1.x) + GroundLine.p1.y;

				}

				float fDiff = vObjPos.y - y;
				vObjPos.y += fDiff;

				m_fDiff = fDiff;

				_OtherObject->Transform()->SetRelativePos(vObjPos);

				if (nullptr != playerScript)
				{
					playerScript->SetOnGround(true);
				}

				if (nullptr != gravityScript)
				{
					gravityScript->SetOnGround(true);
				}

			}
		}
	}






	/*if (rigidBodyScript != nullptr)
	{
		Vec3 vForce = rigidBodyScript->GetForce();
		vForce.y = 0.f;
		Vec3 vVelocity = rigidBodyScript->GetVelocity();
		vVelocity.y = 0.f;

		rigidBodyScript->SetForce(vForce);
		rigidBodyScript->SetVelocity(vVelocity);

	}*/

}

void CGroundScript::OnCollision(CGameObject* _OtherObject)
{
	// Player <-> Ground  Collide Check
	CPlayerScript* playerScript = (CPlayerScript*)_OtherObject->GetScriptByName(L"CPlayerScript");
	CGravityScript* gravityScript = (CGravityScript*)_OtherObject->GetScriptByName(L"CGravityScript");


	Vec3 vObjPos = _OtherObject->Collider2D()->GetWorldPos();
	Vec2 vObjScale = _OtherObject->Collider2D()->GetOffsetScale();

	Vec3 vGroundPos = Collider2D()->GetWorldPos();
	Vec2 vGroundScale = Collider2D()->GetOffsetScale();
	


	float fAngle = Collider2D()->GetOffsetRotation();

	if (fAngle == 0.f)
	{

		// 겹친 길이 
		float fLen = abs(vObjPos.y - vGroundPos.y);
		float fValue = (vObjScale.y / 2.f + vGroundScale.y / 2.f) - fLen;

		float vObj_SouthLine_y = vObjPos.y - (vObjScale.y / 2);

		float vGround_SountLine_y = vGroundPos.y - (vGroundScale.y / 2);
		float vGround_NorthLine_y = vGroundPos.y + (vGroundScale.y / 2);

		if (vGround_SountLine_y < vObj_SouthLine_y && vObj_SouthLine_y <= vGround_NorthLine_y)
		{
			// 올린다. 
			Vec3 playerPrevPos = Vec3(0.f, 0.f, 0.f);
			Vec3 playerCurPos = Vec3(0.f, 0.f, 0.f);
			if (nullptr != playerScript)
			{
				playerPrevPos = playerScript->GetPrevPos();
				playerCurPos = _OtherObject->Transform()->GetRelativePos();

			}

			// 위 -> 아래로 향할 때 
			if (playerPrevPos.y - playerCurPos.y >= 0.f)
			{
				vObjPos = _OtherObject->Transform()->GetRelativePos();
				vObjPos.y += fValue;
				_OtherObject->Transform()->SetRelativePos(vObjPos);

				if (nullptr != playerScript)
				{
					playerScript->SetOnGround(true);
				}

				if (nullptr != gravityScript)
				{
					gravityScript->SetOnGround(true);
				}

			}

		}

	}
	else
	{
		float vGround_SountLine_y = vGroundPos.y - (vGroundScale.y / 2);
		float vGround_NorthLine_y = vGroundPos.y + (vGroundScale.y / 2);
		float vObj_SouthLine_y = vObjPos.y - (vObjScale.y / 2);

		float fRadian = (fAngle * 3.141592f) / 180.f;

		float Ground_x = (vGroundScale.x / 2.f);
		float Ground_y = 0.f;

		float RotX = Ground_x * cos(fRadian) - Ground_y * sin(fRadian);
		float RotY = Ground_x * sin(fRadian) + Ground_y * cos(fRadian);

		Line	ObjLine;
		Line	GroundLine;
		ObjLine.Init(Vec2(vObjPos.x - (vObjScale.x / 2.f), vObj_SouthLine_y)
			, Vec2(vObjPos.x + (vObjScale.x / 2.f), vObj_SouthLine_y));

		GroundLine.Init(Vec2(RotX * -1 + vGroundPos.x, RotY * -1 + vGroundPos.y)
			, Vec2(RotX + vGroundPos.x, RotY + vGroundPos.y));

		// normal vector
		Vec3 P = vGroundPos;
		Vec3 Q = Vec3(GroundLine.p1.x, GroundLine.p1.y, 1.f) - P;
		Vec3 R = P - Vec3(GroundLine.p2.x, GroundLine.p2.y, -1.f);
		Vec3 PQ = Q - P;
		Vec3 PR = R - P;
		Vec3 Normal = PR.Cross(PQ);
		Normal.Normalize();
		float fLen = vGroundScale.y / 2.f;
		GroundLine.p1.x += fLen * Normal.x;
		GroundLine.p1.y += fLen * Normal.y;
		GroundLine.p2.x += fLen * Normal.x;
		GroundLine.p2.y += fLen * Normal.y;


		if (LineInterSection(ObjLine, GroundLine))
		{
			// 올린다. 
			Vec3 playerPrevPos = Vec3(0.f, 0.f, 0.f);
			Vec3 playerCurPos = Vec3(0.f, 0.f, 0.f);
			if (nullptr != playerScript)
			{
				playerPrevPos = playerScript->GetPrevPos();
				playerCurPos = _OtherObject->Transform()->GetRelativePos();

			}

			// 위 -> 아래로 향할 때 
			if (playerPrevPos.y - playerCurPos.y >= 0.f)
			{
 				vObjPos = _OtherObject->Transform()->GetRelativePos();
				Vec2 ColOffsetPos = _OtherObject->Collider2D()->GetOffsetPos();
				// 직선의 방정식 - Ground 
				float m = (GroundLine.p2.y - GroundLine.p1.y) / (GroundLine.p2.x - GroundLine.p1.x);
				float y;

				// Player Move 
				if (0.f < fAngle && fAngle <= 90.f)
				{
					vObjPos.y = m * (vObjPos.x   + (vObjScale.x / 2.f) - GroundLine.p1.x) + GroundLine.p1.y;
					y = m * (vObjPos.x  - GroundLine.p1.x) + GroundLine.p1.y;
				}
				else if (-90.f <= fAngle && fAngle < 0.f)
				{
					vObjPos.y = m * (vObjPos.x  - (vObjScale.x / 2.f) - GroundLine.p1.x) + GroundLine.p1.y;
					y = m * (vObjPos.x  - GroundLine.p1.x) + GroundLine.p1.y;

				}

				//float fDiff = vObjPos.y - y;
				vObjPos.y += m_fDiff;


				_OtherObject->Transform()->SetRelativePos(vObjPos);

				if (nullptr != playerScript)
				{
					playerScript->SetOnGround(true);
				}

				if (nullptr != gravityScript)
				{
					gravityScript->SetOnGround(true);
				}

			}
		}
	}

	

}

void CGroundScript::OnCollisionExit(CGameObject* _OtherObject)
{
	// Player <-> Ground  Collide Check
	CPlayerScript* playerScript = (CPlayerScript*)_OtherObject->GetScriptByName(L"CPlayerScript");
	CGravityScript* gravityScript = (CGravityScript*)_OtherObject->GetScriptByName(L"CGravityScript");

	if (nullptr != playerScript)
	{
		playerScript->SetOnGround(false);
	}

	if (nullptr != gravityScript)
	{
		gravityScript->SetOnGround(false);
	}
}

void CGroundScript::SaveToScene(FILE* _pFile)
{
}

void CGroundScript::LoadFromScene(FILE* _pFile)
{
}


// 두 선분이 교차하면 1을 교차하지 않으면 0을 반환합니다.
bool CGroundScript::LineInterSection(Line l1, Line l2)
{
	int ret;
	// l1을 기준으로 l2가 교차하는 지 확인한다.
	int l1_l2 = CCW(l1.p1, l1.p2, l2.p1) * CCW(l1.p1, l1.p2, l2.p2);
	// l2를 기준으로 l1이 교차하는 지 확인한다.
	int l2_l1 = CCW(l2.p1, l2.p2, l1.p1) * CCW(l2.p1, l2.p2, l1.p2);


	// l1 과 l2가 일직선 상에 있는 경우
	if (l1_l2 == 0 && l2_l1 == 0) {
		// Line1의 점의 크기 순서를 p1 < p2 순서로 맞춘다.
		if (Comparator(l1.p2, l1.p1)) Swap(&l1.p1, &l1.p2);
		// Line2의 점의 크기 순서를 p1 < p2 순서로 맞춘다.
		if (Comparator(l2.p2, l2.p1)) Swap(&l2.p1, &l2.p2);

		// l1.p1 -----------l1.p2
		//         l2.p1 -----------l2.p2
		// 위 조건을 만족하는 지 살펴본다.
		// See if the above conditions are met.
		ret = (Comparator(l2.p1, l1.p2)) && (Comparator(l1.p1, l2.p2));
	}
	// l1과 l2가 일직선 상에 있지 않는 경우
	else {
		ret = (l1_l2 <= 0) && (l2_l1 <= 0);
	}
	return ret;
}

int CGroundScript::CCW(Vec2 p1, Vec2 p2, Vec2 p3)
{
	int cross_product = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

	if (cross_product > 0) {
		return 1;
	}
	else if (cross_product < 0) {
		return -1;
	}
	else {
		return 0;
	}
}

int CGroundScript::Comparator(Vec2 Left, Vec2 Right)
{
	int ret;
	if (Left.x == Right.x) {
		ret = (Left.y <= Right.y);
	}
	else {
		ret = (Left.x <= Right.x);
	}
	return ret;
}

void CGroundScript::Swap(Vec2* p1, Vec2* p2)
{
	Vec2 temp;
	temp = *p1;
	*p1 = *p2;
	*p2 = temp;

}