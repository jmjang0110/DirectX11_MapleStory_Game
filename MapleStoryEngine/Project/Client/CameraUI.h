#pragma once
#include "ComponentUI.h"
class CameraUI :
    public ComponentUI
{
private:


public:
    virtual void update() override;
    virtual void render_update() override;

public:
    CameraUI();
    ~CameraUI();
};

