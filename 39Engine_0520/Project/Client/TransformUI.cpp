#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI("Transform", COMPONENT_TYPE::TRANSFORM)	
{
}

TransformUI::~TransformUI()
{
}

void TransformUI::update()
{
	ComponentUI::update();

	CGameObject* pTargetObj = GetTargetObject();
	if (nullptr == pTargetObj)
		return;
		
	CTransform* pTransform = pTargetObj->Transform();

	m_vRelativePos = pTransform->GetRelativePos();
	m_vRelativeScale = pTransform->GetRelativeScale();
	m_vRelativeRot = pTransform->GetRelativeRotation();
}

void TransformUI::render_update()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button("TRANSFORM");
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}