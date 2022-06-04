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

     // ======== File ============
public:
    virtual void SaveToFile(FILE* _pFile);
    virtual void LoadFromFile(FILE* _pFile, bool IsPrevRead = false);

    // ======== File ============

public:
    CMeshRender();    
    ~CMeshRender();
};

