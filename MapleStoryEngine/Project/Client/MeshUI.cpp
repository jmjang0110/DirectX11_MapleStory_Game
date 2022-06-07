#include "pch.h"
#include "MeshUI.h"

#include <Engine/CMesh.h>
#include <Engine/CResMgr.h>




MeshUI::MeshUI()
	: ResInfoUI("Mesh", RES_TYPE::MESH)
{


}

MeshUI::~MeshUI()
{
}


void MeshUI::update()
{
	ResInfoUI::update();

}

void MeshUI::render_update()
{
	ResInfoUI::render_update();

	// To Do
	CMesh* pMesh = dynamic_cast<CMesh*>(GetTargetRes());
	assert(pMesh);

	// Material Key
	string strName = string(pMesh->GetKey().begin(), pMesh->GetKey().end());

	// Material Name
	ImGui::Text("Mesh");
	ImGui::SameLine(100);
	ImGui::InputText("##MeshName", (char*)strName.c_str(), strName.capacity(), ImGuiInputTextFlags_ReadOnly);

	Vertex*		pVtxMem = (Vertex*)pMesh->GetVtxMem();
	UINT		VtxCnt = pMesh->GetVtxCnt();
	UINT*		pIdxMem = (UINT*)pMesh->GetIdxMem();
	UINT		IdxCnt = pMesh->GetIdxCnt();

	ImGui::Text("PIxel Count : %d", VtxCnt);
	ImGui::Text("Index Count : %d", IdxCnt);


	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// Mesh  ±×¸°´Ù. 
	float thickness = 1.0f;
	ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 colf_dot = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	ImGui::ColorEdit4("Color", &colf.x);

	const ImVec2 p = ImGui::GetCursorScreenPos();
	const ImU32 color = ImColor(colf);
	const ImU32 color_dot = ImColor(colf_dot);
	const float size = 150.f;

	ImVec2 startPoint;
	startPoint.x = p.x + 100.f;
	startPoint.y = p.y + 100.f;

	if (strName == "RectMesh" || strName == "CircleMesh")
	{
		for (int i = 0; i < IdxCnt; i += 3)
		{
			int firstIdx	= pIdxMem[i];
			int secondIdx	= pIdxMem[i + 1];
			int thirdIdx	= pIdxMem[i + 2];

			Vtx first		= pVtxMem[firstIdx];
			Vtx second		= pVtxMem[secondIdx];
			Vtx third		= pVtxMem[thirdIdx];

			// 0.1f = 10 pixel 
			first.vPos	*= size;
			second.vPos *= size;
			third.vPos	*= size;

			// start - middle
			draw_list->AddCircleFilled(ImVec2(startPoint.x + first.vPos.x, startPoint.y + first.vPos.y * -1)
				, 2.5f, color_dot, 12);
			draw_list->AddLine(ImVec2(startPoint.x + first.vPos.x, startPoint.y + first.vPos.y * -1)
				, ImVec2(startPoint.x + second.vPos.x, startPoint.y + second.vPos.y * -1) 
				, color, thickness);

			// middle - end
			draw_list->AddCircleFilled(ImVec2(startPoint.x + second.vPos.x, startPoint.y + second.vPos.y * -1)
				, 2.5f, color_dot, 12);
			draw_list->AddLine(ImVec2(startPoint.x + second.vPos.x, startPoint.y + second.vPos.y * -1)
				, ImVec2(startPoint.x + third.vPos.x, startPoint.y + third.vPos.y * -1)  
				, color, thickness);

			// end - start  
			draw_list->AddCircleFilled(ImVec2(startPoint.x + third.vPos.x, startPoint.y + third.vPos.y * -1)
				, 2.5f, color_dot, 12);
			draw_list->AddLine(ImVec2(startPoint.x + third.vPos.x, startPoint.y + third.vPos.y * -1)
				, ImVec2(startPoint.x + first.vPos.x, startPoint.y + first.vPos.y * -1) 
				, color, thickness);
		}
	}
	else if (strName == "RectMesh_LineStrip" || strName == "CircleMesh_LineStrip")
	{
		for (int i = 0; i < IdxCnt - 1; ++i)
		{
			int firstIdx = pIdxMem[i];
			int secondIdx = pIdxMem[i + 1];

			Vtx first = pVtxMem[firstIdx];
			Vtx second = pVtxMem[secondIdx];

			// 0.1f = 10 pixel 
			first.vPos *= size;
			second.vPos *= size;

			draw_list->AddCircleFilled(ImVec2(startPoint.x + first.vPos.x, startPoint.y + first.vPos.y * -1)
				, 2.5f, color_dot, 12);
			draw_list->AddLine(ImVec2(startPoint.x + first.vPos.x, startPoint.y + first.vPos.y * -1)
				, ImVec2(startPoint.x + second.vPos.x, startPoint.y + second.vPos.y * -1)
				, color, thickness);
		}
	}
	else if (strName == "PointMesh")
	{
		int firstIdx = pIdxMem[0];
		Vtx first = pVtxMem[firstIdx];

		draw_list->AddCircleFilled(ImVec2(startPoint.x + first.vPos.x, startPoint.y + first.vPos.y * -1)
			, 2.5f, color_dot, 12);

	}

}
