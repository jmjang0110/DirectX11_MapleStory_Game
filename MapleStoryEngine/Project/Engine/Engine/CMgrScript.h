#pragma once
#include "CScript.h"

#include "CSceneMgr.h"
#include "CScene.h"

template<typename T>
class CMgrScript :
    public CScript
{
private:

public:
    T* GetInst()
    {
        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        if (pCurScene->GetSceneState() != SCENE_STATE::PLAY)
            return nullptr;

        CGameObject* pDummyObj = CSceneMgr::GetInst()->FindObjectByName(L"DummyObject");

        if (nullptr == pDummyObj)
        {
            pDummyObj = new CGameObject;
            pDummyObj->AddComponent(new CTransform);
            CSceneMgr::GetInst()->GetCurScene()->AddObject(pDummyObj, 0);
        }

        T* pScript = pDummyObj->GetScript<T>();
        if (nullptr != pScript)
            return pScript;

        pScript = new T;
        pDummyObj->AddComponent(pScript);
        return pScript;
    }

public:
    CMgrScript()
    {

    }

    ~CMgrScript()
    {

    }
};

