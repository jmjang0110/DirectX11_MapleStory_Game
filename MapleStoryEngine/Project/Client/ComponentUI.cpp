#include "pch.h"
#include "ComponentUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>




ComponentUI::ComponentUI(const string& _strName, COMPONENT_TYPE _eComType)
	: UI(_strName)
	, m_eComType(_eComType)
	, m_bActive(false)
	, m_bDel(false)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::update()
{
	if (nullptr == m_pTargetObject)
		return;

	CComponent* pComponent = m_pTargetObject->GetComponent(m_eComType);
	m_bActive = pComponent->IsActive();
}

void ComponentUI::render_update()
{
	// 담당 Component 이름
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button(ToString(m_eComType));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	// Component 활성화 체크
	CComponent* pComponent = m_pTargetObject->GetComponent(m_eComType);

	// Transform 은 기본 설정 Component로 default Component 
	if (pComponent->GetType() != COMPONENT_TYPE::TRANSFORM)
	{
		// Component Active Button 
		bool IsActive = pComponent->IsActive();
		ImGui::SameLine();
		ImGui::Checkbox("##ComponentActive", &IsActive);

		if (pComponent->IsActive() != IsActive)
		{
			if (IsActive)
				pComponent->Activate();
			else
				pComponent->Deactivate();
		}

		ImGui::SameLine(300);
		if (ImGui::Button("X"))
		{
			m_bDel = true;
		}

		if (m_bDel)
		{
			ImGui::OpenPopup("ReallyDelete?");
			bool unused_open = true;
			if (ImGui::BeginPopupModal("ReallyDelete?", &unused_open))
			{

				ImGui::TextColored(ImVec4(1.f,0.f,0.f,1.f),"\t\tWARNING!!\n\nAre You Really DELETE this Component ?? \n\n");
				if (ImGui::Button("Yes"))
				{
					m_bDel = false;
					//m_pTargetObject->DeleteComponent(m_eComType);

					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();
				if (ImGui::Button("No"))
				{
					m_bDel = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}


	}
}
