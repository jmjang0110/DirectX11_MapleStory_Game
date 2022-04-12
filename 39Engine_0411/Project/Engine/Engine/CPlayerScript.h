#pragma once
#include "CScript.h"


class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>    m_pMissilePrefab;
    float           m_fSpeed;


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;


    CLONE(CPlayerScript)

public:
    CPlayerScript();
    ~CPlayerScript();

};

