#pragma once
#include "singleton.h"

class CCamera;


class CRenderMgr 
    : public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);


private:
    vector<CCamera*>  m_vecCam;
    CCamera* m_pEditorCam;


public:
    void init();
    void update();
    void render();
public:
    void RegisterCamera(CCamera* _pCam);
    void ClearCamera() { m_vecCam.clear(); }
    void SwapCameraIndex(CCamera* _pCam, int _iChangeIdx);


};

