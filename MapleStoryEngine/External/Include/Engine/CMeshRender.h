#pragma once
#include "CRenderComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CMeshRender :
    public CRenderComponent
{
public:
    virtual void finalupdate() override;
    virtual void render() override;


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CMeshRender)



public:
    CMeshRender();    
    ~CMeshRender();
};

