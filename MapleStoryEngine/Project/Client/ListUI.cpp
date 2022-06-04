#include "pch.h"
#include "ListUI.h"

ListUI::ListUI()
    : UI("##ListUI")
    , m_Inst(nullptr)
    , m_DBCEvent(nullptr)
{
    SetPos(Vec2(800.f, 300.f));
    SetSize(Vec2(500.f, 600.f));
}

ListUI::~ListUI()
{

}

void ListUI::update()
{
}

void ListUI::render_update()
{
    ImVec2 vWinSize = ImGui::GetWindowContentRegionMax();
    vWinSize.y -= 27;

    static int item_current_idx = 0; // Here we store our selection data as an index.
    if (ImGui::BeginListBox("##ListUIListBox", vWinSize))
    {
        for (size_t i = 0; i < m_vecList.size(); i++)
        {
            const bool is_selected = (item_current_idx == i);
            if (ImGui::Selectable(m_vecList[i].c_str(), is_selected))
            {
                item_current_idx = i;
            }

            // 리스트의 아이템의 더블클릭 확인
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            {
                Deactivate();

                if (nullptr != m_Inst && nullptr != m_DBCEvent)
                {
                    (m_Inst->*m_DBCEvent)((DWORD_PTR)m_vecList[i].c_str());
                }
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
}

void ListUI::Activate()
{
    UI::Activate();
    SetSize(Vec2(500.f, 600.f));

}