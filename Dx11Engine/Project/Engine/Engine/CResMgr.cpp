#include "pch.h"
#include "CResMgr.h"

CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Del_Map(m_Res[i]);
	}
}


Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _format, UINT _flag, bool _bEngineRes)
{
	/*
// 주의 사항
D3D11_BIND_UNORDERED_ACCESS
D3D11_BIND_SHADER_RESOURCE
조합은 가능하지만 여기에 추가해서

D3D11_BIND_DEPTH_STENCIL 은 불가능하다 .
Depth Stencil 바인딩은 따로 (유일하게)설정해야한다.

*/


	assert(nullptr == FindRes<CTexture>(_strKey));


	CTexture* pTexture = new CTexture;

	pTexture->Create(_iWidth, _iHeight, _format, _flag);
	pTexture->SetKey(_strKey);
	pTexture->m_bEngineRes = _bEngineRes;

	AddRes<CTexture>(_strKey, pTexture);

	return pTexture;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _pTex2D, bool _bEngineRes)
{
	assert(nullptr == FindRes<CTexture>(_strKey));


	CTexture* pTexture = new CTexture;

	pTexture->Create(_pTex2D);
	pTexture->SetKey(_strKey);
	pTexture->m_bEngineRes = _bEngineRes;

	AddRes<CTexture>(_strKey, pTexture);

	return pTexture;
}
