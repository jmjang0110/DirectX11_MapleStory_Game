#pragma once

#include "componentUI.h"

class MeshRenderUI :
    public ComponentUI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

