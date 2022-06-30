#include "pch.h"
#include "CToolObjMgr.h"

#include "CGameObjectEx.h"
#include "CCameraEx.h"
#include <Engine\CTransform.h>

#include <Engine\CRenderMgr.h>
#include "CEditorCamScript.h"

CToolObjMgr::CToolObjMgr()
{

}

CToolObjMgr::~CToolObjMgr()
{
	Safe_Del_Vec(m_vecObj);
}

void CToolObjMgr::init()
{
	// Editor Camera
	CGameObjectEx* pToolObj = new CGameObjectEx;
	pToolObj->SetName(L"EditorCamera");
	pToolObj->AddComponent(new CTransform);
	pToolObj->AddComponent(new CCameraEx);
	pToolObj->AddComponent(new CEditorCamScript);

	pToolObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pToolObj->Camera()->SetFar(10000);
	pToolObj->Camera()->CheckLayerMaskAll();

	m_vecObj.push_back(pToolObj);

	CRenderMgr::GetInst()->RegisterEditorCamera(pToolObj->Camera());
}

void CToolObjMgr::progress()
{
	if (CSceneMgr::GetInst()->GetCurScene()->GetSceneState() == SCENE_STATE::PLAY)
		return;

	for (size_t i = 0; i < m_vecObj.size(); ++i)\
	{
		m_vecObj[i]->update();
	}

	for (size_t i = 0; i < m_vecObj.size(); ++i)\
	{
		m_vecObj[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecObj.size(); ++i)\
	{
		m_vecObj[i]->finalupdate();
	}

	for (size_t i = 0; i < m_vecObj.size(); ++i)\
	{
		if (nullptr == m_vecObj[i]->GetRenderComponent()
			|| nullptr == m_vecObj[i]->GetRenderComponent()->GetMesh()
			|| nullptr == m_vecObj[i]->GetRenderComponent()->GetMaterial()
			|| nullptr == m_vecObj[i]->GetRenderComponent()->GetMaterial()->GetShader())
			continue;

		m_vecObj[i]->render();
	}
}
