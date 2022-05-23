#pragma once
#include "UI.h"

class InspectorUI :
    public UI
{
private:


public:
    virtual void update() override;
    virtual void render_update() override;

public:
    InspectorUI();
    ~InspectorUI();
};

