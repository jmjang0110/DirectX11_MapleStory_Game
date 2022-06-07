#pragma once
#include "ResInfoUI.h"

class CComputeShader;

class ComputeShaderUI :
    public ResInfoUI
{

private:

public:
    virtual void update() override;
    virtual void render_update() override;


    // Sub Func 
private:
    void ShowCParticleUpdateShader(CComputeShader*  _pComputeShader);
    void ShowTestShader(CComputeShader* _pComputeShader);


public:
    ComputeShaderUI();
    virtual ~ComputeShaderUI();

};

