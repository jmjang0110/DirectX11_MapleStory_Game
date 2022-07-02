#pragma once
#include "ComponentUI.h"
class Light2DUI :
    public ComponentUI
{
private:
    tLightInfo  m_LightInfo;
    int         m_iLightIdx;


public:
    virtual void update() override;
    virtual void render_update() override;


public:
    Light2DUI();
    ~Light2DUI();


};

