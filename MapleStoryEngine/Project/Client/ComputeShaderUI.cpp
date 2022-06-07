#include "pch.h"
#include "ComputeShaderUI.h"

#include <Engine/CComputeShader.h>
#include <Engine/CParticleUpdateShader.h>
#include <Engine/CTestShader.h>
#include <Engine/CStructuredBuffer.h>
#include "ParamUI.h"




ComputeShaderUI::ComputeShaderUI()
	: ResInfoUI("ComputeShader", RES_TYPE::COMPUTE_SHADER)
{
}

ComputeShaderUI::~ComputeShaderUI()
{
}

void ComputeShaderUI::update()
{
	ResInfoUI::render_update();

	
}

void ComputeShaderUI::render_update()
{
	ResInfoUI::render_update();

	CComputeShader* pComputeShader = dynamic_cast<CComputeShader*>(GetTargetRes());
	assert(pComputeShader);

	// ParticleShader Key Name 
	string strKey = string(pComputeShader->GetKey().begin(), pComputeShader->GetKey().end());

	// ParticleShader NAme 
	ImGui::Text("Shader Name ");
	ImGui::SameLine(150);
	ImGui::InputText("##ShaderName", (char*)strKey.c_str(), strKey.capacity(), ImGuiInputTextFlags_ReadOnly);

	// relative Path Name 
	string strRelativePath = string(pComputeShader->GetRelativePath().begin(), pComputeShader->GetRelativePath().end());
	ImGui::Text("RelativePath ");
	ImGui::SameLine(150);
	ImGui::InputText("##RelativePath", (char*)strRelativePath.c_str(), strRelativePath.capacity(), ImGuiInputTextFlags_ReadOnly);


	ImGui::Separator();

	Vec3 GroupCnt = pComputeShader->GetGroupCount();
	Vec3 GroupPerThreadCnt = pComputeShader->GetGroupPerThreadCount();

	int group[3] = { (UINT)GroupCnt.x, (UINT)GroupCnt.y ,(UINT)GroupCnt.z };

	ImGui::Text("Group Count");
	ImGui::SameLine(150);
	ImGui::InputInt3("##GroupCnt", group, ImGuiInputTextFlags_ReadOnly);


	int groupPer[3] = { (UINT)GroupPerThreadCnt.x, (UINT)GroupPerThreadCnt.y ,(UINT)GroupPerThreadCnt.z };
	ImGui::Text("GroupPerThread Count");
	ImGui::SameLine(150);
	ImGui::InputInt3("##GroupPerThreadCnt", groupPer, ImGuiInputTextFlags_ReadOnly);


	// CParticleUpdateShader
	ShowCParticleUpdateShader(pComputeShader);
	ShowTestShader(pComputeShader);





}

void ComputeShaderUI::ShowCParticleUpdateShader(CComputeShader* _pComputeShader)
{
	if (nullptr != dynamic_cast<CParticleUpdateShader*>(_pComputeShader))
	{
		ImGui::Separator();

		static bool bShowReadMe = false;
		if (ImGui::Button("ReadMe"))
		{
			if (bShowReadMe)
				bShowReadMe = false;
			else
				bShowReadMe = true;

		}


		if (bShowReadMe)
		{
			//ImGui::SameLine();
			ImGui::TextWrapped(
				"You can not modify these Data!\n"
				"just Modify data on Particle System component \nand it will send it to this shader");
			ImGui::Spacing();


		}



		CParticleUpdateShader* pPShader = dynamic_cast<CParticleUpdateShader*>(_pComputeShader);

		// Particle Buffer 
		CStructuredBuffer* ParticleBuffer = pPShader->GetParticleBuffer();
		int elementCnt = ParticleBuffer->GetElementCount();

		ImGui::PushItemWidth(100.f);

		ImGui::Text("ParticleBuffer ElementCnt");
		ImGui::SameLine(250);
		ImGui::InputInt("##ParticleBufferElementCnt", &elementCnt, ImGuiInputTextFlags_ReadOnly);


		int elementSize = ParticleBuffer->GetElementSize();

		ImGui::Text("ParticleBuffer ElementSize");
		ImGui::SameLine(250);
		ImGui::InputInt("byte", &elementSize, ImGuiInputTextFlags_ReadOnly);

		ImGui::PopItemWidth();

		// ParticleData Buffer 
		CStructuredBuffer* ParticleDataBuffer = pPShader->GetParticleDataBuffer();
		elementCnt = ParticleDataBuffer->GetElementCount();

		ImGui::PushItemWidth(100.f);

		ImGui::Text("ParticleDataBuffer ElementCnt");
		ImGui::SameLine(250);
		ImGui::InputInt("##ParticleBufferElementCnt", &elementCnt, ImGuiInputTextFlags_ReadOnly);


		elementSize = ParticleDataBuffer->GetElementSize();

		ImGui::Text("ParticleDataBuffer ElementSize");
		ImGui::SameLine(250);
		ImGui::InputInt("byte", &elementSize, ImGuiInputTextFlags_ReadOnly);

		ImGui::PopItemWidth();


	}

}

void ComputeShaderUI::ShowTestShader(CComputeShader* _pComputeShader)
{

	if (nullptr != dynamic_cast<CTestShader*>(_pComputeShader))
	{
		CTestShader* pShader = dynamic_cast<CTestShader*>(_pComputeShader);

		Ptr<CTexture> pOutputTex = pShader->GetOutputTex();
		string name = "Output Tex";

		CTexture* pTex = ParamUI::Param_Tex(name, pOutputTex.Get());


	}


}

