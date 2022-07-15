#pragma once
#include "CStateScript.h"
class CAttackStateScript :
    public CStateScript
{
private:


public:
    virtual void Enter();
    virtual void Exit();


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


    CLONE(CAttackStateScript)


public:
    CAttackStateScript();
    CAttackStateScript(const CAttackStateScript& _origin);
    virtual ~CAttackStateScript();

};

