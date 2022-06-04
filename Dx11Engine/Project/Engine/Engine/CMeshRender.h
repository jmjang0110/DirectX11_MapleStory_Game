#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "CRenderComponent.h"
#include "CMesh.h"
#include "CMaterial.h"


class CMeshRender :
    public CRenderComponent
{
private:


public:
    virtual void finalupdate() override;
    virtual void render() override;

    CLONE(CMeshRender)


public:
    // 이미지 좌우 반전 여부 
    void SetUVReverse(bool _reverse);
    // 이미지 상하 반전 여부 
    void SetUVUpsideDown(bool _upsideDown);

public:
    CMeshRender();
    ~CMeshRender();
};

