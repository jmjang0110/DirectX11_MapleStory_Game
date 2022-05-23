#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CResMgr.h"

void CRenderMgr::init()
{
	// PostProcess Texture ¸¸µé±â
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	Ptr<CTexture> pPostProcessTex = CResMgr::GetInst()->CreateTexture(L"PostProcessTex"
		, (UINT)vResolution.x, (UINT)vResolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE, true);
}
