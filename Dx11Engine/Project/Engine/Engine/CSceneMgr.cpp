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
#include "CMonsterScript.h"

#include "CTexture.h"
#include "CPrefab.h"

#include "CTestShader.h"

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
	// ----- Layer 설정 -----  
	m_pCurScene = new CScene;	
	m_pCurScene->SetLayerName(0, L"Tile");
	m_pCurScene->SetLayerName(1, L"Default");
	m_pCurScene->SetLayerName(2, L"Player");
	m_pCurScene->SetLayerName(3, L"Monster");
	
	// ----- Texture Load -> CResMgr -----
	LoadTextures();

	// ----- MAKE PREFAB -----
	AddMissilePrefab();

	// ----- MAKE TOOL OBJECT ------
	AddCameraObj();
	
	// ----- MAKE RENDER OBJECT -----
	//AddTauromacisObj();
	//AddMagicClaw();
	AddAncientMixGolem();

	//AddMainPlayerObj();
	//AddMonsterObj();
	//AddTileMapObj();
	//AddAttack1AnimObj();


	/*
		// 주의 사항 
		D3D11_BIND_UNORDERED_ACCESS
		D3D11_BIND_SHADER_RESOURCE
		조합은 가능하지만 여기에 추가해서 

		D3D11_BIND_DEPTH_STENCIL 은 불가능하다 .
		Depth Stencil 바인딩은 따로 (유일하게)설정해야한다. 
		
	*/

	// Texture Create 하기
	Ptr<CTexture> pTestTex = CResMgr::GetInst()->CreateTexture(L"TestTexture", 1024, 1024
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	// ComputeShader 실행하기
	Ptr<CTestShader> pCS = (CTestShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"TestCS").Get();

	pCS->SetOutputTexture(pTestTex);
	pCS->SetColor(Vec4(0.f, 1.f, 0.f, 1.f));
	pCS->Excute();


	//AddParticle();

	// Particle Object Star
	/*CGameObject* pParticleObj = new CGameObject;
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->ParticleSystem()->SetParticleType(PARTICLE_TYPE::HYPO_STAR);
	pParticleObj->Transform()->SetRelativePos(0.f, 200.f, 500.f);

	m_pCurScene->AddObject(pParticleObj, L"Default");*/

	
	
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");

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

	//m_pCurScene->render();

	CDevice::GetInst()->Present();
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


void CSceneMgr::LoadTextures()
{
	/*
		필요한 텍스처들을 CResMgr에 로드합니다. 
		->	이후 CResMgr::GetInst()->Findres() 를 통해서
			텍스처 리소스를 받아 사용합니다. 
	*/
	CResMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Player.bmp");
	CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\MagicCircle.png");

	// Monster Texture Load 
	CResMgr::GetInst()->Load<CTexture>(L"Tauromacis", L"texture\\MapleStoryMonster\\Tauromacis.png");
	CResMgr::GetInst()->Load<CTexture>(L"Attack1", L"texture\\Attack1.png");


}

void CSceneMgr::AddMainPlayerObj()
{
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle");


	// Player Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CAnimator2D);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 500.f);
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	float fLimit = 0.3333f;
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex.Get());

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->Load<CTexture>(L"PlayerAtlas", L"texture\\link_0.png");
	pObject->Animator2D()->CreateAnim(L"WALK_DOWN", pAnimAtlas, Vec2(200.f, 200.f)
		, Vec2(0.f, 260.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.2f, 10, false);




	pObject->Animator2D()->Play(L"WALK_DOWN", true);


	CGameObject* pChildObj = pObject->Clone();
	pChildObj->SetName(L"ChildObject");
	pChildObj->Transform()->SetIgnoreParentScale(true);
	pChildObj->Transform()->SetRelativePos(200.f, 0.f, 0.f);
	pChildObj->Transform()->SetRelativeScale(50.f, 50.f, 50.f);

	pChildObj->Collider2D()->SetOffsetPos(0.f, 0.f);
	pChildObj->Collider2D()->SetOffsetScale(50.f, 50.f);


	pObject->AddChild(pChildObj);
	pObject->AddComponent(new CPlayerScript);

	m_pCurScene->AddObject(pObject, L"Player");


}

void CSceneMgr::AddMonsterObj()
{
	CGameObject* pObject = new CGameObject;

	// Monster Object
	pObject = new CGameObject;
	pObject->SetName(L"Monster");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMissileScript);

	pObject->Transform()->SetRelativePos(Vec3(400.f, 0.f, 500.f));
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	m_pCurScene->AddObject(pObject, L"Monster");


}

void CSceneMgr::AddTauromacisObj()
{

	CGameObject* pObj = new CGameObject;
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Tauromacis");


	pObj->SetName(L"Tauromacis");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));


	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	float fLimit = 0.3333f;
	pObj->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObj->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex.Get());
	
	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObj->Collider2D()->SetOffsetScale(Vec2(200.f, 200.f));

	pObj->Animator2D()->CreateAnim(L"STOP", pTex, Vec2(200.f, 200.f)
		, Vec2(5.f, 0.f), Vec2(176.f ,160.f), Vec2(186.25f, 0.f), 0.2f, 7, false);




	pObj->Animator2D()->Play(L"STOP", true);
	m_pCurScene->AddObject(pObj, L"Monster");


	
}

