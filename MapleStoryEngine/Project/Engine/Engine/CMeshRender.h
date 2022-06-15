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

    CLONE(CMeshRender)



public:
    CMeshRender();    
    ~CMeshRender();
};

