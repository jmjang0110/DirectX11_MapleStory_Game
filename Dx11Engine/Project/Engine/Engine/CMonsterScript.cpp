#include "pch.h"
#include "CMonsterScript.h"

#include "CTimeMgr.h"
#include "CTransform.h"

#include"CAnimation2D.h"
#include "CAnimator2D.h"

CMonsterScript::CMonsterScript()
	: m_fSpeed(50.f)
	, m_fAccTime(0.f)
	, m_CurActType(ACT_TYPE::MOVE)
	, m_MoveDir(1)
	, m_fMoveDist(100.f)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::update()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Transform()->SetRelativePos(vPos);
	m_fAccTime += DT;

	if (GetAsyncKeyState('0') & 0x8000)
	{
		SetCurActType(ACT_TYPE(0));
	}
	if (GetAsyncKeyState('1') & 0x8000)
	{
		SetCurActType(ACT_TYPE(1));
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		SetCurActType(ACT_TYPE(2));
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		SetCurActType(ACT_TYPE(3));
	}

	if (GetAsyncKeyState('4') & 0x8000)
	{
		SetCurActType(ACT_TYPE(4));
	}

	if (GetAsyncKeyState('5') & 0x8000)
	{
		SetCurActType(ACT_TYPE(5));
	}

	if (GetAsyncKeyState('6') & 0x8000)
	{
		SetCurActType(ACT_TYPE(6));
	}

	if (GetAsyncKeyState('7') & 0x8000)
	{
		SetCurActType(ACT_TYPE(7));
	}

	//switch (m_CurActType)
	//{
	//case ACT_TYPE::DIE:

	//	

	//	break;
	//case ACT_TYPE::HIT:
	//	break;
	//case ACT_TYPE::MOVE:
	//	
	//	vPos.x += DT * m_fSpeed * m_MoveDir;
	//	m_fMoveDist += DT * m_fSpeed * m_MoveDir;

	//	if (m_fMoveDist < 0.f)
	//	{
	//		m_fMoveDist = 0.f;
	//		m_MoveDir *= -1;
	//	}
	//		
	//	else if (m_fMoveDist > 100.f)
	//	{
	//		m_fMoveDist = 100.f;
	//		m_MoveDir *= -1;
	//	}

	//	break;
	//case ACT_TYPE::STAND:

	//	break;
	//case ACT_TYPE::ATTACK1:
	//	break;
	//case ACT_TYPE::ATTACK2:
	//	break;
	//case ACT_TYPE::SKILL1:
	//	break;
	//case ACT_TYPE::SKILL2:
	//	break;
	//default:
	//	break;
	//}

	//Transform()->SetRelativePos(vPos);
	//m_fAccTime += DT;

	//// Test
	//if (m_fAccTime >= 3.f)
	//{
	//	//int _act = rand() % (UINT)ACT_TYPE::END;
	//	
	//	//SetCurActType((ACT_TYPE)_act);
	//	//m_fAccTime = 0.f;

	//}
	
}

void CMonsterScript::OnCollisionEnter(CGameObject* _pOtherObj)
{
	if (_pOtherObj->GetName() == L"ChildObject")
	{
		_pOtherObj->Destroy();

	}
}

void CMonsterScript::SetCurActType(ACT_TYPE _actType)
{
	CAnimator2D* Animator = GetOwner()->Animator2D();

	if (nullptr != Animator)
	{
		CAnimation2D* anim = Animator->FindAnim(acString[(UINT)_actType]);
		if (anim != nullptr)
			Animator->Play(acString[(UINT)_actType], true);

	}

}


