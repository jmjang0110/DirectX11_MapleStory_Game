#pragma once
#include "ResInfoUI.h"
class MeshUI :
    public ResInfoUI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;


public:
    MeshUI();
    ~MeshUI();
};

