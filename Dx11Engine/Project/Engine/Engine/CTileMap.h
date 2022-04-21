#pragma once
#include "CRenderComponent.h"
#include "CTexture.h"

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_pAtlasTex;
    Vec2            m_vSlicePixel;
    Vec2            m_vSliceUV;


private:
    virtual void render() override;
    virtual void finalupdate() override;
    virtual void UpdateData() override;

    CLONE(CTileMap);


public:
    CTileMap();
    ~CTileMap();


};

