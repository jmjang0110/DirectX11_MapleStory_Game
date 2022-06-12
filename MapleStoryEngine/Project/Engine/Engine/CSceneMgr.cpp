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
#include "CAnimator2D.h"
#include "CTileMap.h"
#include "CParticleSystem.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CMissileScript.h"

#include "CTexture.h"
#include "CPrefab.h"

#include "CTestShader.h"

#include "CCore.h"

#include "CFileMgr.h"



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
	m_pCurScene->SetName(L"firstScene");

	m_pCurScene->SetLayerName(0, L"Tile");
	m_pCurScene->SetLayerName(1, L"Default");
	m_pCurScene->SetLayerName(2, L"Player");
	m_pCurScene->SetLayerName(3, L"Monster");

	// Texture 한장 로딩해보기
	CResMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Player.bmp");
	CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\MagicCircle.png");
	CResMgr::GetInst()->Load<CTexture>(L"Tauromacis", L"texture\\Tauromacis.png");
	CResMgr::GetInst()->Load<CTexture>(L"RedWheel", L"texture\\RedGearWheel.png");
	CResMgr::GetInst()->Load<CTexture>(L"Back", L"Texture\\0_100.png");

	
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle");

	// Texture Create 하기
	Ptr<CTexture> pTestTex = CResMgr::GetInst()->CreateTexture(L"TestTexture", 1024, 1024
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	// ComputeShader 실행하기
	Ptr<CTestShader> pCS = (CTestShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"TestCS").Get();
	
	pCS->SetOutputTexture(pTestTex);
	pCS->SetColor(Vec4(0.f, 1.f, 0.f, 1.f));
	pCS->Excute();


	// Prefab 제작
	CGameObject* pMissileObj = new CGameObject;
	pMissileObj->AddComponent(new CTransform);
	pMissileObj->AddComponent(new CMeshRender);
	pMissileObj->AddComponent(new CMissileScript);
		
	pMissileObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	pMissileObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	pMissileObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	
	CResMgr::GetInst()->AddRes<CPrefab>(L"MissilePrefab", new CPrefab(pMissileObj));


	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();

	m_pCurScene->AddObject(pCamObj, L"Default");
	

	// Plane Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Background");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 500.f);
	pObject->Transform()->SetRelativeScale(1600.f, 900.f, 1.f);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PaperBurnMtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"BackGroundTex", L"texture\\Background.png"));

	m_pCurScene->AddObject(pObject, L"Default");


	// Particle Object 
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"ParticleObject_01");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->Transform()->SetRelativePos(-200.f, 0.f, 500.f);

	Ptr<CTexture> pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"Particle_01", L"texture\\particle\\AlphaCircle.png");
	pParticleObj->ParticleSystem()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pParticleTex);

	m_pCurScene->AddObject(pParticleObj, L"Default");


	//pParticleObj = pParticleObj->Clone();
	//pParticleObj->SetName(L"ParticleObject_02");
	//pParticleObj->Transform()->SetRelativePos(-500.f, 0.f, 500.f);
	//m_pCurScene->AddObject(pParticleObj, L"Default");




	// PostProcess Object
	CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcessObject");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pPostProcess->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	m_pCurScene->AddObject(pPostProcess, L"Default");
	pPostProcess->Deactivate();


	((CCameraMoveScript*)pCamObj->GetScript(0))->SetFilter(pPostProcess);
	CCameraMoveScript* pCamMoveScript = pCamObj->GetScript<CCameraMoveScript>();


	//AddPlayer();
	Add_MapleStory_Player();

	pObject = new CGameObject;
	pObject->SetName(L"tile");
	pObject->AddComponent(new CTransform);
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.5f));
	pObject->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pObject->AddComponent(new CTileMap);

	Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"yellowToyCastleTile", L"texture//tilemap//yellowToyCastle//yellowToyCastleTile.png");
	pObject->TileMap()->SetAtlasTex(pTileAtlas);
	pObject->TileMap()->SetTileSize(Vec2(30.f, 30.f));
	pObject->TileMap()->SetTileMapCount(9, 16);


	for (int i = 0; i < 9 * 16; ++i)
	{
		pObject->TileMap()->SetTileData(i, i);
	}

	/*for (int i = 8; i < 16; ++i)
	{
		pObject->TileMap()->SetTileData(i, 2);
	}
	pObject->TileMap()->SetTileData(7, -1);*/

	m_pCurScene->AddObject(pObject, L"Tile");


	// 충돌 레이어 설정
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");

	

	//m_pCurScene->SetSceneState(SCENE_STATE::PLAY);
	m_pCurScene->start();
}

void CSceneMgr::progress()
{
	if (KEY_TAP(KEY::T))
	{
		CFileMgr::GetInst()->LoadFromFile<CScene>((DWORD_PTR)m_pCurScene);

		//CSceneMgr::GetInst()->LoadFromFile();
		//CSceneMgr::GetInst()->SaveToFile();

	}
	if (KEY_TAP(KEY::U))
	{

		CFileMgr::GetInst()->SaveToFile<CScene>((DWORD_PTR)m_pCurScene);

	}

	SCENE_STATE eState = m_pCurScene->GetSceneState();

	if (SCENE_STATE::PLAY == eState)
	{
		m_pCurScene->update();
		m_pCurScene->lateupdate();
	}
	
	m_pCurScene->finalupdate();

	// Collision Check
	if (SCENE_STATE::PLAY == eState)
	{
		CCollisionMgr::GetInst()->update();
	}
}


