#include "pch.h"
#include "CSceneMgr.h"

#include "CEventMgr.h"
#include "CCollisionMgr.h"

#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

#include "CScene.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CCamera.h"
#include "CCollider2D.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CMissileScript.h"

#include "CTexture.h"
#include "CPrefab.h"



CSceneMgr::CSceneMgr()
	: m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}


void CSceneMgr::init()
{
	m_pCurScene = new CScene;	
	m_pCurScene->SetLayerName(0, L"Default");
	m_pCurScene->SetLayerName(1, L"Player");
	m_pCurScene->SetLayerName(2, L"Monster");
	
	// Texture 한장 로딩해보기
	CResMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Player.bmp");
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerTexture");

	// Prefab 제작
	CGameObject* pMissileObj = new CGameObject;
	pMissileObj->AddComponent(new CTransform);
	pMissileObj->AddComponent(new CMeshRender);
	pMissileObj->AddComponent(new CMissileScript);
		
	pMissileObj->Transform()->SetScale(Vec3(50.f, 50.f, 1.f));
	pMissileObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	pMissileObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	
	CResMgr::GetInst()->AddRes<CPrefab>(L"MissilePrefab", new CPrefab(pMissileObj));


	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	m_pCurScene->AddObject(pCamObj, L"Default");

	// Player Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	int a = 0;	
	pObject->MeshRender()->GetMaterial()->SetScalarParam(L"IsColorRed", &a);
	pObject->MeshRender()->GetMaterial()->SetTexParam(L"OutputTex", pTex);

	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	m_pCurScene->AddObject(pObject, L"Player");

	// Monster Object
	pObject = new CGameObject;
	pObject->SetName(L"Missile");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);	
	pObject->AddComponent(new CMissileScript);

	pObject->Transform()->SetPos(Vec3(400.f, 0.f, 0.f));
	pObject->Transform()->SetScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	m_pCurScene->AddObject(pObject, L"Monster");




	CCollisionMgr::GetInst()->CollisionCheck(1, 2);


	m_pCurScene->start();
}

void CSceneMgr::progress()
{
	m_pCurScene->update();
	m_pCurScene->lateupdate();
	m_pCurScene->finalupdate();
}

void CSceneMgr::render()
{
	CDevice::GetInst()->ClearTarget();

	m_pCurScene->render();

	CDevice::GetInst()->Present();
}



void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)_pSpawnObject;
	info.wParam = (DWORD_PTR)_iLayerIdx;

	_pSpawnObject->Transform()->SetPos(_vWorldPos);
	_pSpawnObject->SetName(_strName);

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)_pSpawnObject;
	info.wParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(info);
}

void CSceneMgr::AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::ADD_CHILD;
	info.lParam = (DWORD_PTR)_pParentObject;
	info.wParam = (DWORD_PTR)_pChildObject;

	CEventMgr::GetInst()->AddEvent(info);
}
