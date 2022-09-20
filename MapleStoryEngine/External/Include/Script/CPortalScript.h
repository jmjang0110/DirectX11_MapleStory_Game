#pragma once
#include <Engine/CScript.h>
class CPortalScript :
    public CScript
{
private:
    wstring m_wNextScene_RelativePath;
    wstring m_wCurScene_RelativePath;

    
    CScene* m_pNextScene;
    CScene* m_pCurScene;


    Vec2    m_vNextSceneStartPos;

public:
    void SavePlayerToPrefab();
    void SaveInventoryToPrefab();
    void SaveQuickSlotToPrefab();



public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;



public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CPortalScript)


public:
    CPortalScript();
    CPortalScript(const CPortalScript& _origin);
    ~CPortalScript();

};

