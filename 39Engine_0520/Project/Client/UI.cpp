#include "pch.h"
#include "UI.h"


UI::UI(const string& _strName)
	: m_strName(_strName)
	, m_pParentUI(nullptr)
{
}

UI::~UI()
{
}

void UI::render()
{
	if (nullptr == m_pParentUI)
	{
		ImGui::Begin(m_strName.c_str());

		render_update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::End();
	}

	else
	{
		ImGui::BeginChild(m_strName.c_str());

		render_update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->render();
		}

		ImGui::EndChild();
	}
}

