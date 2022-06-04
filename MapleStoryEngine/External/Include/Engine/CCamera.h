#pragma once
#include "CComponent.h"


enum class PROJ_TYPE
{
    ORTHOGRAPHIC,
    PERSPECTIVE,
};


class CGameObject;

class CCamera :
    public CComponent
{
private:
    vector<CGameObject*>    m_vecForward;           // 불투명 물체
    vector<CGameObject*>    m_vecMasked;            // 투명, 불투명 물체
    vector<CGameObject*>    m_vecTranslucent;       // 반투명 물체
    vector<CGameObject*>    m_vecPostProcess;       // 후 처리 물체

    Matrix                  m_matView;
    Matrix                  m_matProj;

    PROJ_TYPE               m_eProjType;

    float                   m_fWidth;       // 투영 가로길이
    float                   m_fAspectRatio; // 종횡비 : (가로 / 세로)
    float                   m_fFOV;         // Filed Of View (시야각)
    float                   m_fFar;         // 최대 시야 거리

    UINT                    m_iLayerMask;
    int                     m_iCamIdx;  // RenderMgr 상에서   


public:
    void SetCameraAsMain();
    void SetCameraIndex(int _iIdx);
    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
    void SetAspectRatio(float _fAR) { m_fAspectRatio = _fAR; }
    void SetFOV(float _FOV) { m_fFOV = _FOV; }
    void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
    void SetFar(float _Far) { m_fFar = _Far; }

    float GetWidth() { return m_fWidth; }
    float GetAspectRatio() { return m_fAspectRatio; }
    float GetFOV() { return m_fFOV; }
    float GetFar() { return m_fFar; }

    PROJ_TYPE GetProjType() { return m_eProjType; }

    void CheckLayerMask(int _iLayerIdx);
    void CheckLayerMask(const wstring& _strLayerName);
    void CheckLayerMaskAll(){ m_iLayerMask = 0xffffffff; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }


private:
    void SortGameObject();  // Shader Domain 에 따른 물체 분류


public:    
    virtual void finalupdate() override;
    void render_forward();
    void render_masked();
    void render_translucent();
    void render_postprocess();

    CLONE(CCamera)

public:
    CCamera();
    CCamera(const CCamera& _origin);
    ~CCamera();

    friend class CRenderMgr;
};

