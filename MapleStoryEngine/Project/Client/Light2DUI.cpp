#include "pch.h"
#include "Light2DUI.h"


#include <Engine/CLight2D.h>
#include <Engine/CGameObject.h>

Light2DUI::Light2DUI()
	: ComponentUI("Light2D", COMPONENT_TYPE::LIGHT2D)
{


}

Light2DUI::~Light2DUI()
{


}



void Light2DUI::update()
{

	ComponentUI::update();

}

void Light2DUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();
	CLight2D* pLight = pTargetObject->Light2D();
	tLightInfo LightInfo = pLight->GetLightInfo();

	/*
		struct tLightInfo
	{
		tLightColor color;
		int			iLightType;
	};
	*/

	ImGui::Text("Range"); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Range", &LightInfo.fRange);
	ImGui::PopItemWidth();
	pLight->SetRange(LightInfo.fRange);

	ImGui::Text("Angle"); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##Angle", &LightInfo.fAngle);
	ImGui::PopItemWidth();
	pLight->SetAngle(LightInfo.fAngle);

	ImGui::Text("DirX"); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##DirX", &LightInfo.vLightDir.x);
	ImGui::PopItemWidth();

	ImGui::Text("DirY"); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##DirY", &LightInfo.vLightDir.y);
	ImGui::PopItemWidth();

	ImGui::Text("DirZ"); ImGui::SameLine();
	ImGui::PushItemWidth(60);
	ImGui::DragFloat("##DirZ", &LightInfo.vLightDir.z);
	ImGui::PopItemWidth();
	pLight->SetLightDir(LightInfo.vLightDir);



	m_LightInfo.color;
	m_LightInfo.fRange;
	m_LightInfo.vLightDir;
	m_LightInfo.iLightType;
	m_LightInfo.vWorldPos;



}

