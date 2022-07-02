#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CPrefab.h>

#include "CScriptMgr.h"

#include <Engine/CScript.h>

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
    const vector<CScript*>& vecScript = _pObj->GetScripts();
    size_t iScriptCount = vecScript.size();

    fwrite(&iScriptCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iScriptCount; ++i)
    {
        wstring strScriptName = CScriptMgr::GetScriptName(vecScript[i]);
        SaveWStringToFile(strScriptName, _pFile);
        vecScript[i]->SaveToScene(_pFile);
    }

    // Child Object
    const vector<CGameObject*>& vecChild = _pObj->GetChild();
    size_t iChildCount = vecChild.size();
    fwrite(&iChildCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iChildCount; ++i)
    {
        SaveGameObject(vecChild[i], _pFile);
    }
}

// Todo =====
//void CSceneSaveLoad::SavePrefab(CGameObject* _pProtoObj, CPrefab* _pPrefab, FILE* _pFile)
//{
//    wstring wstrResKey = L"prefab\\" + _pProtoObj->GetName() + L".pref";
//    // Prefab 의 Key 를 저장 = RelativePath 
// 
//    SaveWStringToFile(wstrResKey, _pFile);
//    // Proto Object 저장 
//    SaveGameObject(_pProtoObj, _pFile);
//
//}
// ===========-
CScene* CSceneSaveLoad::LoadScene(const wstring& _strSceneFilePath)
{
    // 최종 경로에서 상대경로만 추출
    wstring strKey = CPathMgr::GetInst()->GetRelativePath(_strSceneFilePath);

    // CResMgr 에서 상대경로를 키값으로, CSceneFile 을 찾아냄
    CResMgr::GetInst()->Load<CSceneFile>(strKey, strKey);

    // Load 할 Scene 생성 및 파일로부터 로딩
    CScene* pLoadScene = new CScene;
    pLoadScene->SetResKey(strKey);

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
    size_t iScriptCount = 0;
    fread(&iScriptCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iScriptCount; ++i)
    {
        wstring strScriptName;
        LoadWStringFromFile(strScriptName, _pFile);
        CScript* pLoadScript = CScriptMgr::GetScript(strScriptName);
        pLoadObj->AddComponent(pLoadScript);
        pLoadScript->LoadFromScene(_pFile);
    }


    size_t iChildCount = 0;
    fread(&iChildCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iChildCount; ++i)
    {
        CGameObject* pChild = LoadGameObject(_pFile);
        pLoadObj->AddChild(pChild);
    }

    return pLoadObj;
}

// Todo ========
//CPrefab* CSceneSaveLoad::LoadPrefab(FILE* _pFile)
//{
//    CPrefab* pLoadPrefab = nullptr;
//    CGameObject* pLoadObj = new CGameObject;
//    wstring PrefabKey = L"";
//
//    LoadWStringFromFile(PrefabKey, _pFile);
//   
//    pLoadObj = LoadGameObject(_pFile);
//    pLoadPrefab = new CPrefab(pLoadObj);
//    pLoadPrefab->Load(PrefabKey);             // Key , RelativePath 저장 
// 
//
//    return pLoadPrefab;
//}
// ================





// ======
// Prefab
// ======
void CSceneSaveLoad::SavePrefab(CPrefab* _Prefab, const wstring& _strFilePath)
{
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

    SaveGameObject(_Prefab->GetProto(), pFile);

    fclose(pFile);
}

int CSceneSaveLoad::LoadPrefab(CPrefab* _Prefab, const wstring& _strFilePath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    if (nullptr == pFile)
        return E_FAIL;

    CGameObject* pProto = LoadGameObject(pFile);
    _Prefab->SetProto(pProto);

    fclose(pFile);

    return S_OK;
}


