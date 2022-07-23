#include "pch.h"
#include "CameraPlayerMoveScript.h"

#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/Clayer.h>

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

#include <Engine/CDevice.h>


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
	// Resolution
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

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


	// Check Map Size 
	CLayer* DefaultLayer = pCurScene->GetLayer(L"Default2");
	CGameObject* MapSizeObj = DefaultLayer->FindObj(L"MapSize");
	if (MapSizeObj != nullptr)
	{
		Vec3 vMapSizePos = MapSizeObj->Collider2D()->GetWorldPos();
		Vec3 vMapSize = MapSizeObj->Collider2D()->GetWorldScale();

		float Right = vMapSizePos.x + vMapSize.x / 2;
		float Left = vMapSizePos.x - vMapSize.x / 2;
		float Bottom = vMapSizePos.y - vMapSize.y / 2;
		float Top = vMapSizePos.y + vMapSize.y / 2;
		// Check Right Side

		// vDiff_RT -> [x] - Diff Right Side [y] - Diff top Side
		Vec2 vDiff_RT = Vec2(Right - (vPos.x + vRenderResolution.x / 2),
			Top - (vPos.y + vRenderResolution.y / 2));
		
		// vDiff_LB -> [x] - Diff Left Side [y] - Diff Bottom side 
		Vec2 vDiff_LB = Vec2(Left - (vPos.x - vRenderResolution.x / 2),
			Bottom - (vPos.y - vRenderResolution.y / 2));;

		// Check Right Side 
		if (vDiff_RT.x <= 0.f)
		{
			vPos.x = Right - (vRenderResolution.x / 2);;
		}
		// check left Side 
		if (vDiff_LB.x >= 0.f)
		{
			vPos.x = Left + (vRenderResolution.x / 2);
		}
		// Check Top side 
		if (vDiff_RT.y <= 0.f)
		{
			vPos.y = Top - (vRenderResolution.y / 2);
		}

		// check Bottom Side 
		if (vDiff_LB.y >= 0.f)
		{
			vPos.y = Bottom + (vRenderResolution.y / 2);
		}

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

