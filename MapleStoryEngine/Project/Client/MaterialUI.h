#pragma once
#include "ResInfoUI.h"

class MaterialUI :
    public ResInfoUI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    MaterialUI();
    ~MaterialUI();
};

