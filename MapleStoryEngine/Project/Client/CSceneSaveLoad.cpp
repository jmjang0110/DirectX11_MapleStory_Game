#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Script/CScriptMgr.h>


void CSceneSaveLoad::SaveScene(CScene* _pScene, const wstring& _strSceneFilePath)
{
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
        for (size_t i = 0; i < vecRootObj.size(); ++i)
        {
            SaveGameObject(vecRootObj[i], pFile);
        }
    }


    fclose(pFile);
}

void CSceneSaveLoad::SaveGameObject(CGameObject* _pObj, FILE* _pFile)
{
    // 이름, 상태, 컴포넌트 저장
    _pObj->SaveToScene(_pFile);


    // Script 저장
    const vector<CScript*>& vecScript = _pObj->GetScripts();

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

        // Layer 보유 오브젝트 개수
        size_t iObjCount = 0;
        fread(&iObjCount, sizeof(size_t), 1, pFile);

        // Root 오브젝트
        for (size_t i = 0; i < iObjCount; ++i)
        {
            CGameObject* pLoadObj = LoadGameObject(pFile);
            pLayer->AddObject(pLoadObj);
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



    return pLoadObj;
}
