#include "pch.h"
#include "CCloudScript.h"


#include <Engine/CTimeMgr.h>
#include <Engine/CTileMap.h>
#include <Engine/CTransform.h>

#include <Engine/CEventMgr.h>


#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CPrefab.h>
#include <Engine/CCollider2D.h>


#include "CSceneSaveLoad.h"
#include "CButtonScript.h"
#include "CItemScript.h"
#include "CNumberScript.h"



CCloudScript::CCloudScript()
	: CScript((int)SCRIPT_TYPE::CLOUDSCRIPT)
	, m_fSpeed(100.f)
	, m_vStartPos(Vec3(1000.f, 0.f, 0.f))
	, m_vEndPos(Vec3(0.f, 0.f, 0.f))
	, m_dir(-1)

{
	SetName(CScriptMgr::GetScriptName(this));


}

CCloudScript::CCloudScript(const CCloudScript& _origin)
	: CScript((int)SCRIPT_TYPE::CLOUDSCRIPT)


{
	SetName(CScriptMgr::GetScriptName(this));


}

CCloudScript::~CCloudScript()
{
}

void CCloudScript::start()
{
	CLayer* pLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Default2");
	CGameObject* pObj = pLayer->FindObj(L"MapSize");

	Vec3 vScale = pObj->Collider2D()->GetWorldScale();
	Vec3 vCenter = pObj->Collider2D()->GetWorldPos();
	Vec3 CloudScale = GetOwner()->Transform()->GetWorldScale();

	float fSpeed = rand() % 60;
	m_fSpeed = 100.f + fSpeed;


	m_vStartPos.x = vCenter.x + (vScale.x / 2) + CloudScale.x;
	m_vStartPos.y = rand() % 1000;

	m_vEndPos = m_vStartPos;
	m_vEndPos.x = vCenter.x - (vScale.x / 2) - CloudScale.x;

}

void CCloudScript::update()
{

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	vPos.x += DT * m_fSpeed * m_dir;

	// -> Right Move 
	if (m_dir >= 1)
	{
		if (vPos.x >= m_vEndPos.x)
		{
			vPos.x = m_vStartPos.x;
		}
	}
	// <- Left Move 
	else if (m_dir <= -1)
	{
		if (vPos.x <= m_vEndPos.x)
		{
			vPos.x = m_vStartPos.x;

		}
	}

	GetOwner()->Transform()->SetRelativePos(vPos);

}

void CCloudScript::lateupdate()
{
}

void CCloudScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CCloudScript::OnCollision(CGameObject* _OtherObject)
{
}

void CCloudScript::OnCollisionExit(CGameObject* _OtherObject)
{
}
