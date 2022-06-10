#include "pch.h"
#include "TileMapUI.h"

#include "TreeUI.h"


#include <Engine/CTileMap.h>

/*
	
	* 메이플 스토리 타일을 만들거라서 한 칸을 90x60 으로 만들것이다. 
	
	* 트리 를 사용해서 타일을 고를 수 있도록 만들것
	
	* tile info 에 정보들을 저장해서 타일을 고르면 그 타일의 이미지를 띄우고
	  정보를 띄울 것
	 
	 * 이미지를 띄우기 위해서 미리 타일 사이즈를 저장해 놓을 것 
	


*/

TileMapUI::TileMapUI()
	: ComponentUI("TileMap", COMPONENT_TYPE::TILEMAP)
	, m_TreeUI(nullptr)
	, m_iTileCountX(0)
	, m_iTileCountY(0)

{
	SetSize(Vec2(0.f, 300.f));



}

TileMapUI::~TileMapUI()
{

}

void TileMapUI::update()
{
	ComponentUI::update();





}

void TileMapUI::render_update()
{
	ComponentUI::render_update();


	CGameObject* pObject = GetTargetObject();

	int TileCountX = pObject->TileMap()->GetTileCountX();
	int TileCountY = pObject->TileMap()->GetTileCountY();

	ImGui::PushItemWidth(100);

	ImGui::Text("Tile Count");
	ImGui::SameLine(150);
	ImGui::InputInt("##TileCountX", &TileCountX);
	ImGui::SameLine();
	ImGui::InputInt("##TileCountY", &TileCountY);

	pObject->TileMap()->SetTileMapCount(TileCountX, TileCountY);

	// 이거 해줘야 이미지가 출력됨 
	for (int i = 0; i < TileCountX * TileCountY; ++i)
	{
		pObject->TileMap()->SetTileData(i, 0);
	}


	ImGui::PopItemWidth();

	ImGui::PushItemWidth(200);

	Vec2 SlicePixel = pObject->TileMap()->GetTileSize();
	ImGui::Text("Tile px Size");
	ImGui::SameLine(150);
	ImGui::InputFloat2("##TilepxSize", &SlicePixel.x);
	pObject->TileMap()->SetTileSize(SlicePixel);

	ImGui::PopItemWidth();



	

}

