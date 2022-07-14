#pragma once
#include <Engine/CScript.h>


/*
    Monster를 Scene 에 자동생성되게 하기 위한 Script
    - Default Layer 에 MonsterFactory GameObject 를 추기
    - 이 Obj 는 Monster Layer 에 Monster 들을 추가 한다. 
*/
class CMonsterFactoryScript :
    public CScript
{



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

