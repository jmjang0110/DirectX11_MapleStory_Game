#pragma once
#include "CComponent.h"


class CGameObject;

class CCamera :
    public CComponent
{
private:
    vector<CGameObject*> m_vecForward;  // 불투명 물체 
    vector<CGameObject*> m_vecMasked;   // 투명 , 불투명 물체
    vector<CGameObject*> m_vecOpaque;   // 반투명 물체 



    
    Matrix              m_matView;
    Matrix              m_matProj;


    UINT                m_iLayerMask;
    int                 m_iCamIdx; // RenderMgr 상에서 
public:
    void SetCameraAsMain();
    void CheckLayerMask(int _iLayerIdx);
    void CheckLayerMask(const wstring&& _strLayerName);
    void CheckLayerMaskAll() { m_iLayerMask = 0xffffffff; }


private:
    void SortGameObject(); // Shader Domain 에 따른 Object 분류 


public:    
    virtual void finalupdate() override;
    void render_forward();
    void render_masked();
    void render_opaque();

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }
    CLONE(CCamera)

public:
    CCamera();
    CCamera(const CCamera& _origin);
    ~CCamera();

    friend class CRenderMgr;

};

