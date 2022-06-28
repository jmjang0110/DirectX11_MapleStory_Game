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

void CResMgr::SaveChangedRes()
{
	wstring strContent = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		map<wstring, CRes*>::iterator iter = m_Res[i].begin();
		for (; iter != m_Res[i].end(); ++iter)
		{
			if (iter->second->IsChanged())
			{
				iter->second->Save(strContent + iter->second->GetRelativePath());
			}
		}
	}
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _format, UINT _flag, bool _bEngineRes)
{
	assert(nullptr == FindRes<CTexture>(_strKey));
	

	CTexture* pTexture = new CTexture;

	pTexture->Create(_iWidth, _iHeight, _format, _flag);
	pTexture->SetKey(_strKey);
	pTexture->m_bEngineRes = _bEngineRes;

	AddRes<CTexture>(_strKey, pTexture, _bEngineRes);

	return pTexture;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _pTex2D, bool _bEngineRes)
{
	assert(nullptr == FindRes<CTexture>(_strKey));


	CTexture* pTexture = new CTexture;

	pTexture->Create(_pTex2D);
	pTexture->SetKey(_strKey);
	pTexture->m_bEngineRes = _bEngineRes;

	AddRes<CTexture>(_strKey, pTexture, _bEngineRes);

	return pTexture;
}


void CResMgr::DeleteRes(const wstring& _strKey)
{
	map<wstring, CRes*>::iterator iter;
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		iter = m_Res[i].find(_strKey);
		if (iter != m_Res[i].end())
		{
			SAFE_DELETE(iter->second);
			m_Res[i].erase(iter);
			return;
		}
	}

	assert(nullptr);
}
