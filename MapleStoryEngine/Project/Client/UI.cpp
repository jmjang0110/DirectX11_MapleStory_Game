#include "pch.h"
#include "UI.h"



UI::UI(const string& _strName)
	: m_strName(_strName)
	, m_pParentUI(nullptr)
	, m_bOpen(true)
	, m_bModal(false)
{
}

UI::~UI()
{
	Safe_Del_Vec(m_vecChildUI);
}

void UI::update()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->update();
	}
}

void UI::render()
{
	if (!(m_vPos.x == 0.f && m_vPos.y == 0.f) && m_bOpen)
	{
		ImGui::SetNextWindowPos(m_vPos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(m_vSize);
	}

	if (nullptr == m_pParentUI)
	{
		if (m_bOpen)
		{
			string strName = m_strTitle + m_strName;

			if (m_bModal)
			{
				// 모달은 가운데 고정
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				ImGui::OpenPopup(strName.c_str());
				if (ImGui::BeginPopupModal(strName.c_str(), &m_bOpen))
				{
					// 사이즈 재확인
					ImVec2 vSize = ImGui::GetWindowSize();
					m_vSize.x = vSize.x;
					m_vSize.y = vSize.y;

					render_update();

					for (size_t i = 0; i < m_vecChildUI.size(); ++i)
					{
						m_vecChildUI[i]->render();
						ImGui::Separator();
					}

					ImGui::EndPopup();
				}
			}
			else
			{
				ImGui::Begin(strName.c_str(), &m_bOpen);

				// 사이즈 재확인
				ImVec2 vSize = ImGui::GetWindowSize();
				m_vSize.x = vSize.x;
				m_vSize.y = vSize.y;

				render_update();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->render();

					if (m_vecChildUI[i]->IsActive())
						ImGui::Separator();
				}

				ImGui::End();
			}
		}
	}

	else
	{
		if (m_bOpen)
		{
			ImGui::BeginChild(m_strName.c_str(), m_vSize);

			render_update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->render();

				if (m_vecChildUI[i]->IsActive())
					ImGui::Separator();
			}

			ImGui::EndChild();
		}
	}
}




Vec2::operator ImVec2() const
{
	return ImVec2(x, y);
}
