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
#include <Engine/CLight2D.h>

#include <Engine/CSceneFile.h>


// PLayerSCript 가 Script 프로젝트로 바꿨는데
// < ENgine> 에서 바꿨기 때문에 터진거지....;; 

#include <Script/CPlayerScript.h>
#include <Script/CCameraMoveScript.h>
#include <Script/CMissileScript.h>
#include <Script/CCursorScript.h>
#include <Script/CRigidBodyScript.h>
#include <Script/CGravityScript.h>

#include <Engine/CTestShader.h>


void CTestScene::CreateTestScene()
{
	//CResMgr::GetInst()->Load<CSceneFile>(L"scene\\Test3.scene", L"scene\\Test3.scene");
	//CResMgr::GetInst()->Load<CSceneFile>(L"scene\\SibalNoma.scene", L"scene\\SibalNoma.scene");


	CScene* pCurScene = new CScene;
	CSceneMgr::GetInst()->ChangeScene(pCurScene);
	//return;


	pCurScene->SetName(L"firstScene");

	pCurScene->SetLayerName(0, L"Tile");
	pCurScene->SetLayerName(1, L"Default");
	pCurScene->SetLayerName(2, L"Player");
	pCurScene->SetLayerName(3, L"Monster");

	// Texture 한장 로딩해보기
	CResMgr::GetInst()->Load<CTexture>(L"texture\\Player.bmp", L"texture\\Player.bmp");
	CResMgr::GetInst()->Load<CTexture>(L"texture\\MagicCircle.png", L"texture\\MagicCircle.png");
	CResMgr::GetInst()->Load<CTexture>(L"texture\\Tauromacis.png", L"texture\\Tauromacis.png");
	CResMgr::GetInst()->Load<CTexture>(L"texture\\RedGearWheel.png", L"texture\\RedGearWheel.png");
	CResMgr::GetInst()->Load<CTexture>(L"Texture\\0_100.png", L"Texture\\0_100.png");
	CResMgr::GetInst()->Load<CTexture>(L"Texture\\castle.png", L"Texture\\castle.png");
	CResMgr::GetInst()->Load<CTexture>(L"Texture\\test6.png", L"Texture\\test6.png");
	CResMgr::GetInst()->Load<CTexture>(L"Texture\\portal.png", L"Texture\\portal.png");

	CResMgr::GetInst()->Load<CTexture>(L"Texture\\back.0.png", L"Texture\\back.0.png");




	//Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"MagicCircle");

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
	pMissileObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TestMtrl.mtrl"));

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

	//// 광원 오브젝트 추가
	//CGameObject* pLight2D = new CGameObject;
	//pLight2D->SetName(L"Light Object");

	//pLight2D->AddComponent(new CTransform);
	//pLight2D->AddComponent(new CMeshRender);
	//pLight2D->AddComponent(new CLight2D);

	//pLight2D->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	//pLight2D->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	//pLight2D->Light2D()->SetRange(500.f);
	//pLight2D->Light2D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	//pLight2D->Light2D()->SetLightDir(Vec3(0.5f, 0.5f, 0.f));
	//pLight2D->Light2D()->SetAngle(30.f);

	//pCurScene->AddObject(pLight2D, L"Default");


	// Plane Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Background");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 300.f);
	pObject->Transform()->SetRelativeScale(1280, 720.f, 1.f);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DLightMtrl.mtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\Background.png", L"texture\\Background.png"));

	pCurScene->AddObject(pObject, L"Default");

	//// Plane Object 2 
	pObject = new CGameObject;
	pObject->SetName(L"Background_2");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 300.f);
	pObject->Transform()->SetRelativeScale(800.f, 500.f, 1.f);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));
	//pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"BackGroundTex", L"texture//0_100.png"));

	float fLimit = 0.333f;
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObject->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->Load<CTexture>(L"texture\\0_100.png", L"texture\\0_100.png"));


	pCurScene->AddObject(pObject, L"Default");


	// Particle Object 
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"ParticleObject_01");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	pParticleObj->Transform()->SetRelativePos(-200.f, 0.f, 500.f);

	Ptr<CTexture> pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\particle\\AlphaCircle.png", L"texture\\particle\\AlphaCircle.png");
	pParticleObj->ParticleSystem()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pParticleTex);

	pCurScene->AddObject(pParticleObj, L"Default");


	pParticleObj = pParticleObj->Clone();
	pParticleObj->SetName(L"ParticleObject_02");
	pParticleObj->Transform()->SetRelativePos(-500.f, 0.f, 500.f);
	pCurScene->AddObject(pParticleObj, L"Default");








	// PostProcess Object
	CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcessObject");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pPostProcess->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PostProcessMtrl.mtrl"));

	pCurScene->AddObject(pPostProcess, L"Default");
	pPostProcess->Deactivate();


	((CCameraMoveScript*)pCamObj->GetScript(0))->SetFilter(pPostProcess);
	CCameraMoveScript* pCamMoveScript = pCamObj->GetScript<CCameraMoveScript>();


	AddPlayer(pCurScene);
	Add_MapleStory_Player(pCurScene);
	AddCursor(pCurScene);


	pObject = new CGameObject;
	pObject->SetName(L"tile");
	pObject->AddComponent(new CTransform);
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1.5f));

	pObject->AddComponent(new CTileMap);

	Ptr<CTexture> pTileAtlas = CResMgr::GetInst()->Load<CTexture>(L"texture\\tilemap\\YellowToyCastle\\YellowToyCastleTile.png", L"texture\\tilemap\\YellowToyCastle\\YellowToyCastleTile.png");
	pObject->TileMap()->SetAtlasTex(pTileAtlas);
	pObject->TileMap()->SetTileSize(Vec2(30.f, 30.f));
	pObject->TileMap()->SetTileMapCount(12, 20);

	pObject->Transform()->SetRelativeScale(12 * 30.f, 20 * 30.f, 1.f);

	pCurScene->AddObject(pObject, L"Tile");


	


	// 충돌 레이어 설정
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");

	pCurScene->start();
	pCurScene->SetSceneState(SCENE_STATE::PLAY);

}

