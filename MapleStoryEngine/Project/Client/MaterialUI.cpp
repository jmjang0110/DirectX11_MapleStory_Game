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
	// To Do


	ResInfoUI::update();
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
	if (ImGui::BeginCombo("##ShaderName", strShaderName.c_str(), 0))
	{
			// ListUI 활성화한다.
			const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::GRAPHICS_SHADER);
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->Clear();
			pListUI->SetTitle("Shader List");

			for (const auto& pair : mapRes)
			{
				pListUI->AddList(string(pair.first.begin(), pair.first.end()));
			}

			pListUI->Activate();
			pListUI->SetDBCEvent(this, (DBCLKED)&MaterialUI::ShaderSelect);

		ImGui::EndCombo();
	}

	// Shader Parameter 확인
	if (nullptr == pShader)
		return;

	const vector<tScalarParamInfo>& vecScalarInfo = pShader->GetScalarParamInfo();
	for (size_t i = 0; i < vecScalarInfo.size(); ++i)
	{
		string strDesc = string(vecScalarInfo[i].strDesc.begin(), vecScalarInfo[i].strDesc.end());

		void* pData = pMtrl->GetScalarParam(vecScalarInfo[i].eScalarParam);

		switch (vecScalarInfo[i].eScalarParam)
		{
		case SCALAR_PARAM::INT_0:
		case SCALAR_PARAM::INT_1:
		case SCALAR_PARAM::INT_2:
		case SCALAR_PARAM::INT_3:
			ParamUI::Param_Int(strDesc, (int*)pData);
			break;
		case SCALAR_PARAM::FLOAT_0:
		case SCALAR_PARAM::FLOAT_1:
		case SCALAR_PARAM::FLOAT_2:
		case SCALAR_PARAM::FLOAT_3:
			ParamUI::Param_Float(strDesc, (float*)pData);
			break;
		case SCALAR_PARAM::VEC2_0:
		case SCALAR_PARAM::VEC2_1:
		case SCALAR_PARAM::VEC2_2:
		case SCALAR_PARAM::VEC2_3:
			ParamUI::Param_Vec2(strDesc, (Vec2*)pData);
			break;
		case SCALAR_PARAM::VEC4_0:
		case SCALAR_PARAM::VEC4_1:
		case SCALAR_PARAM::VEC4_2:
		case SCALAR_PARAM::VEC4_3:
			ParamUI::Param_Vec4(strDesc, (Vec4*)pData);
			break;
		}
	}

	// ParamUI::Param_Tex =>> 이미지 띄운다. 
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
			CTexture* pTex = ParamUI::Param_Tex(strDesc, pMtrl->GetTexParam(vecTexParamInfo[i].eTexParam).Get());
			pMtrl->SetTexParam(vecTexParamInfo[i].eTexParam, pTex);
			break;
		}
	}

	// Mtrl 의 Outpute Tex 변경 
	ImGui::SameLine();
	if (ImGui::Button("Select\n Tex"))
	{
		// ListUI 활성화한다.
		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Texture List");

		for (const auto& pair : mapRes)
		{
			pListUI->AddList(string(pair.first.begin(), pair.first.end()));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&TextureUI::TextureSelect_toMtrl);
	}




}



void MaterialUI::ShaderSelect(DWORD_PTR _param)
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
