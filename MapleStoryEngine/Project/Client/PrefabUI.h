#pragma once
#include "ResInfoUI.h"
class PrefabUI :
    public ResInfoUI
{
private:


public:
    virtual void update() override;
    virtual void render_update() override;

public:
    PrefabUI();
    ~PrefabUI();

};

