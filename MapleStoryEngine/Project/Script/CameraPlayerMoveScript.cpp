#include "pch.h"
#include "CameraPlayerMoveScript.h"

#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/Clayer.h>

CameraPlayerMoveScript::CameraPlayerMoveScript()
	: CScript((int)SCRIPT_TYPE::AMERAPLAYERMOVESCRIPT)
	, m_fSpeed(2.f)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CameraPlayerMoveScript::CameraPlayerMoveScript(const CameraPlayerMoveScript& _origin)
	: CScript((int)SCRIPT_TYPE::AMERAPLAYERMOVESCRIPT)
	, m_fSpeed(_origin.m_fSpeed)

{
	SetName(CScriptMgr::GetScriptName(this));
}



CameraPlayerMoveScript::~CameraPlayerMoveScript()
{
	

}


void CameraPlayerMoveScript::start()
{
}

void CameraPlayerMoveScript::update()
{
	// Get Layer which has "player" Obj
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Player");

	if (nullptr == pLayer)
		return;

	// FInd "player" Obj
	vector<CGameObject*> vecRootObjs = pLayer->GetRootObjects();
	CGameObject* pPlayerObj = nullptr;
	for (int i = 0; i < vecRootObjs.size(); ++i)
	{
		if (L"player" == vecRootObjs[i]->GetName())
		{
			pPlayerObj = vecRootObjs[i];

		}
	}

	if (nullptr == pPlayerObj)
		return;

	Vec3 vPlayerPos = pPlayerObj->Transform()->GetRelativePos();

	// Camera Move Destination
	Vec3 vDestPos = vPlayerPos;
	
	CCamera* pCam = GetOwner()->Camera();
	
	if (nullptr == pCam)
		return;

	Vec3 vPos = Transform()->GetRelativePos();
	
	// Move 
	Vec3 vMoveDist = vDestPos - vPos;
	if (abs(vMoveDist.x) <= 30.f && abs(vMoveDist.y) <= 30.f)
	{
		vMoveDist.x = 0.f;
		vMoveDist.y = 0.f;
		vPos.x += vMoveDist.x * DT * m_fSpeed;
		vPos.y += vMoveDist.y * DT * m_fSpeed;

	}
	else
	{
		vPos.x += vMoveDist.x * DT * m_fSpeed;
		vPos.y += vMoveDist.y * DT * m_fSpeed;

	}

	
	
	Transform()->SetRelativePos(vPos);
}

void CameraPlayerMoveScript::lateupdate()
{
}

void CameraPlayerMoveScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CameraPlayerMoveScript::OnCollision(CGameObject* _OtherObject)
{
}

void CameraPlayerMoveScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

void CameraPlayerMoveScript::SaveToScene(FILE* _pFile)
{
}

void CameraPlayerMoveScript::LoadFromScene(FILE* _pFile)
{
}

