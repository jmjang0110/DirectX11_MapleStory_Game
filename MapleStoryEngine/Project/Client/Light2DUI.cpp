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

	tLightInfo LightInfo = GetTargetObject()->Light2D()->GetLightInfo();


	ImGui::InputFloat("Range", &LightInfo.fRange);
	ImGui::InputFloat("Ragne", &LightInfo.fRange);


	m_LightInfo.color;
	m_LightInfo.fRange;
	m_LightInfo.vLightDir;
	m_LightInfo.iLightType;
	m_LightInfo.vWorldPos;



}

