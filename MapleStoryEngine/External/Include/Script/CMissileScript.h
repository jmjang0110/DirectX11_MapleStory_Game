#pragma once

#include <Engine/CScript.h>


class CMissileScript :
    public CScript
{
private:
    float       m_fSpeed;
    float       m_fAccTime;

public:
    virtual void update();

    virtual void OnCollisionEnter(CGameObject* _pOtherObj) override;

    CLONE(CMissileScript);
public:
    CMissileScript();
    ~CMissileScript();
};

