#include "pch.h"
#include "CMissileScript.h"

#include "CTimeMgr.h"
#include "CTransform.h"


CMissileScript::CMissileScript()
	: m_fSpeed(0.f)
	, m_fAccTime(0.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::update()
{
	Vec3 vPos = Transform()->GetPos();

	vPos.y += DT * m_fSpeed;

	Transform()->SetPos(vPos);


	m_fAccTime += DT;

	if (m_fAccTime >= 2.f)
	{
		//GetOwner()->Destroy();
	}
}

