#include "pch.h"
#include "CCursorScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>



CCursorScript::CCursorScript()
	: CScript((int)SCRIPT_TYPE::CURSORSCRIPT)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CCursorScript::CCursorScript(const CCursorScript& _origin)
	: CScript((int)SCRIPT_TYPE::CURSORSCRIPT)
{
	SetName(CScriptMgr::GetScriptName(this));

}

CCursorScript::~CCursorScript()
{
}


void CCursorScript::start()
{
}

void CCursorScript::update()
{


	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec3 vMouseScale = GetOwner()->Transform()->GetRelativeScale();

	// find Camera Obj !!  
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(L"Default");
	CGameObject* pCameraObj = nullptr;

	if (nullptr != pLayer)
	{
		vector<CGameObject*> vObjs = pLayer->GetRootObjects();
		for (int i = 0; i < vObjs.size(); ++i)
		{
			if (vObjs[i]->GetName() == L"MainCamera")
			{
				pCameraObj = vObjs[i];
				break;
			}

		}
	}

	if (nullptr != pCameraObj)
	{
		const Vec3 vCameraPos = pCameraObj->Transform()->GetRelativePos();
		float fWidth = pCameraObj->Camera()->GetWidth();
		float fHeight = fWidth / pCameraObj->Camera()->GetAspectRatio();

		// 현재 window 화면창에서 정중앙을 (0.f ,0.f) 한 것을 기준으로 한 좌표값 
		Vec2 vDx11Pos = Vec2(vMousePos.x - (fWidth / 2.f), (fHeight / 2.f) - vMousePos.y);

		// 실제 월드상에서의 좌표값 
		Vec2 vRealPos = vDx11Pos + vCameraPos;

		GetOwner()->Transform()->SetRelativePos(Vec3(vRealPos.x + (vMouseScale.x / 2.f), vRealPos.y - (vMouseScale.y / 2.f), 0.f));


	}


}

void CCursorScript::lateupdate()
{
}

void CCursorScript::OnCollisionEnter(CGameObject* _OtherObject)
{
}

void CCursorScript::OnCollision(CGameObject* _OtherObject)
{
}

void CCursorScript::OnCollisionExit(CGameObject* _OtherObject)
{
}

