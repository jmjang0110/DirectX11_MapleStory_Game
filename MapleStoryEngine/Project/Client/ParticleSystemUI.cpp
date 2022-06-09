#include "pch.h"
#include "ParticleSystemUI.h"


#include <Engine/CParticleSystem.h>

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)

{
	SetSize(Vec2(0.f, 380.f));

}

ParticleSystemUI::~ParticleSystemUI()
{
}




void ParticleSystemUI::update()
{
	ComponentUI::update();


}

void ParticleSystemUI::render_update()
{
	ComponentUI::render_update();

	CParticleSystem* pParticleSystem = GetTargetObject()->ParticleSystem();
	string strParticleName = string(GetTargetObject()->GetName().begin(), GetTargetObject()->GetName().end());


	ImGui::Text("ParticleSystem  Name");
	ImGui::SameLine(150);
	ImGui::InputText("##MaterialName", (char*)strParticleName.c_str(), strParticleName.capacity(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Separator();

	int PosInherit = pParticleSystem->GetPosInherit();
	ImGui::Text("PosInherit");
	ImGui::SameLine(150);
	ImGui::InputInt("##PosInherit", &PosInherit);
	pParticleSystem->SetPosInherit(PosInherit);

	int maxCnt = pParticleSystem->GetMaxCount();
	ImGui::Text("MaxCount");
	ImGui::SameLine(150);
	ImGui::InputInt("##MaxCount", &maxCnt);
	pParticleSystem->SetMaxCount(maxCnt);

	int AliveCnt = pParticleSystem->GetAliveCount();
	ImGui::Text("AliveCnt");
	ImGui::SameLine(150);
	ImGui::InputInt("##AliveCnt", &AliveCnt);
	pParticleSystem->SetAliveCount(AliveCnt);

	ImGui::Separator();

	float MinLifeTime = pParticleSystem->GetMinLifeTime();
	ImGui::Text("MinLifeTime");
	ImGui::SameLine(150);
	ImGui::InputFloat("##MinLifeTime", &MinLifeTime);
	pParticleSystem->SetMinLifeTime(MinLifeTime);


	float MaxLifeTime = pParticleSystem->GetMaxLifeTime();
	ImGui::Text("MaxLifeTime");
	ImGui::SameLine(150);
	ImGui::InputFloat("##MaxLifeTime", &MaxLifeTime);
	pParticleSystem->SetMaxLifeTime(MaxLifeTime);

	ImGui::Separator();

	float StartSpeed = pParticleSystem->GetStartSpeed();
	ImGui::Text("StartSpeed");
	ImGui::SameLine(150);
	ImGui::InputFloat("##StartSpeed", &StartSpeed);
	pParticleSystem->SetStartSpeed(StartSpeed);


	float EndSpeed = pParticleSystem->GetEndSpeed();
	ImGui::Text("EndSpeed");
	ImGui::SameLine(150);
	ImGui::InputFloat("##EndSpeed", &EndSpeed);
	pParticleSystem->SetEndSpeed(EndSpeed);

	ImGui::Separator();

	Vec4 StartColor = pParticleSystem->GetStartColor();
	ImGui::Text("StartColor");
	ImGui::SameLine(150);
	ImGui::ColorEdit4("##StartColor", &StartColor.x);
	pParticleSystem->SetStartColor(StartColor);

	Vec4 EndColor = pParticleSystem->GetEndColor();
	ImGui::Text("EndColor");
	ImGui::SameLine(150);
	ImGui::ColorEdit4("##EndColor", &EndColor.x);
	pParticleSystem->SetEndColor(EndColor);

	ImGui::Separator();

	Vec3 StartScale = pParticleSystem->GetStartScale();
	ImGui::Text("StartScale");
	ImGui::SameLine(150);
	ImGui::InputFloat3("##StartScale", &StartScale.x);
	pParticleSystem->SetStartScale(StartScale);

	Vec3 EndScale = pParticleSystem->GetEndScale();
	ImGui::Text("EndScale");
	ImGui::SameLine(150);
	ImGui::InputFloat3("##EndScale", &EndScale.x);
	pParticleSystem->SetEndScale(EndScale);

	ImGui::Separator();

	float CreateDistance = pParticleSystem->GetParticleCreateDIstance();
	ImGui::Text("CreateDistance");
	ImGui::SameLine(150);
	ImGui::InputFloat("##CreateDistance", &CreateDistance);
	pParticleSystem->SetParticleCreateDIstance(CreateDistance);


	float CreateTerm = pParticleSystem->GetParticleCreateTerm();
	ImGui::Text("CreateTerm");
	ImGui::SameLine(150);
	ImGui::InputFloat("##CreateTerm", &CreateTerm);
	pParticleSystem->SetParticleCreateTerm(CreateTerm);
}

