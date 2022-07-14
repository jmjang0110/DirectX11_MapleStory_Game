#pragma once
#include <Engine/CScript.h>
#include "CStateScript.h"

class CAIScript :
    public CScript
{

private:
    map<MONSTER_STATE, CStateScript*>   m_mapState;
    CStateScript*                       m_pCurState;

public:
    void AddState(CStateScript* _pState);
    CStateScript* GetStateScript(MONSTER_STATE _eState);
    void SetCurState(MONSTER_STATE _eState);
    void ChangeState(MONSTER_STATE _eNextState);

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


    CLONE(CAIScript)


public:
    CAIScript();
    CAIScript(const CAIScript& _origin);
    ~CAIScript();

};

