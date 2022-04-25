#pragma once
#include "CRenderComponent.h"

#include "CTexture.h"

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>        m_pAtlasTex;        // 텍스쳐 정보  
    Vec2                 m_vSlicePixel;      // 원하는 타일의 가로 세로 Pixel 길이 
    Vec2                 m_vSliceUV;         // 원하는 타일의 가로 세로 UV 길이 
  
    UINT                 m_iRowCount;
    UINT                 m_iColCount;


    //  몇개 짜리 타일 맵인지 지정 : X * Y 개 타일맵
    UINT                 m_iTileCountX;
    UINT                 m_iTileCountY;

    vector<tTileData>    m_vecTileData; // 타일별 정보를 vector 로 관리한다. 
     

public:
    void SetAtlasTex(Ptr<CTexture> _pAtlasTex)
    {
        m_pAtlasTex = _pAtlasTex;
        m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileSize(Vec2 _vPixelSize)
    {
        m_vSlicePixel = _vPixelSize;
        if (nullptr != m_pAtlasTex)
            m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileMapCount(UINT _iCountX, UINT _iCountY);
    void SetTileData(int _iTileIdx, int _iImgIdx);
    void ClearTileData();


public:
    virtual void finalupdate() override;
    virtual void render() override;
    virtual void UpdateData() override;

    CLONE(CTileMap)
public:
    CTileMap();
    ~CTileMap();
};

