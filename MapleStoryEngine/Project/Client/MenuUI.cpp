#include "pch.h"
#include "MenuUI.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

#include <Script/CScriptMgr.h>

MenuUI::MenuUI()
    : UI("Menu")
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::update()
{
}

void MenuUI::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        render_update();


        ImGui::EndMainMenuBar();
    }
}

void MenuUI::render_update()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Packaging", NULL, &m_bPackaging))

            ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Scene"))
    {
        ImGui::MenuItem("Scene Save", NULL, &m_bSceneSave);
        ImGui::MenuItem("Scene Load", NULL, &m_bSceneLoad);

        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();
        if (SCENE_STATE::PLAY == eState)
            m_strPlayPause = "Pause";
        else
            m_strPlayPause = "Play";

        ImGui::MenuItem(m_strPlayPause.c_str(), NULL, &m_bScenePlayPause);
        ImGui::MenuItem("Stop", NULL, &m_bSceneStop);

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Component"))
    {
        if (ImGui::BeginMenu("Add Component"))
        {
            ImGui::MenuItem("MeshRender", NULL);
            ImGui::MenuItem("Camera", NULL);
            ImGui::MenuItem("Collider2D", NULL);
            ImGui::MenuItem("Collider3D", NULL);
            ImGui::MenuItem("Animator2D", NULL);
            ImGui::MenuItem("Animator3D", NULL);

            if (ImGui::BeginMenu("Add Script"))
            {
                vector<wstring> vecScriptName;
                CScriptMgr::GetScriptInfo(vecScriptName);
                for (size_t i = 0; i < vecScriptName.size(); ++i)
                {
                    string strScriptName = string(vecScriptName[i].begin(), vecScriptName[i].end());
                    ImGui::MenuItem(strScriptName.c_str(), NULL);
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Tools"))
    {
        ImGui::EndMenu();
    }



}
