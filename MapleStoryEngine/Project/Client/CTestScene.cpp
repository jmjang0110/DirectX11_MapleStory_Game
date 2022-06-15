#include "pch.h"
#include "CTestScene.h"

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CSceneMgr.h>



#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CCollider2D.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTileMap.h>
#include <Engine/CParticleSystem.h>

// PLayerSCript 가 Script 프로젝트로 바꿨는데
// < ENgine> 에서 바꿨기 때문에 터진거지....;; 

#include <Script/CPlayerScript.h>
#include <Script/CCameraMoveScript.h>
#include <Script/CMissileScript.h>

#include <Engine/CTestShader.h>


void CTestScene::CreateTestScene()
{
	CScene* pCurScene = new CScene;
	pCurScene->SetName(L"firstScene");

	pCurScene->SetLayerName(0, L"Tile");
	pCurScene->SetLayerName(1, L"Default");
	pCurScene->SetLayerName(2, L"Player");
	pCurScene->SetLayerName(3, L"Monster");

	// Texture 한장 로딩해보기
	CResMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\Player.bmp");
	CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\MagicCircle.png");
	CResMgr::GetInst()->Load<CTexture>(L"Tauromacis", L"texture\\Tauromacis.png");
	CResMgr::GetInst()->Load<CTexture>(L"RedWheel", L"texture\\RedGearWheel.png");
	CResMgr::GetInst()->Load<CTexture>(L"Back", L"Texture\\0_100.png");
	CResMgr::GetInst()->Load<CTexture>(L"castle", L"Texture\\castle.png");


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
	//pMissileObj->AddComponent(new CMissileScript);

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

	pCurScene->AddObject(pCamObj, L"Default");


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

	pCurScene->AddObject(pObject, L"Default");


	// Particle Object 
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"ParticleObject_01");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->Transform()->SetRelativePos(-200.f, 0.f, 500.f);

	Ptr<CTexture> pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"Particle_01", L"texture\\particle\\AlphaCircle.png");
	pParticleObj->ParticleSystem()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pParticleTex);

	pCurScene->AddObject(pParticleObj, L"Default");


	//pParticleObj = pParticleObj->Clone();
	//pParticleObj->SetName(L"ParticleObject_02");
	//pParticleObj->Transform()->SetRelativePos(-500.f, 0.f, 500.f);
	//pCurScene->AddObject(pParticleObj, L"Default");




	// PostProcess Object
	CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcessObject");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pPostProcess->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	pCurScene->AddObject(pPostProcess, L"Default");
	pPostProcess->Deactivate();


	((CCameraMoveScript*)pCamObj->GetScript(0))->SetFilter(pPostProcess);
	CCameraMoveScript* pCamMoveScript = pCamObj->GetScript<CCameraMoveScript>();


	//AddPlayer(pCurScene);
	Add_MapleStory_Player(pCurScene);

	pObject = new CGameObject;
	pObject->SetName(L"tile");
	pObject->AddComponent(new CTransform);
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.5f));

	pObject->AddComponent(new CTileMap);


	Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"yellowToyCastleTile", L"texture//tilemap//YellowToyCastle//YellowToyCastleTile.png");
	pObject->TileMap()->SetAtlasTex(pTileAtlas);
	pObject->TileMap()->SetTileSize(Vec2(30.f, 30.f));
	pObject->TileMap()->SetTileMapCount(12, 20);

	pObject->Transform()->SetRelativeScale(12 * 30.f, 20 * 30.f, 1.f);

	pCurScene->AddObject(pObject, L"Tile");


	CSceneMgr::GetInst()->ChangeScene(pCurScene);


	// 충돌 레이어 설정
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");

	pCurScene->start();
	pCurScene->SetSceneState(SCENE_STATE::PLAY);

}

void CTestScene::AddPlayer(CScene* _pCurScene)
{

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->Load<CTexture>(L"PlayerAtlas", L"texture\\link_0.png");

	// Player Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CAnimator2D);
	pObject->AddComponent(new CPlayerScript);

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

	_pCurScene->AddObject(pObject, L"Player");
}




void CTestScene::Add_MapleStory_Player(CScene* _pCurScene)
{
	Ptr<CTexture> pBody = CResMgr::GetInst()->Load<CTexture>(L"walk1", L"texture\\walk1.png");

	// Body
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"MapleStoryPlayer");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CPlayerScript);


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
	_pCurScene->AddObject(pObj, L"Player");
	//pCurScene->AddObject(pObjChild, L"Player");


}


