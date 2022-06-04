#pragma once
#include "ComponentUI.h"
#include <Engine/Ptr.h>

class CTexture;
class CAnimation2D;


enum class ANIMATOR_MODE
{
    CREATE, // Create New Frame Mode 
    MODIFY, // Modify Selected Frame Mode 
    END,

};

class Animator2DUI :
    public ComponentUI
{
private:
    ANIMATOR_MODE           m_CurMode;

    vector<string>          m_vecTexList;   // CRes 저장된 texture string 정보
    vector<string>          m_vecAnimList;  // TargetObject의 Animation 정보 


    Ptr<CTexture>           m_pChoosenAtlas; // 현재 선택된 Texture Atlas
    
private:
    // CRATE MODE 
    // New Anim Info
    CAnimation2D*           m_pNewAnim2D;
    vector<tAnim2DFrame>	m_NewAnim_vecFrm;               // 새로 만든 텍스처 저장 

    wstring                 m_NewAnim_strName;              // 애니메이션 이름 
    tAnim2DFrame            m_NewFrameInfo;                 // 애니메이션 정보 

    Vec2                    m_NewAnim_vBackLT;              // 백그라운드 시작위치
    Vec2                    m_NewAnim_vBackSize;            // 백그라운드 사이즈    
  
    Vec2                    m_TextureStartPos;              // Window상 Texture 시작 위치 
    Vec2                    m_TexSize;                      // Texture 사이즈 

    int                     m_FrmCnt;
    int                     m_iDemoFrameIdx;

private:
    // MODIFY MODE 
    int                     m_ModifyFrameIdx; 
    bool                    m_bModifyBtn;

    // swap
    Vec2                    m_SwapIdx;
    bool                    m_bSwapBtn;

    //Flip
    int                         m_iHorizontal_Flip;     // 1 : yes 0 : No
    int                         m_iVertical_Flip;       // 1 : yes 0 : No

public:
    virtual void update() override;
    virtual void render_update() override;


public:
    
    void TextureSelect(DWORD_PTR _param);
    void render_update_CreateAnim2DTool();
    void UpdateTool();


private:
    // main Func 
    void SelectAtlasTexture();  
    void ShowAtlasTexTool();    
    void ShowFrameTool();           

private:
    // Sub Func
    void DrawFrmaes();                          // Vector 에 저장된 Frame들 출력 Green box
    void DrawFrameRect();                       // Yellow / BackGround

    void DrawSettingBtn();                      // 새로 만들 프레임 정보 조정버튼 
    void DrawShowNewFrame(ImTextureID _texid);  // 만들고 있는 프레임 실시간 출력 
    void DrawPlayAnimation(ImTextureID _texid); // 저장된 프레임으로 애니메이션 비디오 출력 

    void DrawAllNewFrames(ImTextureID _texid);  // 저장된 프레임 각각을 출력 
    void DrawDecisionFrameBtn();                // Add / Delete Frame 버튼 

public:
    Animator2DUI();
    ~Animator2DUI();
};

