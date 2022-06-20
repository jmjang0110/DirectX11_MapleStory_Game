#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Script/CScriptMgr.h>

#include <Engine/CParticleSystem.h>



void CSceneSaveLoad::SaveScene(CScene* _pScene, const wstring& _strSceneFilePath)
{
    // 리소스 변경상태 저장
    CResMgr::GetInst()->SaveChangedRes();


    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strSceneFilePath.c_str(), L"wb");

    assert(pFile);
    if (nullptr == pFile)
        return;

    // Scene 이름 저장
    _pScene->SaveToScene(pFile);

    // Scene 의 Layer 정보 저장
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        // Layer 이름 저장
        CLayer* pLayer = _pScene->GetLayer(i);
        pLayer->SaveToScene(pFile);

        // Layer 가 보유중인 Root 오브젝트 개수 저장
        vector<CGameObject*>& vecRootObj = pLayer->GetRootObjects();
        size_t iObjCount = vecRootObj.size();
        fwrite(&iObjCount, sizeof(size_t), 1, pFile);

        // 각 루트오브젝트 저장
        for (size_t j = 0; j < vecRootObj.size(); ++j)
        {
            SaveGameObject(vecRootObj[j], pFile);
        }
    }


    fclose(pFile);
}

void CSceneSaveLoad::SaveGameObject(CGameObject* _pObj, FILE* _pFile)
{
    // 이름, 상태, 컴포넌트 저장
    _pObj->SaveToScene(_pFile);

    // Script 저장
    //const vector<CScript*>& vecScript = _pObj->GetScripts();


    // Child Object
    const vector<CGameObject*>& vecChild = _pObj->GetChild();
    size_t iChildCount = vecChild.size();
    fwrite(&iChildCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iChildCount; ++i)
    {
        SaveGameObject(vecChild[i], _pFile);
    }
}

CScene* CSceneSaveLoad::LoadScene(const wstring& _strSceneFilePath)
{
    CScene* pLoadScene = new CScene;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strSceneFilePath.c_str(), L"rb");

    assert(pFile);
    if (nullptr == pFile)
        return nullptr;

    // Scene 이름 불러오기
    pLoadScene->LoadFromScene(pFile);

    // Scene 의 Layer 정보 불러오기
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        // Layer 의 이름 
        CLayer* pLayer = pLoadScene->GetLayer(i);
        pLayer->LoadFromScene(pFile);

        // Layer 보유 오브젝트 개수
        size_t iObjCount = 0;
        fread(&iObjCount, sizeof(size_t), 1, pFile);

        // Root 오브젝트
        for (size_t j = 0; j < iObjCount; ++j)
        {
            CGameObject* pLoadObj = LoadGameObject(pFile);
            pLoadScene->AddObject(pLoadObj, i);
        }
    }

    fclose(pFile);

    return pLoadScene;
}

CGameObject* CSceneSaveLoad::LoadGameObject(FILE* _pFile)
{
    CGameObject* pLoadObj = new CGameObject;

    // 이름, 상태, 컴포넌트 불러오기
    pLoadObj->LoadFromScene(_pFile);

    // Script 불러오기    


    size_t iChildCount = 0;
    fread(&iChildCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iChildCount; ++i)
    {
        CGameObject* pChild = LoadGameObject(_pFile);
        pLoadObj->AddChild(pChild);
    }

    return pLoadObj;
}
