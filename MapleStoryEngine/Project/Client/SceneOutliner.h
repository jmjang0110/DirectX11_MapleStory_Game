#pragma once
#include "UI.h"

class TreeUI;

class SceneOutliner :
    public UI
{
private:
    TreeUI* m_TreeUI;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    SceneOutliner();
    ~SceneOutliner();
};

