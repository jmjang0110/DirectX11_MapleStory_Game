#include "pch.h"
#include "GraphicsShaderUI.h"

#include <Engine/CGraphicsShader.h>
#include <Engine/CResMgr.h>


GraphicsShaderUI::GraphicsShaderUI()
	: ResInfoUI("GraphicsShader", RES_TYPE::GRAPHICS_SHADER)
{
}

GraphicsShaderUI::~GraphicsShaderUI()
{
}




void GraphicsShaderUI::update()
{
	// ToDo

	ResInfoUI::update();

}

void GraphicsShaderUI::render_update()
{
	ResInfoUI::render_update();

	CGraphicsShader* pGshader = dynamic_cast<CGraphicsShader*>(GetTargetRes());
	assert(pGshader);

	// GraphicsShader Key 
	string strKey = string(pGshader->GetKey().begin(), pGshader->GetKey().end());

	// GraphicsShader Name
	ImGui::Text("GraphcisShader Name ");
	ImGui::SameLine(150);
	ImGui::InputText("##GraphicsSahderName", (char*)strKey.c_str(), strKey.capacity(), ImGuiInputTextFlags_ReadOnly);

	// relative Path Name 
	string strRelativePath = string(pGshader->GetRelativePath().begin(), pGshader->GetRelativePath().end());
	ImGui::Text("RelativePath ");
	ImGui::SameLine(150);
	ImGui::InputText("##RelativePath", (char*)strRelativePath.c_str(), strRelativePath.capacity(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Separator();

	// Domain Name 
	SHADER_DOMAIN domain = pGshader->GetShaderDomain();
	string strName = ToString(domain);
	ImGui::Text("Shader Domain ");
	ImGui::SameLine(150);
	if (ImGui::BeginCombo("##domain", strName.c_str(), 0))
	{
		for (int i = 0; i < (int)SHADER_DOMAIN::END; ++i)
		{
			if (ImGui::Selectable(ToString((SHADER_DOMAIN)i)))
			{
				pGshader->SetShaderDomain((SHADER_DOMAIN)i);
			}
		}

		ImGui::EndCombo();
	}

	// RS_TYPE Name 
	RS_TYPE Rasterizer = pGshader->GetRSType();
	strName = ToString(Rasterizer);
	ImGui::Text("Rasterizer State");
	ImGui::SameLine(150);
	if (ImGui::BeginCombo("##RasterizerState", strName.c_str(), 0))
	{
		for (int i = 0; i < (int)RS_TYPE::END; ++i)
		{
			if (ImGui::Selectable(ToString((RS_TYPE)i)))
			{
				pGshader->SetRSType((RS_TYPE)i);
			}
		}

		ImGui::EndCombo();
	}



	// DS_TYPE Name 
	DS_TYPE DepthStencil = pGshader->GetDSType();
	 strName = ToString(DepthStencil);
	ImGui::Text("DepthStencil State");
	ImGui::SameLine(150);
	if (ImGui::BeginCombo("##DepthStencilState", strName.c_str(), 0))
	{
		for (int i = 0; i < (int)DS_TYPE::END; ++i)
		{
			if (ImGui::Selectable(ToString((DS_TYPE)i)))
			{
				pGshader->SetDSType((DS_TYPE)i);
			}
		}

		ImGui::EndCombo();
	}

	// BS_TYPE Name 
	BS_TYPE BlendState = pGshader->GetBSType();
	 strName = ToString(BlendState);
	ImGui::Text("Blend State");
	ImGui::SameLine(150);
	if (ImGui::BeginCombo("##BlendState", strName.c_str(), 0))
	{
		for (int i = 0; i < (int)BS_TYPE::END; ++i)
		{
			if (ImGui::Selectable(ToString((BS_TYPE)i)))
			{
				pGshader->SetBSType((BS_TYPE)i);
			}
		}

		ImGui::EndCombo();
	}

	// PIpeline Stage
	ImGui::Separator();
	showPipelineStage(pGshader);
	ImGui::Separator();


	D3D11_PRIMITIVE_TOPOLOGY topology = pGshader->GetTopology();


}


void GraphicsShaderUI::showPipelineStage(CGraphicsShader* _pShader)
{ 
	PIPELINE_STAGE GSpipeline = _pShader->GetPipelineStage();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::PushItemWidth(30.f);
	// Vertex Shader 
	if (GSpipeline & PIPELINE_STAGE::VS)
	{
		ImGui::Text("PIPELINE_STAGE::VS");
		ImGui::SameLine(150);
		
		ImGui::InputText("##PIPELINE_STAGE::VS", (char*)"O", sizeof("O"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 255, 255));

	}
	else
	{
		ImGui::Text("PIPELINE_STAGE::VS");
		ImGui::SameLine(150);
		ImGui::InputText("##PIPELINE_STAGE::VS", (char*)"X", sizeof("X"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));
	}

	// Hull Sahder 
	if (GSpipeline & PIPELINE_STAGE::HS)
	{
		ImGui::Text("PIPELINE_STAGE::HS");
		ImGui::SameLine(150);
		ImGui::InputText("##PIPELINE_STAGE::HS", (char*)"O", sizeof("O"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 255, 255));


	}
	else
	{
		ImGui::Text("PIPELINE_STAGE::HS");
		ImGui::SameLine(150);
		ImGui::InputText("##PIPELINE_STAGE::HS", (char*)"X", sizeof("X"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));
	}


	// Domain Shader 
	if (GSpipeline & PIPELINE_STAGE::DS)
	{
		ImGui::Text("PIPELINE_STAGE::DS");
		ImGui::SameLine(150);
		ImGui::InputText("##PIPELINE_STAGE::DS", (char*)"O", sizeof("O"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 255, 255));


	}
	else
	{
		ImGui::Text("PIPELINE_STAGE::DS");
		ImGui::SameLine(150);
		ImGui::InputText("##PIPELINE_STAGE::DS", (char*)"X", sizeof("X"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));
	}


	// Geometry Shader 
	if (GSpipeline & PIPELINE_STAGE::GS)
	{
		ImGui::Text("PIPELINE_STAGE::GS");
		ImGui::SameLine(150);
		ImGui::InputText("##PIPELINE_STAGE::GS", (char*)"O", sizeof("O"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 255, 255));


	}
	else
	{
		ImGui::Text("PIPELINE_STAGE::GS");
		ImGui::SameLine(150);
		ImGui::InputText("##PIPELINE_STAGE::GS", (char*)"X", sizeof("X"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));
	}

	// Pixel Sahder 
	if (GSpipeline & PIPELINE_STAGE::PS)
	{
		ImGui::Text("PIPELINE_STAGE::PS");
		ImGui::SameLine(150);
		ImGui::InputText("##PIPELINE_STAGE::PS", (char*)"O", sizeof("O"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 255, 255));


	}
	else
	{
		ImGui::Text("PIPELINE_STAGE::PS");
		ImGui::SameLine(150);
		ImGui::InputText("##PIPELINE_STAGE::PS", (char*)"X", sizeof("X"), ImGuiInputTextFlags_ReadOnly);
		draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));
	}
	ImGui::PopItemWidth();

}

