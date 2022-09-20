#pragma once
#include <Engine/CScript.h>
#include <Engine/CPrefab.h>

/*
    Default Layer 에서 Scene 이 시작됐을 때 시작할 일들을 
    Script 로 만듭니다. 
    예를들어 Scene 이 시작되면  Layer 에 저장된 Player 를 Load 할 수 있습니다. 
*/
class CSceneStartScript :
    public CScript
{
private:
    class CGameObject* m_pOwner;
    
    // 해당Scene 에서 필요한 Prefab 들을 미리 Load 시켜 놓는다. 
    map<wstring, CPrefab*> m_MapPrefab;
    map<wstring, CPrefab*> m_MapMonsterPrefab;

private:
    Ptr<CSound> pSound_Bgm;
    CScene* m_pCurScene;

private:
    float m_fTimer;


private:
    bool        m_bBossEnd;


public:
    void InsertPrefab(wstring _name);
    void InsertMonsterPrefab(wstring _name);

    void InsertPrefab_RelatedPlayer();
    void InsertPrefab_RelatedMonster(wstring _SceneName);

public:
    // 모든 Scene 이 공통적으로 가지고 있어야 할 Prefab 저장 
    void LoadDefaultPrefab();

public:
    CPrefab* GetPrefab(wstring _name);
    CPrefab* GetMonsterPrefab(wstring _name);
    CPrefab* GetMonsterPrefab(int idx);


public:
    void SavePlayerToPrefab(Vec2 _vnextScenePlayerpos);
    void SaveInventoryToPrefab();
    void SaveQuickSlotToPrefab();

    void ChangeBackGroundBgm(wstring _BgmName);

public:
    void End();

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


    CLONE(CSceneStartScript)


public:
    CSceneStartScript();
    CSceneStartScript(const CSceneStartScript& _origin);
    ~CSceneStartScript();


    friend class CMonsterScript;

};





