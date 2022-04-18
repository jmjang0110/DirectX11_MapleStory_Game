#pragma once
#include "CScript.h"

class CMissileScript :
    public CScript
{
private:
    float       m_fSpeed;
    float       m_fAccTime;

public:
    virtual void update();

    void OnCollisionEnter(CGameObject* _pOtherObj) override;


    CLONE(CMissileScript);
public:
    CMissileScript();
    ~CMissileScript();
};

