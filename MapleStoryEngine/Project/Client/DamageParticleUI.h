#pragma once
#include "ComponentUI.h"
class DamageParticleUI :
    public ComponentUI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    DamageParticleUI();
    ~DamageParticleUI();

};

