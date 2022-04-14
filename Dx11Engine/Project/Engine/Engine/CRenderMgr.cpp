#include "pch.h"
#include "CRenderMgr.h"


#include "CCamera.h"
#include "CDevice.h"

CRenderMgr::CRenderMgr()
	:m_pEditorCam(nullptr)
{

}

CRenderMgr::~CRenderMgr()
{


}

void CRenderMgr::init()
{


}

void CRenderMgr::update()
{


}

void CRenderMgr::render()
{
	if (m_vecCam.empty())
		return;

	CDevice::GetInst()->ClearTarget();

	// Main Camera 시점으로 Render
	// [ 0 ] Camera : Main Camera  
	CCamera* pMainCam = m_vecCam[0];
	
	// Camera 가 찍는 Layer 의 오브젝트들을 Shader Domain 에 따라 분류홰둠 
	pMainCam->SortGameObject();

	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();

	// Forward 물체 렌더링 
	pMainCam->render_forward();

	// Masked 물체 렌더링 
	pMainCam->render_masked();
	// Alpha 물체 렌더링 
	pMainCam->render_opaque();


	// Sub Camera 시점으로 Render 
	for (int i = 1; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i];

	}



	CDevice::GetInst()->Present();

}

void CRenderMgr::RegisterCamera(CCamera* _pCam)
{
	// 카메라가 RenderMgr 에 최초 등록 되는 경우 
	if (-1 == _pCam->m_iCamIdx)
	{
		m_vecCam.push_back(_pCam);
		int iIdx = m_vecCam.size() - 1;
		_pCam->m_iCamIdx = iIdx;
	}
	else
	{
		if (m_vecCam.size() <= _pCam->m_iCamIdx)
		{
			m_vecCam.resize((size_t)_pCam->m_iCamIdx + 1);;
		}
		
		m_vecCam[_pCam->m_iCamIdx] = _pCam;

	}
		
}

void CRenderMgr::SwapCameraIndex(CCamera* _pCam, int _iChangeIdx)
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		// 등록된 카메라를 찾았다면 
		if (_pCam == m_vecCam[i])
		{
			if (nullptr != m_vecCam[_iChangeIdx])
			{
				m_vecCam[_iChangeIdx]->m_iCamIdx = i;
				_pCam->m_iCamIdx = _iChangeIdx;

				return;

			}
		}
	}

	assert(nullptr);


}
