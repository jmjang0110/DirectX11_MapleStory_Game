#pragma once
#include "CStateScript.h"


class CHitScript :
    public CStateScript
{
private:
    float m_fTimer;

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


    CLONE(CHitScript)


public:
    CHitScript();
    CHitScript(const CHitScript& _origin);
    virtual ~CHitScript();


};

