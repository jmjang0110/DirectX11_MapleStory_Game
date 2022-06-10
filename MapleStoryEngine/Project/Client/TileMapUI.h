#pragma once
#include "ComponentUI.h"
#include <Engine/ptr.h>

class TreeUI;
class CTexture;

// 타일을 불러 올 수 있도록;; 
struct TileInfo
{
    Ptr<CTexture>       pAtlasTex;
    tTileData           tileData;

    Vec2                tileSize;

};

class TileMapUI :
    public ComponentUI
{
private:
    TreeUI* m_TreeUI; // 고를 타일맵의 Atlas 정보를 띄운다.

    UINT                m_iTileCountX;
    UINT                m_iTileCountY;


public:
    virtual void update() override;
    virtual void render_update() override;


public:
    TileMapUI();
    ~TileMapUI();


};

