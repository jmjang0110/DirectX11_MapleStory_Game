#include "pch.h"
#include "MaterialUI.h"


#include "ParamUI.h"
#include "ListUI.h"
#include "CImGuiMgr.h"

#include "MeshRenderUI.h"
#include "TextureUI.h"
#include "InspectorUI.h"



#include <Engine/CMaterial.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CResMgr.h>

MaterialUI::MaterialUI()
	: ResInfoUI("Material", RES_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::update()
{
	ResInfoUI::update();
	// To Do



}
void MaterialUI::render_update()
{
	ResInfoUI::render_update();

	// To Do
	CMaterial* pMtrl = dynamic_cast<CMaterial*>(GetTargetRes());
	assert(pMtrl);

	// Material Key
	string strName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());

	// Material Name
	ImGui::Text("Material");
	ImGui::SameLine(100);
	ImGui::InputText("##MaterialName", (char*)strName.c_str(), strName.capacity(), ImGuiInputTextFlags_ReadOnly);

	// Shader Name
	CGraphicsShader* pShader = pMtrl->GetShader().Get();

	string strShaderName;
	if (nullptr != pShader)
		strShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());

	ImGui::Text("Shader");
	ImGui::SameLine(100);
	ImGui::InputText("##ShaderName", (char*)strShaderName.c_str(), strShaderName.capacity(), ImGuiInputTextFlags_ReadOnly);

	// Shader Parameter 확인
	if (nullptr == pShader)
		return;

	const vector<tScalarParamInfo>& vecScalarInfo = pShader->GetScalarParamInfo();
	for (size_t i = 0; i < vecScalarInfo.size(); ++i)
	{
		string strDesc = string(vecScalarInfo[i].strDesc.begin(), vecScalarInfo[i].strDesc.end());

		const void* pData = pMtrl->GetScalarParam(vecScalarInfo[i].eScalarParam);

		switch (vecScalarInfo[i].eScalarParam)
		{
		case SCALAR_PARAM::INT_0:
		case SCALAR_PARAM::INT_1:
		case SCALAR_PARAM::INT_2:
		case SCALAR_PARAM::INT_3:
		{
			int data = ParamUI::Param_Int(strDesc, (const int*)pData);
			if (*(const int*)pData != data)
			{
				pMtrl->SetScalarParam(vecScalarInfo[i].eScalarParam, &data);
			}
		}
		break;
		case SCALAR_PARAM::FLOAT_0:
		case SCALAR_PARAM::FLOAT_1:
		case SCALAR_PARAM::FLOAT_2:
		case SCALAR_PARAM::FLOAT_3:
		{
			float data = ParamUI::Param_Float(strDesc, (const float*)pData);
			if (*(const float*)pData != data)
			{
				pMtrl->SetScalarParam(vecScalarInfo[i].eScalarParam, &data);
			}
		}
		break;
		case SCALAR_PARAM::VEC2_0:
		case SCALAR_PARAM::VEC2_1:
		case SCALAR_PARAM::VEC2_2:
		case SCALAR_PARAM::VEC2_3:
		{
			Vec2 data = ParamUI::Param_Vec2(strDesc, (const Vec2*)pData);
			if (*(const Vec2*)pData != data)
			{
				pMtrl->SetScalarParam(vecScalarInfo[i].eScalarParam, &data);
			}
		}

		break;
		case SCALAR_PARAM::VEC4_0:
		case SCALAR_PARAM::VEC4_1:
		case SCALAR_PARAM::VEC4_2:
		case SCALAR_PARAM::VEC4_3:
		{
			Vec4 data = ParamUI::Param_Vec4(strDesc, (const Vec4*)pData);
			if (*(const Vec4*)pData != data)
			{
				pMtrl->SetScalarParam(vecScalarInfo[i].eScalarParam, &data);
			}
		}

		break;
		}
	}

	const vector<tTexParamInfo>& vecTexParamInfo = pShader->GetTexParamInfo();

	for (size_t i = 0; i < vecTexParamInfo.size(); ++i)
	{
		string strDesc = string(vecTexParamInfo[i].strDesc.begin(), vecTexParamInfo[i].strDesc.end());

		switch (vecTexParamInfo[i].eTexParam)
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
			if (ParamUI::Param_Tex(strDesc, pMtrl->GetTexParam(vecTexParamInfo[i].eTexParam).Get()
				, this, (DBCLKED)&MaterialUI::TextureSelected))
			{
				m_eSelectedTexParam = vecTexParamInfo[i].eTexParam;
			}
			break;
		}
	}
}

// Delegate 용
void MaterialUI::ShaderSelected(DWORD_PTR _param)
{

	string strSelectedName = (char*)_param;
	wstring strTexKey = wstring(strSelectedName.begin(), strSelectedName.end());

	Ptr<CGraphicsShader> pGShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(strTexKey);
	assert(pGShader.Get());

	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	CRes* pTargetRes = pInspectorUI->GetTargetRes();

	assert(!(nullptr == pTargetRes));

	if (RES_TYPE::MATERIAL == pTargetRes->GetResType())
	{
		CMaterial* pMtrl = (CMaterial*)pTargetRes;
		pMtrl->SetShader(pGShader);

	}
}

// Delegate 용
void MaterialUI::TextureSelected(DWORD_PTR _ptr)
{
	string str = (char*)_ptr;
	wstring strKey = wstring(str.begin(), str.end());

	CTexture* pSelectedTex = CResMgr::GetInst()->FindRes<CTexture>(strKey).Get();

	CMaterial* pMtrl = dynamic_cast<CMaterial*>(GetTargetRes());
	assert(pMtrl);

	// 변경점이 있을 때만 세팅
	if (pMtrl->GetTexParam(m_eSelectedTexParam) != pSelectedTex)
		pMtrl->SetTexParam(m_eSelectedTexParam, pSelectedTex);
}
