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


public:
    TileImgFile() {};
    ~TileImgFile() {

        Safe_Del_Map(imgFile);
    }
};




class TileMapUI :
    public ComponentUI
{
private:
    // 메이플 전용 img
    TreeUI*                         m_TreeUI;       
    vector<TreeNode*>               m_vimgNode;

    TileImgFile*                    m_pSelected_imgFile; // img File
    string                          m_Selected_imgFIle_Name;
    Tile*                           m_pSelected_Tile;


    int                             m_iMapCountX;    // 맵 크기 
    int                             m_iMapCountY;

    Vec2                            m_vTileSize;


    // Button 
    bool                            m_bEditMode;
    bool                            m_bAutoSetNextTile;
    bool                            m_bUseMapleStory_ImgFile;
    bool                            m_bimgFIleChange;
    int                             m_bTilemapInnerClicked;


private:
    // 일반 Img 
    TileImgFile                     m_ImgFile; // 메이플 스토리 imgFile 을 쓰지 않을 때 쓰는 img 
    TilePackage*                    m_tilePackage;
    Tile*                           m_tile;
    bool                            m_bUseThisTile;

    int                             m_iRow;
    int                             m_iCol;

 
private:
    // 저장된 TileImgFile , Package 목록들을 저장해놓는다. - 삭제할 떄 쓰기 위해서
    // m_pTreeUI 를 지우면 TreeNode 들을 지우지만 Node 안에 있는 
    // 데이터들은 지우지 않는다. 
    // 그래서 직접 데이터들을 저장해놓고 지울것이다. 
    vector<TileImgFile*>            m_StoreimgFile;
    vector<TilePackage*>            m_StorePackage;

public:
    virtual void update() override;
    virtual void render_update() override;


    void EditorUpdate();
    void SelectAtlasTexture();
    void SelectTile();

public:
    void Reset();
    
    // ========== 이렇게 해야 되나... ==================
    TreeNode* FillimgFIleinfo(TreeNode* _pDestNode, TileImgFile* pimgFile);


    TreeNode* PushimgFiletoTree(wstring FileName, TreeNode* _pDestNode);                         //  img
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

