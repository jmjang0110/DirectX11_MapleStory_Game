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


    CLONE(CMissileScript);
public:
    CMissileScript();
    ~CMissileScript();
};

