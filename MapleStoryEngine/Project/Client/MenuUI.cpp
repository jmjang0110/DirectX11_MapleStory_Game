#include "pch.h"
#include "MenuUI.h"

#include <Engine/CCore.h>
#include <Engine/CPathMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Script/CScriptMgr.h>


#include <Script/CSceneSaveLoad.h>



// ==== todo 
#include "CImGuiMgr.h"
#include "SceneOutlinerTool.h"
// =========


MenuUI::MenuUI()
    : UI("Menu")
    , m_bPackaging(false)
    , m_bSceneSave(false)
    , m_bSceneLoad(false)
    , m_bScenePlayPause(false)
    , m_bSceneStop(false)
{
}


MenuUI::~MenuUI()
{
}

void MenuUI::update()
{
    Task();
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
        ImGui::MenuItem("Packaging", NULL, &m_bPackaging);


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

        if (SCENE_STATE::STOP == eState)
            ImGui::MenuItem("Stop", NULL, &m_bSceneStop, false);
        else
            ImGui::MenuItem("Stop", NULL, &m_bSceneStop, true);

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



void MenuUI::Task()
{
    if (m_bSceneSave)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"scene";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetSaveFileName(&ofn))
        {
            CSceneSaveLoad::SaveScene(CSceneMgr::GetInst()->GetCurScene(), szName);
        }

        m_bSceneSave = false;
    }

    else if (m_bSceneLoad)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"scene";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetOpenFileName(&ofn))
        {
            CScene* pLoadScene = CSceneSaveLoad::LoadScene(szName);
            CSceneMgr::GetInst()->ChangeScene(pLoadScene);

          
        }

        // Todo ====== 
          // SceneOutlinertool 을 바뀐 Scene 으로 변경 
          // CImGuiMgr 에 Delegate 등록 
 /*       tUIDelegate tDeleteCom;
        tDeleteCom.dwParam = (DWORD_PTR)nullptr;
        tDeleteCom.pFunc = (PARAM_1)&SceneOutlinerTool::Reset;
        tDeleteCom.pInst = CImGuiMgr::GetInst()->FindUI("SceneOutlinerTool");

        CImGuiMgr::GetInst()->AddDelegate(tDeleteCom);*/
        // ===========

        SceneOutlinerTool* pUI = (SceneOutlinerTool*)CImGuiMgr::GetInst()->FindUI("SceneOutlinerTool");
        pUI->Reset();


        m_bSceneLoad = false;
    }

    if (m_bScenePlayPause)
    {
        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::STOP == eState || SCENE_STATE::PAUSE == eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::PLAY);
        }
        else if (SCENE_STATE::PLAY == eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::PAUSE);
        }

        m_bScenePlayPause = false;
    }

    if (m_bSceneStop)
    {
        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::STOP != eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::STOP);
            CSceneFile* pSceneFile = pCurScene->GetSceneFile().Get();

            wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pSceneFile->GetRelativePath();
            CScene* pNewScene = CSceneSaveLoad::LoadScene(strFilePath);
            CSceneMgr::GetInst()->ChangeScene(pNewScene);
        }

        // SceneOutliner 갱신
        ((SceneOutlinerTool*)CImGuiMgr::GetInst()->FindUI("SceneOutlinerTool"))->Reset();

        m_bSceneStop = false;

    }
}
