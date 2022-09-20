#pragma once
#include <Engine/CScript.h>
#include <Engine/CPrefab.h>

/*
    Default Layer ���� Scene �� ���۵��� �� ������ �ϵ��� 
    Script �� ����ϴ�. 
    ������� Scene �� ���۵Ǹ�  Layer �� ����� Player �� Load �� �� �ֽ��ϴ�. 
*/
class CSceneStartScript :
    public CScript
{
private:
    class CGameObject* m_pOwner;
    
    // �ش�Scene ���� �ʿ��� Prefab ���� �̸� Load ���� ���´�. 
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
    // ��� Scene �� ���������� ������ �־�� �� Prefab ���� 
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





