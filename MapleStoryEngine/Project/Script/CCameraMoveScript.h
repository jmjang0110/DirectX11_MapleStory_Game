#pragma once

#include <Engine/CScript.h>


class CCameraMoveScript :
    public CScript
{
private:
    CGameObject*    m_Filter;
    float           m_fCamSpeed;

   // Vec2 i;


public:
    void SetFilter(CGameObject* _pFilterObject){m_Filter = _pFilterObject;}

public:
    virtual void update() override;
    CLONE(CCameraMoveScript)
public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

