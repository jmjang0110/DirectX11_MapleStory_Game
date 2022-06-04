#pragma once
#include "ResInfoUI.h"

class TextureUI :
    public ResInfoUI
{
private:

public:
    virtual void update() override;
    virtual void render_update() override;


public:
    TextureUI();
    ~TextureUI();
};

