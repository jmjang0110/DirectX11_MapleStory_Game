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
class TreeUI;
class CTexture;
class TreeNode;
class CGameObject;

// ============ 정보 ============
struct TileInfo
{
    TileInfo(int _imgidx = -1) { iImgIdx = _imgidx; }
    int                 iImgIdx;            // 이미지인덱스 번호 
};


// ============ 타일 ============
struct Tile
{
    struct TilePackage*     _parent;
    wstring                 Name;            // 타일 이미지 이름 
   
    int                     iImgIdxNum;      // 완성된 타일을 그리는데 필요한 이미지인덱스 개수 
    int                     iRow;            // 행으로 몇개 타일 
    int                     iCol;            // 열로 몇개 타일 

    vector<TileInfo>        vTilesInfo;

};

// ============ 패키지 ==========
struct TilePackage
{
    struct TileImgFile* _parent;

    wstring name;                           // 패키지 이름 
    int num;                                // 패키지가 갖고 있는 타일 개수 
};

// ============ img ============
// img 파일 
struct TileImgFile
{
    wstring                 Name;

    Ptr<CTexture>           pAtlasTex;      // 타일 이미지
    map<wstring, Tile*>     imgFile;

};




class TileMapUI :
    public ComponentUI
{
private:
    TreeUI*                         m_TreeUI;       

    TileImgFile*                    m_pSelected_imgFile; // img File
    Tile*                           m_pSelected_Tile;

    CGameObject*                    m_pTargetObject;

    int                             m_iMapCountX;    // 맵 크기 
    int                             m_iMapCountY;

    bool                            m_bEditMode;
    bool                            m_bShowGrid;
    bool                            m_bAutoSetNextTile;

    int                             m_bTilemapInnerClicked;


    string                          m_Selected_imgFIle_Name;
    vector<TreeNode*>               m_vimgNode;
    bool                            m_bimgFIleChange;


public:
    virtual void update() override;
    virtual void render_update() override;
    void EditorUpdate();

public:
    void Reset();
    
    // ========== 이렇게 해야 되나... ==================
    TreeNode* FillimgFIleinfo(TreeNode* _pDestNode, TileImgFile* pimgFile);


    TreeNode* PushimgFiletoTree(wstring FileName, TreeNode* _pDestNode); // img
    TreeNode* PushPackageFiletoTree(TilePackage* pPack, TreeNode* _pDestNode);                   //  pack
    TreeNode* PushTiletoTree(Tile* _pTile, TreeNode* _pDestNode);                                //  Tile
     
    void CreateNewTilesInfo(TreeNode* _pDestNode, TilePackage* _pPackage, TileImgFile* _pimgFile);                       //  info 


private:
    Tile* FillTileInfo(int _TileID_Number,wstring PackageName, TilePackage* _pPackage
        , int _startImgIdx, int _iAllImgIdxNum, int _row, int _col);
    void ResetimgFile(string _imgFileName, TreeNode* _imgFIleNode);

public:
    // delegate 
    void TileClicked(DWORD_PTR _dw);
  

public:
    TileMapUI();
    ~TileMapUI();


};

