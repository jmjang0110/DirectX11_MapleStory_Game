#include "pch.h"
#include "CameraUI.h"


#include <Engine/CCamera.h>

CameraUI::CameraUI()
    : ComponentUI("Camera", COMPONENT_TYPE::CAMERA)
{
    SetSize(Vec2(0.f, 180.f));
}

CameraUI::~CameraUI()
{
}

void CameraUI::update()
{
}

void CameraUI::render_update()
{
    ComponentUI::render_update();

    CCamera* pCameraCom = GetTargetObject()->Camera();

    // PROJECTION TYPE
    ImGui::Text("Projection Type");
    ImGui::SameLine();

    PROJ_TYPE eType = pCameraCom->GetProjType();

    const char* szProjType[2] = { "ORTHOGRAPHIC", "PERSPECTIVE" };

    if (ImGui::BeginCombo("##ProjType", szProjType[(int)eType]))
    {
        for (int i = 0; i < 2; i++)
        {
            if (ImGui::Selectable(szProjType[i]))
            {
                pCameraCom->SetProjType((PROJ_TYPE)i);
            }
        }

        ImGui::EndCombo();
    }

    // 투영 가로길이
    ImGui::Text("Width");
    ImGui::SameLine(100);
    float fWidth = pCameraCom->GetWidth();

    if (PROJ_TYPE::PERSPECTIVE == eType)
    {
        ImGui::GetStyle().Alpha = 0.25f;
        ImGui::InputFloat("##InputWidth", &fWidth, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::GetStyle().Alpha = 1.f;
    }

    else
        ImGui::InputFloat("##InputWidth", &fWidth);

    pCameraCom->SetWidth(fWidth);

    // 종횡비
    ImGui::Text("AspectRatio");
    ImGui::SameLine(100);
    float fAspectRatio = pCameraCom->GetAspectRatio();
    ImGui::InputFloat("##InputAspectRatio", &fAspectRatio);
    pCameraCom->SetAspectRatio(fAspectRatio);


    // 시야 각      
    ImGui::Text("FOV");
    ImGui::SameLine(100);
    float fFOV = (pCameraCom->GetFOV() / XM_PI) * 180.f;

    if (PROJ_TYPE::ORTHOGRAPHIC == eType)
    {
        ImGui::GetStyle().Alpha = 0.25f;
        ImGui::InputFloat("##InputFOV", &fFOV, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::GetStyle().Alpha = 1.f;
    }
    else
        ImGui::InputFloat("##InputFOV", &fFOV);

    fFOV = (fFOV / 180.f) * XM_PI;
    pCameraCom->SetFOV(fFOV);

    // 시야 거리
    ImGui::Text("Far Distance");
    ImGui::SameLine(100);
    float fFar = pCameraCom->GetFar();
    ImGui::InputFloat("##InputFar", &fFar);
    pCameraCom->SetFar(fFar);
}
