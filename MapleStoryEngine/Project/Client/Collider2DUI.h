#pragma once
#include "ComponentUI.h"
class Collider2DUI :
    public ComponentUI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;



public:
    Collider2DUI();
    ~Collider2DUI();

};
