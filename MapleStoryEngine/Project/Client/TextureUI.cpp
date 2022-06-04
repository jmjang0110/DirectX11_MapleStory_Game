#include "pch.h"
#include "TextureUI.h"

#include <Engine/CTexture.h>

TextureUI::TextureUI()
	: ResInfoUI("Texture", RES_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{

}

void TextureUI::update()
{
	// To Do


	ResInfoUI::update();
}

void TextureUI::render_update()
{
	ResInfoUI::render_update();

	// To Do
	CTexture* pTexture = dynamic_cast<CTexture*>(GetTargetRes());
	assert(pTexture);

	// Texture Key
	string strName = string(pTexture->GetKey().begin(), pTexture->GetKey().end());
	ImGui::Text("Texture Name");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureName", (char*)strName.c_str(), strName.capacity(), ImGuiInputTextFlags_ReadOnly);

	// Texture Resolution
	char szWidth[255] = {};
	char szHeight[255] = {};
	_itoa_s((UINT)pTexture->Width(), szWidth, 255, 10);
	_itoa_s((UINT)pTexture->Height(), szHeight, 255, 10);

	ImGui::Text("Resolution");
	ImGui::SameLine(100);

	ImGui::PushItemWidth(50);
	ImGui::InputText("##Width", szWidth, 255, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	ImGui::InputText("##Height", szHeight, 255, ImGuiInputTextFlags_ReadOnly);

	// Texture Image
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
	ImGui::Image((ImTextureID)pTexture->GetSRV().Get()
		, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);


}

