#pragma once
#include "ComponentUI.h"
class ParticleSystemUI :
    public ComponentUI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    ParticleSystemUI();
    ~ParticleSystemUI();

};

