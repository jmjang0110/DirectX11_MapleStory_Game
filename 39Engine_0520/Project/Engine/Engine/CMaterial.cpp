#include "pch.h"
#include "CMaterial.h"

#include "CGraphicsShader.h"
#include "CDevice.h"
#include "CConstBuffer.h"


CMaterial::CMaterial()
	: m_pShader(nullptr)
	, m_arrTex{}
{
}

CMaterial::~CMaterial()
{
}


void CMaterial::UpdateData()
{
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		if (nullptr != m_arrTex[i])
		{
			m_arrTex[i]->UpdateData((int)PIPELINE_STAGE::ALL, i);
			m_Param.bTex[i] = 1;
		}
		else
		{		
			CTexture::Clear(i);
			m_Param.bTex[i] = 0;
		}
	}


	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::SCALAR_PARAM);
	pCB->SetData(&m_Param, sizeof(tScalarParam));
	pCB->UpdateData();


	if (nullptr != m_pShader)
	{
		m_pShader->UpdateData();
	}
}

void CMaterial::SetShader(Ptr<CGraphicsShader> _pShader)
{
	m_pShader = _pShader;

	if (nullptr != m_pShader)
	{
		m_vecScalarParamInfo = m_pShader->GetScalarParamInfo();
		m_vecTexParamInfo = m_pShader->GetTexParamInfo();
	}
}

void CMaterial::SetScalarParam(SCALAR_PARAM _eType, void* _pData)
{
	switch (_eType)
	{
	case SCALAR_PARAM::INT_0:		
	case SCALAR_PARAM::INT_1:
	case SCALAR_PARAM::INT_2:
	case SCALAR_PARAM::INT_3:		
		m_Param.iArr[(UINT)_eType - (UINT)SCALAR_PARAM::INT_0] = *((int*)_pData);
		break;
	case SCALAR_PARAM::FLOAT_0:
	case SCALAR_PARAM::FLOAT_1:
	case SCALAR_PARAM::FLOAT_2:
	case SCALAR_PARAM::FLOAT_3:
		m_Param.fArr[(UINT)_eType - (UINT)SCALAR_PARAM::FLOAT_0] = *((float*)_pData);
		break;
	case SCALAR_PARAM::VEC2_0:
	case SCALAR_PARAM::VEC2_1:
	case SCALAR_PARAM::VEC2_2:
	case SCALAR_PARAM::VEC2_3:
		m_Param.v2Arr[(UINT)_eType - (UINT)SCALAR_PARAM::VEC2_0] = *((Vec2*)_pData);
		break;
	case SCALAR_PARAM::VEC4_0:
	case SCALAR_PARAM::VEC4_1:
	case SCALAR_PARAM::VEC4_2:
	case SCALAR_PARAM::VEC4_3:
		m_Param.v4Arr[(UINT)_eType - (UINT)SCALAR_PARAM::VEC4_0] = *((Vec4*)_pData);
		break;
	case SCALAR_PARAM::MAT_0:
	case SCALAR_PARAM::MAT_1:
	case SCALAR_PARAM::MAT_2:
	case SCALAR_PARAM::MAT_3:
		m_Param.matArr[(UINT)_eType - (UINT)SCALAR_PARAM::MAT_0] = *((Matrix*)_pData);
		break;
	}
}

void CMaterial::SetScalarParam(const wstring& _strParamName, void* _pData)
{
	for (size_t i = 0; i < m_vecScalarParamInfo.size(); ++i)
	{
		if (m_vecScalarParamInfo[i].strDesc == _strParamName)
		{
			SetScalarParam(m_vecScalarParamInfo[i].eScalarParam, _pData);
			break;
		}
	}
}


void CMaterial::SetTexParam(TEX_PARAM _eType, Ptr<CTexture> _pTex)
{
	switch (_eType)
	{
	case TEX_PARAM::TEX_0:
	case TEX_PARAM::TEX_1:
	case TEX_PARAM::TEX_2:
	case TEX_PARAM::TEX_3:
	case TEX_PARAM::TEX_4:
	case TEX_PARAM::TEX_5:
	case TEX_PARAM::TEX_CUBE_0:
	case TEX_PARAM::TEX_CUBE_1:
	case TEX_PARAM::TEX_ARR_0:
	case TEX_PARAM::TEX_ARR_1:
		m_arrTex[(UINT)_eType] = _pTex;
		break;
	}
}

void CMaterial::SetTexParam(const wstring& _strParamName, Ptr<CTexture> _pTex)
{
	for (size_t i = 0; i < m_vecTexParamInfo.size(); ++i)
	{
		if (m_vecTexParamInfo[i].strDesc == _strParamName)
		{
			SetTexParam(m_vecTexParamInfo[i].eTexParam, _pTex);
			break;
		}
	}
}