void CSceneMgr::AddPlayer()
{

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->Load<CTexture>(L"PlayerAtlas", L"texture\\link_0.png");

	// Player Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CAnimator2D);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 50.f);
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	float fLimit = 0.3333f;
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pAnimAtlas.Get());

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	
	pObject->Animator2D()->CreateAnim(L"WALK_DOWN", pAnimAtlas
		, Vec2(300.f, 300.f), Vec2(0.f, 260.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.2f, 10);


	pObject->Animator2D()->Play(L"WALK_DOWN", true);

	m_pCurScene->AddObject(pObject, L"Player");
}




void CSceneMgr::SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)_pSpawnObject;
	info.wParam = (DWORD_PTR)_iLayerIdx;

	_pSpawnObject->Transform()->SetRelativePos(_vWorldPos);
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

void CSceneMgr::DisconnectParent(CGameObject* _pObject)
{
	tEventInfo info = {};
	info.eType = EVENT_TYPE::DISCONNECT_PARENT;
	info.lParam = (DWORD_PTR)_pObject;

	CEventMgr::GetInst()->AddEvent(info);
}


void CSceneMgr::ClearLayer()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pCurScene->GetLayer(i)->Clear();
	}
}

CGameObject* CSceneMgr::FindObjectByName(const wstring& _strName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = m_pCurScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (_strName == vecObj[j]->GetName())
			{
				return vecObj[j];
			}
		}
	}

	return nullptr;
}

void CSceneMgr::FindObjectsByName(wstring& _strName, vector<CGameObject*>& _vecOut)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = m_pCurScene->GetLayer(i);
		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (_strName == vecObj[j]->GetName())
			{
				_vecOut.push_back(vecObj[j]);
			}
		}
	}
}


void CSceneMgr::Add_MapleStory_Player()
{
	Ptr<CTexture> pBody = CResMgr::GetInst()->Load<CTexture>(L"walk1", L"texture\\walk1.png");

	// Body
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"MapleStoryPlayer");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CAnimator2D);
	//pObj->AddComponent(new CPlayerScript);


	pObj->Transform()->SetRelativePos(0.f, 0.f, 50.f);
	pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObj->Collider2D()->SetOffsetScale(Vec2(31.f, 32.f));


	float fLimit = 0.3333f;
	pObj->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObj->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pBody.Get());


	pObj->Animator2D()->CreateAnim(L"WALK1", pBody, Vec2(300.f, 300.f)
		, Vec2(0.f, 0.f), Vec2(31.f, 32.f), Vec2(31.f, 32.f), 0.035f, 24, 3, 8);


	pObj->Animator2D()->Play(L"WALK1", true);
	Ptr<CTexture> pHead = CResMgr::GetInst()->Load<CTexture>(L"frontHead", L"texture\\frontHead.png");

	// Head
	CGameObject* pObjChild = new CGameObject;
	pObjChild->SetName(L"MapleStoryPlayer_frontHead");

	pObjChild->AddComponent(new CTransform);
	pObjChild->AddComponent(new CCollider2D);
	pObjChild->AddComponent(new CMeshRender);
	pObjChild->AddComponent(new CAnimator2D);

	pObjChild->Transform()->SetRelativePos(0.f, 32.f, 0.f);
	pObjChild->Transform()->SetIgnoreParentScale(true);
	pObjChild->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObjChild->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pObjChild->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObjChild->Collider2D()->SetOffsetScale(Vec2(35.f, 35.f));

	pObjChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObjChild->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObjChild->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObjChild->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pHead.Get());


	pObjChild->Animator2D()->CreateAnim(L"HEAD", pHead, Vec2(300.f, 300.f)
		, Vec2(0.f, 0.f), Vec2(35.f, 35.f), Vec2(0.f, 0.f), 10.f, 1, 1, 1);


	pObjChild->Animator2D()->Play(L"HEAD", true);


	// Eye 
	Ptr<CTexture> pEye = CResMgr::GetInst()->Load<CTexture>(L"FaceBlink", L"texture\\face_blink.png");


	CGameObject* pObjEye = new CGameObject;
	pObjEye->SetName(L"MapleStoryPlayer_Eye");

	pObjEye->AddComponent(new CTransform);
	pObjEye->AddComponent(new CMeshRender);
	pObjEye->AddComponent(new CAnimator2D);

	pObjEye->Transform()->SetRelativePos(-4.f, -3.f, -1.f);
	pObjEye->Transform()->SetIgnoreParentScale(true);
	pObjEye->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObjEye->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObjEye->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObjEye->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObjEye->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pEye.Get());
	
	pObjEye->Animator2D()->CreateAnim(L"BLINK", pEye, Vec2(300.f, 300.f)
		, Vec2(0.f, 0.f), Vec2(26.f, 16.f), Vec2(26.f, 16.f), 0.1f, 6, 2, 3);


	pObjEye->Animator2D()->Play(L"BLINK", true);

	pObjChild->AddChild(pObjEye);
	pObj->AddChild(pObjChild);
	m_pCurScene->AddObject(pObj, L"Player");
	//m_pCurScene->AddObject(pObjChild, L"Player");


}


 