void CSceneMgr::AddTileMapObj()
{
	// TileMap Object
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
	pObject->Transform()->SetRelativeScale(1000.f, 1000.f, 1.f);

	pObject->AddComponent(new CTileMap);

	Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"TileMapAtlas", L"texture//TILE_32.bmp");
	pObject->TileMap()->SetAtlasTex(pTileAtlas);
	pObject->TileMap()->SetTileSize(Vec2(64.f, 64.f));
	pObject->TileMap()->SetTileMapCount(16, 16);

	for (int i = 0; i < 8; ++i)
	{
		pObject->TileMap()->SetTileData(i, 0);
	}

	for (int i = 8; i < 256; ++i)
	{
		pObject->TileMap()->SetTileData(i, 2);
	}
	pObject->TileMap()->SetTileData(7, -1);

	m_pCurScene->AddObject(pObject, L"Tile");


}

void CSceneMgr::AddMagicClaw()
{
	Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(L"MagicClaw", L"texture//MagicClaw.png");
	CGameObject* pObj = new CGameObject;


	pObj->SetName(L"MagicClaw");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));


	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	float fLimit = 0.3333f;
	pObj->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObj->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex.Get());

	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObj->Collider2D()->SetOffsetScale(Vec2(200.f, 200.f));

	pObj->Animator2D()->CreateAnim(L"HIT", pTex, Vec2(500.f, 500.f)
		, Vec2(0.f, 0.f), Vec2(102.f, 100.f), Vec2(102.f, 0.f), 0.3f, 5, false);

	pObj->Animator2D()->Play(L"HIT", true);
	m_pCurScene->AddObject(pObj, L"Monster");



}

void CSceneMgr::AddAncientMixGolem()
{
	Ptr<CTexture> pTex_Die = CResMgr::GetInst()->Load<CTexture>(L"AncientMixGolem_Die", L"texture//AncientMixGolem//Die.png");
	Ptr<CTexture> pTex_Move = CResMgr::GetInst()->Load<CTexture>(L"AncientMixGolem_Move", L"texture//AncientMixGolem//Move.png");
	Ptr<CTexture> pTex_Stand = CResMgr::GetInst()->Load<CTexture>(L"AncientMixGolem_Stand", L"texture//AncientMixGolem//Stand.png");
	Ptr<CTexture> pTex_Attack1 = CResMgr::GetInst()->Load<CTexture>(L"AncientMixGolem_Attack1", L"texture//AncientMixGolem//Attack1.png");
	Ptr<CTexture> pTex_Skill1 = CResMgr::GetInst()->Load<CTexture>(L"AncientMixGolem_Skill1", L"texture//AncientMixGolem//Skill1.png");
	Ptr<CTexture> pTex_Attack2 = CResMgr::GetInst()->Load<CTexture>(L"AncientMixGolem_Attack2", L"texture//AncientMixGolem//Attack2.png");
	Ptr<CTexture> pTex_Skill2 = CResMgr::GetInst()->Load<CTexture>(L"AncientMixGolem_Skill2", L"texture//AncientMixGolem//Skill2.png");
	
	CGameObject* pObj = new CGameObject;


	pObj->SetName(L"AncientMixGolem");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CMonsterScript);


	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	float fLimit = 0.3333f;
	//float fLimit = 0.4444f;
	pObj->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObj->MeshRender()->SetUVReverse(true);
	pObj->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex_Stand.Get());

	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	pObj->Animator2D()->CreateAnim(L"MOVE", pTex_Move, Vec2(500.f, 500.f)
		, Vec2(0.f, 0.f), Vec2(233.f, 191.f), Vec2(233.f, 0.f), 0.15f, 8, false);

	pObj->Animator2D()->CreateAnim(L"DIE", pTex_Die, Vec2(500.f, 500.f)
		, Vec2(0.f, 0.f), Vec2(303.f, 221.f), Vec2(303.f, 221.f), 0.1f, 19, 3, 8);

	pObj->Animator2D()->CreateAnim(L"STAND", pTex_Stand, Vec2(500.f, 500.f)
		, Vec2(0.f, 0.f), Vec2(230.f, 188.f), Vec2(230.f, 0.f), 0.15f, 6 , false);

	pObj->Animator2D()->CreateAnim(L"ATTACK1", pTex_Attack1, Vec2(500.f, 500.f)
		, Vec2(0.f, 0.f), Vec2(431.f, 235.f), Vec2(431.f, 249.f), 0.15f, 21, 3, 8);

	pObj->Animator2D()->CreateAnim(L"SKILL1", pTex_Skill1, Vec2(500.f, 500.f)
		, Vec2(0.f, 0.f), Vec2(164.f, 148.f), Vec2(164.f, 148.f), 0.1f, 5, 1, 5);

	pObj->Animator2D()->CreateAnim(L"ATTACK2", pTex_Attack2, Vec2(500.f, 500.f)
		, Vec2(0.f, 0.f), Vec2(270.f, 267.f), Vec2(270.f, 250.f), 0.15f, 27, 4, 8);

	pObj->Animator2D()->CreateAnim(L"SKILL2", pTex_Skill2, Vec2(500.f, 500.f)
		, Vec2(0.f, 0.f), Vec2(352.f, 448.f), Vec2(352.f, 448.f), 0.1f,16, 2, 8);
	   

	pObj->Animator2D()->Play(L"SKILL2", true);

	m_pCurScene->AddObject(pObj, L"Monster");
}



