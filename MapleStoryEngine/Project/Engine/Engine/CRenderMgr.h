#pragma once
#include "singleton.h"

class CCamera;
class CLight2D;


class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    // Camera 
    vector<CCamera*>    m_vecCam;       // Scene 에 있는 Camera 들
    CCamera*            m_pEditorCam;   // Editor 시점 카메라


    // Light2D 
    vector<CLight2D*>   m_vecLight2D;
    CStructuredBuffer* m_pLight2DBuffer;    // Scene 의 2D 광원 버퍼



public:
    void init();
    void update();
    void render();

private:
    void render_play();
    void render_editor();
public:
    void RegisterCamera(CCamera* _pCam);
    void RegisterEditorCamera(CCamera* _pCam) { m_pEditorCam = _pCam; }
    void ClearCamera() { m_vecCam.clear(); }
    void SwapCameraIndex(CCamera* _pCam, int _iChangeIdx);
    void CopyTargetToPostProcess();

    int RegisterLight2D(CLight2D* _pLight2D)
    {
        m_vecLight2D.push_back(_pLight2D);
        return (int)m_vecLight2D.size() - 1;
    }
private:
    void UpdateLight2D();





};