void CTestScene::AddPlayer(CScene* _pCurScene)
{

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->Load<CTexture>(L"texture\\link_0.png", L"texture\\link_0.png");

	// Player Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Monster");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CAnimator2D);
	pObject->AddComponent(new CPlayerScript);
	pObject->AddComponent(new CRigidBodyScript);


	pObject->Transform()->SetRelativePos(0.f, 0.f, 50.f);
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));

	float fLimit = 0.3333f;
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pAnimAtlas.Get());

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));


	pObject->Animator2D()->CreateAnim(L"WALK_DOWN", pAnimAtlas
		, Vec2(300.f, 300.f), Vec2(0.f, 260.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.2f, 10);


	pObject->Animator2D()->Play(L"WALK_DOWN", true);

	// 광원 오브젝트 추가
	CGameObject* pLight2D = new CGameObject;
	pLight2D->SetName(L"Light Object");

	pLight2D->AddComponent(new CTransform);
	pLight2D->AddComponent(new CMeshRender);
	pLight2D->AddComponent(new CLight2D);

	pLight2D->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	pLight2D->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pLight2D->Light2D()->SetRange(500.f);
	pLight2D->Light2D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	pLight2D->Light2D()->SetLightDir(Vec3(0.5f, 0.5f, 0.f));
	pLight2D->Light2D()->SetAngle(30.f);

	//pCurScene->AddObject(pLight2D, L"Default");
	pObject->AddChild(pLight2D);

	_pCurScene->AddObject(pObject, L"Monster");
}

void CTestScene::AddCursor(CScene* _pCurScene)
{
	Ptr<CTexture> pCursor = CResMgr::GetInst()->Load<CTexture>(L"texture\\Cursor.0.png", L"texture\\Cursor.0.png");

	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Cursor");


	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CCursorScript);
	//pObj->AddComponent(new CAnimator2D);

	pObj->Transform()->SetRelativePos(0.f, 0.f, 50.f);
	pObj->Transform()->SetRelativeScale(Vec3(24.f, 25.f, 1.f));

	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));

	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObj->Collider2D()->SetOffsetScale(Vec2(24.f, 28.f));



	float fLimit = 0.3333f;
	pObj->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObj->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pCursor.Get());

	_pCurScene->AddObject(pObj, L"Default");

}




void CTestScene::Add_MapleStory_Player(CScene* _pCurScene)
{
	Ptr<CTexture> pBody = CResMgr::GetInst()->Load<CTexture>(L"texture\\walk1.png", L"texture\\walk1.png");

	// Body
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"MapleStoryPlayer");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CAnimator2D);
	
	/*pObj->AddComponent(new CRigidBodyScript);
	pObj->AddComponent(new CPlayerScript);*/


	pObj->Transform()->SetRelativePos(0.f, 0.f, 50.f);
	pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));

	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::BOX);
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pObj->Collider2D()->SetOffsetScale(Vec2(31.f, 32.f));


	float fLimit = 0.3333f;
	pObj->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObj->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pBody.Get());


	pObj->Animator2D()->CreateAnim(L"WALK1", pBody, Vec2(300.f, 300.f)
		, Vec2(0.f, 0.f), Vec2(31.f, 32.f), Vec2(31.f, 32.f), 0.035f, 24, 3, 8);


	pObj->Animator2D()->Play(L"WALK1", true);
	Ptr<CTexture> pHead = CResMgr::GetInst()->Load<CTexture>(L"texture\\frontHead.png", L"texture\\frontHead.png");

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
	pObjChild->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));

	pObjChild->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &fLimit);
	pObjChild->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, pHead.Get());


	pObjChild->Animator2D()->CreateAnim(L"HEAD", pHead, Vec2(300.f, 300.f)
		, Vec2(0.f, 0.f), Vec2(35.f, 35.f), Vec2(0.f, 0.f), 10.f, 1, 1, 1);


	pObjChild->Animator2D()->Play(L"HEAD", true);


	// Eye 
	Ptr<CTexture> pEye = CResMgr::GetInst()->Load<CTexture>(L"texture\\face_blink.png", L"texture\\face_blink.png");


	CGameObject* pObjEye = new CGameObject;
	pObjEye->SetName(L"MapleStoryPlayer_Eye");

	pObjEye->AddComponent(new CTransform);
	pObjEye->AddComponent(new CMeshRender);
	pObjEye->AddComponent(new CAnimator2D);

	pObjEye->Transform()->SetRelativePos(-4.f, -3.f, -1.f);
	pObjEye->Transform()->SetIgnoreParentScale(true);
	pObjEye->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObjEye->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObjEye->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std2DMtrl.mtrl"));

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


