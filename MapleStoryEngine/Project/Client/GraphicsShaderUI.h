#pragma once
#include "ResInfoUI.h"
class CGraphicsShader;


class GraphicsShaderUI :
    public ResInfoUI
{

private:

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    GraphicsShaderUI();
    virtual ~GraphicsShaderUI();

    // Sub Func 
private:
    void showPipelineStage(CGraphicsShader* _pShader);


};

