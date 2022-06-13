#pragma once
#include "CRenderComponent.h"

#include "CTexture.h"
#include "CStructuredBuffer.h"

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>       m_pAtlasTex;
    Vec2                m_vSlicePixel;
    Vec2                m_vSliceUV;  

    UINT                m_iRowCount;
    UINT                m_iColCount;   

    UINT                m_iTileCountX;
    UINT                m_iTileCountY;
    vector<tTileData>   m_vecTileData;
    CStructuredBuffer*  m_pBuffer;

    bool                m_bBufferUpdated;


private:
    // Todo 
    int m_iShowGrid; // 0 : No 1 : Yes 

public:
    // 0 : No 1 : Yes
    void SetShowGrid(int _grid) { m_iShowGrid = _grid; }


public:
    void SetAtlasTex(Ptr<CTexture> _pAtlasTex) 
    { 
        m_pAtlasTex = _pAtlasTex; 
        m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileSize(Vec2 _vPixelSize) 
    {  
        m_vSlicePixel = _vPixelSize; 
        if(nullptr != m_pAtlasTex)
            m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileMapCount(UINT _iCountX, UINT _iCountY);
    void SetTileData(int _iTileIdx, int _iImgIdx);
    void ClearTileData();

    // =======Todo ===================
public:
    Ptr<CTexture> GetAtlasTex() { return m_pAtlasTex; }
    Vec2 GetTileSize() { return m_vSlicePixel; }
    UINT GetTileCountX() { return m_iTileCountX; }
    UINT GetTileCountY() { return m_iTileCountY; }
    tTileData GetTileData(UINT _tileIdx) { return m_vecTileData[_tileIdx]; }

    // for MapleStory 
    // - 수동으로 UV 값을 조정 하고 m_pAtlasTex->width 이런게 아니라 
    // 내가 지정한 tile 의 전체 크기로 세팅되게 
    void SetTileSize(Vec2 _vPixelSize, Vec2 _AllTileSize);
    void SetTileData(int _iTileIdx, int _iImgIdx, Vec2 _AllTileSize, Vec2 StartPos_px);


    // ===============================

public:
    virtual void finalupdate() override;
    virtual void render() override;
    virtual void UpdateData() override;

    CLONE(CTileMap)
public:
    CTileMap();
    CTileMap(const CTileMap& _origin);
    ~CTileMap();
};

