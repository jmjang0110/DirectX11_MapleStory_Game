#include "pch.h"
#include "ResInfoUI.h"



ResInfoUI::ResInfoUI(const string& _strName, RES_TYPE _eType)
	: UI(_strName)
	, m_eResType(_eType)
{
}

ResInfoUI::~ResInfoUI()
{

}

void ResInfoUI::render_update()
{
	// 담당 Resource 이름
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.1f, 0.1f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.1f, 0.1f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.1f, 0.1f, 0.8f));
	ImGui::Button(ToString(m_eResType));
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}