#pragma once
#include <Engine/CScript.h>


/*
    Monster�� Scene �� �ڵ������ǰ� �ϱ� ���� Script
    - Default Layer �� MonsterFactory GameObject �� �߱�
    - �� Obj �� Monster Layer �� Monster ���� �߰� �Ѵ�. 
*/
class CMonsterFactoryScript :
    public CScript
{

private:

    wstring         m_wCurSceneName;
    //map<wstring, CPrefab*> m_MapPrefabMob;

private:
    float           m_fTimer;




public:
    void ChangeScene(wstring CurSceneName);
    //void AddPrefabMob(CPrefab* _prefabMob) { m_MapPrefabMob.insert(make_pair(_prefabMob->GetName(), _prefabMob)); }

    void CreateMonster(int _cnt, wstring _MobName, class CMobGroundScript* _script);
    int GetRandomNum(float _min, float _max);



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


    CLONE(CMonsterFactoryScript)


public:
    CMonsterFactoryScript();
    CMonsterFactoryScript(const CMonsterFactoryScript& _origin);
    ~CMonsterFactoryScript();


};