void CSceneMgr::AddCameraObj()
{
	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();


	m_pCurScene->AddObject(pCamObj, L"Default");

}

void CSceneMgr::AddMissilePrefab()
{
	CGameObject* pMissileObj = new CGameObject;
	pMissileObj->AddComponent(new CTransform);
	pMissileObj->AddComponent(new CMeshRender);
	pMissileObj->AddComponent(new CMissileScript);

	pMissileObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	pMissileObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	pMissileObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	CResMgr::GetInst()->AddRes<CPrefab>(L"MissilePrefab", new CPrefab(pMissileObj));



}

void CSceneMgr::AddParticle()
{

	// Particle Object 
	/*CGameObject* pParticleObj = new CGameObject;
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->ParticleSystem()->SetParticleType(PARTICLE_TYPE::BOMB);
	pParticleObj->Transform()->SetRelativePos(0.f, 0.f, 500.f);

	m_pCurScene->AddObject(pParticleObj, L"Default");*/
	//CGameObject* pParticleObj = new CGameObject;
	//// Test PArtcile
	/*pParticleObj = new CGameObject;
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->ParticleSystem()->SetParticleType(PARTICLE_TYPE::MAGIC_CIRCLE);
	pParticleObj->Transform()->SetRelativePos(300.f, 0.f, 500.f);

	m_pCurScene->AddObject(pParticleObj, L"Default");*/


	//// Test PArtcile
	/*pParticleObj = new CGameObject;
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->ParticleSystem()->SetParticleType(PARTICLE_TYPE::FIRECRACKER);
	pParticleObj->Transform()->SetRelativePos(-0.f, -20.f, 500.f);

	m_pCurScene->AddObject(pParticleObj, L"Default");

	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");*/

	//// Test PArtcile
	//pParticleObj = new CGameObject;
	//pParticleObj->AddComponent(new CTransform);
	//pParticleObj->AddComponent(new CParticleSystem);

	//pParticleObj->ParticleSystem()->SetParticleType(PARTICLE_TYPE::BUFF_EFFECT);
	//pParticleObj->Transform()->SetRelativePos(-0.f, -300.f, 500.f);

	//m_pCurScene->AddObject(pParticleObj, L"Default");




	//// Test PArtcile
	//pParticleObj = new CGameObject;
	//pParticleObj->AddComponent(new CTransform);
	//pParticleObj->AddComponent(new CParticleSystem);

	//pParticleObj->ParticleSystem()->SetParticleType(PARTICLE_TYPE::ICE_SPRING);
	//pParticleObj->Transform()->SetRelativePos(-0.f, -300.f, 500.f);

	//m_pCurScene->AddObject(pParticleObj, L"Default");


	////// Test PArtcile
	//pParticleObj = new CGameObject;
	//pParticleObj->AddComponent(new CTransform);
	//pParticleObj->AddComponent(new CParticleSystem);

	//pParticleObj->ParticleSystem()->SetParticleType(PARTICLE_TYPE::ICE_SPIKE);
	//pParticleObj->Transform()->SetRelativePos(-0.f, -300.f, 500.f);

	//m_pCurScene->AddObject(pParticleObj, L"Default");

}




void CSceneMgr::ClearLayer()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_pCurScene->GetLayer(i)->Clear();
	}
}




void CSceneMgr::AddAttack1AnimObj()
{

	CGameObject* pObj = new CGameObject;
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"Attack1");


	pObj->SetName(L"Attack1");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CAnimator2D);

	pObj->Transform()->SetRelativePos(Vec3(-400.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));


	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	float fLimit = 0.3333f;
	pObj->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);



	pObj->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex.Get());

	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObj->Collider2D()->SetOffsetScale(Vec2(200.f, 200.f));

	pObj->Animator2D()->CreateAnim(L"ATTACK1", pTex, Vec2(200.f, 200.f)
		, Vec2(5.f, 0.f), Vec2(176.f, 160.f), Vec2(186.25f, 0.f), 0.2f, 7, false);




	pObj->Animator2D()->Play(L"ATTACK1", true);
	m_pCurScene->AddObject(pObj, L"Default");

}

