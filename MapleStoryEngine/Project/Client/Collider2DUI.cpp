#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>
#include <Engine/CDevice.h>

Collider2DUI::Collider2DUI()
	: ComponentUI("Collider2D", COMPONENT_TYPE::COLLIDER2D)
{

	SetSize(Vec2(0.f, 150.f));

}

Collider2DUI::~Collider2DUI()
{


}



void Collider2DUI::update()
{
	ComponentUI::update();


}

void Collider2DUI::render_update()
{
	ComponentUI::render_update();

	CCollider2D* pCollider2D = GetTargetObject()->Collider2D();
	string strName = string(GetTargetObject()->GetName().begin(), GetTargetObject()->GetName().end());

	ImGui::Text("Collider2D  Name");
	ImGui::SameLine(150);
	ImGui::InputText("##Collider2DName", (char*)strName.c_str(), strName.capacity(), ImGuiInputTextFlags_ReadOnly);


	int CollisionCnt = pCollider2D->GetCollisionCount();
	ImGui::Text("Collision Count : %d ", CollisionCnt);


	COLLIDER2D_TYPE Col2DType = pCollider2D->GetCollider2DType();
	const char* typeName[2] = {"BOX", "CIRCLE"};

	ImGui::Text("Collider2D Type");
	ImGui::SameLine(150);

	if (ImGui::BeginCombo("##Collider2DType", typeName[(int)Col2DType], 0))
	{
		for (int i = 0; i < (int)COLLIDER2D_TYPE::END; ++i)
		{
			if (ImGui::Selectable(typeName[i]))
			{
				pCollider2D->SetCollider2DType((COLLIDER2D_TYPE)i);

			}
		}

		ImGui::EndCombo();
	}

	// OffSetPos
	ImGui::PushItemWidth(150.f);
	Vec2 OffSetPos = pCollider2D->GetOffsetPos();
	Vec2 resolution = CDevice::GetInst()->GetRenderResolution();
	ImGui::Text("OffSetPos");
	ImGui::SameLine(150);
	ImGui::DragFloat("##OffSetPos_x", &OffSetPos.x, 0.5f);
	ImGui::SameLine();
	ImGui::DragFloat("##OffSetPos_y", &OffSetPos.y, 0.5f);
	pCollider2D->SetOffsetPos(OffSetPos);
	ImGui::PopItemWidth();

	// Offset Scale
	ImGui::PushItemWidth(150.f);
	Vec2 OffSetScale = pCollider2D->GetOffsetScale();
	ImGui::Text("OffSetScale");
	ImGui::SameLine(150);
	ImGui::DragFloat("##OffSetScale_x", &OffSetScale.x, 0.5f);
	ImGui::SameLine();
	ImGui::DragFloat("##OffSetScale_y", &OffSetScale.y, 0.5f);
	pCollider2D->SetOffsetScale(OffSetScale);

	ImGui::PopItemWidth();

	// Offset Rotation
	ImGui::PushItemWidth(150.f);
	float  Angle = pCollider2D->GetOffsetRotation();
	ImGui::Text("OffSetRotation");
	ImGui::SameLine(150);
	ImGui::DragFloat("##OffSetRotation_z", &Angle, 0.05f);
	pCollider2D->SetOffsetRotation(Angle);

	ImGui::PopItemWidth();

}

