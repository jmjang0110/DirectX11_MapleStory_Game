#pragma once
#include "CStateScript.h"


class CPatrolStateScript :
    public CStateScript
{
private:

    float       m_fPatrolDist;
    float       m_fTimer;
   
    float       m_fMove;
    int         m_iDir;




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


    CLONE(CPatrolStateScript)


public:
    CPatrolStateScript();
    CPatrolStateScript(const CPatrolStateScript& _origin);
    virtual ~CPatrolStateScript();
};

