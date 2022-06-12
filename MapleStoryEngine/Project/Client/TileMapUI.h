#pragma once
#include "ComponentUI.h"
#include <Engine/ptr.h>

/*
    ** imgFile - Tile Key **
    bsc - 내부
    enH0 - 상
    enH1 - 하
    enV0 - 좌
    enV1 - 우
    edU - 좌/우측 상단
    edD - 좌/우측 하단
    sILU - 좌측 경사 (계단)
    sIRU - 우측 경사 (계단)
    sILD - 좌측 경사 (계단 - Vertical Flip)
    sIRD - 우측 경사 (계단 - Vertical Flip)

*/
//  타일 정보 


class TreeUI;
class CTexture;
class TreeNode;
class CGameObject;



struct info
{
    int                 iImgIdx;    // 이미지인덱스 번호 
    Vec2                tileSize;   // px
};

// bsc, enH0 ...
struct tile
{
    struct TileImgFile*  _parent;
    wstring              Name;       // 타일 이미지 이름 
   
    vector<info>        vTilesInfo;
    int                 iTileNum;    // 타일 개수 
    Vec2                StartPos_px; // 아틀라스에서의 시작 위치
    Vec2                AllTileSize; // UV 값을 구하기 위해서 아틀라스에서의 
                                     // 해당 타일의 전체 사이즈를 구한다. 
};


// img 파일 
struct TileImgFile
{
    wstring                 Name;

    Ptr<CTexture>           pAtlasTex;  // 타일 이미지
    map<wstring, tile*>     imgFile;

};

class TileMapUI :
    public ComponentUI
{
private:
    TreeUI*                         m_TreeUI;       

    TileImgFile*                    m_pSelected_imgFile; // img File
    tile*                           m_pSelected_Tile;    

    CGameObject*                    m_pTargetObject;

    int                            m_iMapCountX;    // 맵 크기 
    int                            m_iMapCountY;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();

    // ========== 이렇게 해야 되나... ==================
    TreeNode* Push_darkpurpleToyCastle_toTree(TreeNode* _pDestNode);


    TreeNode* PushTileFiletoTree(wstring FileName, TileImgFile* pimgFile, TreeNode* _pDestNode);
    TreeNode* PushTileImagetoTree(tile* _pTile, TreeNode* _pDestNode);


public:
    // delegate 
    void TileClicked(DWORD_PTR _dw);
    int LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
    

public:
    TileMapUI();
    ~TileMapUI();


};

