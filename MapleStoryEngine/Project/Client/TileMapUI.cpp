#include "pch.h"
#include "TileMapUI.h"

#include "TreeUI.h"


#include <Engine/CTileMap.h>
#include <Engine/CPathMgr.h>
#include <Engine/CTexture.h>
#include <Engine/CResMgr.h>
#include <Engine/CTransform.h>

/*
	
	* 메이플 스토리 타일을 만들거라서 한 칸을 90x60 으로 만들것이다. 
	
	* 트리 를 사용해서 타일을 고를 수 있도록 만들것
	
	* tile info 에 정보들을 저장해서 타일을 고르면 그 타일의 이미지를 띄우고
	  정보를 띄울 것
	 
	 * 이미지를 띄우기 위해서 미리 타일 사이즈를 저장해 놓을 것 
	

*/

TileMapUI::TileMapUI()
	: ComponentUI("TileMapUI", COMPONENT_TYPE::TILEMAP)
	, m_TreeUI(nullptr)
	, m_iMapCountX(0)
	, m_iMapCountY(0)

{
	SetSize(Vec2(0.f, 300.f));


	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Tile");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);

	m_TreeUI->UseDragDropSelf(false);
	m_TreeUI->UseDragDropOuter(false);

	//AddChild(m_TreeUI);


	m_TreeUI->SetClickedDelegate(this, (CLICKED)&TileMapUI::TileClicked);
	

	Reset();

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

	// TileCount
	ImGui::PushItemWidth(100);
	ImGui::Text("Tile Count");
	ImGui::SameLine(150);
	ImGui::InputInt("##TileCountX", &TileCountX);
	ImGui::SameLine();
	ImGui::InputInt("##TileCountY", &TileCountY);

	pObject->TileMap()->SetTileMapCount(TileCountX, TileCountY);
	ImGui::PopItemWidth();


	// Slice pixel Size 
	ImGui::PushItemWidth(200);
	Vec2 SlicePixel = pObject->TileMap()->GetTileSize();
	float pxSlice[2] = { SlicePixel.x , SlicePixel.y };

	ImGui::Text("Tile px Size");
	ImGui::SameLine(150); 
	ImGui::InputFloat2("##TilepxSize", &pxSlice[0]);
	pObject->TileMap()->SetTileSize(Vec2(pxSlice[0], pxSlice[1]));
	ImGui::PopItemWidth();


	// Tilemap list
	ImGui::BeginChild("tilemaplist", ImVec2(150.f, 200.f), true, ImGuiWindowFlags_HorizontalScrollbar);
	m_TreeUI->render();
	ImGui::EndChild();


	// 타일 개수 / 사이즈 만큼 타일맵 크기를 늘려야한다. 
	pObject->Transform()->SetRelativeScale(Vec3(TileCountX * pxSlice[0], TileCountY * pxSlice[1], 1.f));

	// Show Tile 
	for (int i = 1; i < TileCountX * TileCountY; ++i)
	{
		pObject->TileMap()->SetTileData(i, -1);
	}
	pObject->TileMap()->SetTileData(0, -1);


}


void TileMapUI::Reset()
{
	m_TreeUI->Clear();
	
	//TileImgFile* pimgFile = new TileImgFile;

	//TreeNode* pFileNode = PushTileFiletoTree(L"yellowToyCastle.img", pimgFile, m_TreeUI->GetDummyNode());
	//TreeNode* pNode = Push_darkpurpleToyCastle_toTree(pFileNode);


}

TreeNode* TileMapUI::Push_darkpurpleToyCastle_toTree(TreeNode* _pDestNode)
{

	// img File
	TileImgFile* pimgFile = new TileImgFile;
	pimgFile->Name = L"yellowToyCastle";
	wstring TilemapRelativePath = L"texture//tilemap//" + pimgFile->Name + L"//";
	pimgFile->pAtlasTex = CResMgr::GetInst()->Load<CTexture>(pimgFile->Name, TilemapRelativePath + pimgFile->Name + L".png");

	// tile - bsc 
	tile* pTile = new tile;
	pTile->Name = L"bsc";
	pTile->_parent = pimgFile;
	pTile->iTileNum = 6;
	pTile->StartPos_px = Vec2(0.f, 0.f);
	pTile->AllTileSize = Vec2(270.f, 120.f);
	for (int i = 0; i < pTile->iTileNum; ++i)
	{ 
		info tileInfo;
		tileInfo.iImgIdx = i;
		tileInfo.tileSize = Vec2(90.f, 60.f);

		pTile->vTilesInfo.push_back(tileInfo); 
	}

	pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
	PushTileImagetoTree(pTile, _pDestNode);




	return nullptr;

}


TreeNode* TileMapUI::PushTileFiletoTree(const wstring _FileName, TileImgFile* pimgFile, TreeNode* _pDestNode)
{
	pimgFile->Name = _FileName;
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_FileName.begin(), _FileName.end())
		, (DWORD_PTR)pimgFile);


	return pNode;
}

TreeNode* TileMapUI::PushTileImagetoTree(tile* _pTile, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pTile->Name.begin(), _pTile->Name.end())
		, (DWORD_PTR)_pTile);


	return pNode;

}


void TileMapUI::TileClicked(DWORD_PTR _dw)
{
	//TreeNode* pNode = (TreeNode*)_dw;
	//if (pNode->GetName() == "darkpurpleToyCaslte.img")
	//	return;

	//DWORD_PTR data = pNode->GetData();
	//m_pSelected_Tile = (tile*)data;

	//if (m_pSelected_Tile->Name == L"bsc")
	//{
	//	GetTargetObject()->TileMap()->SetAtlasTex(m_pSelected_Tile->_parent->pAtlasTex);
	//	GetTargetObject()->TileMap()->SetTileSize(m_pSelected_Tile->vTilesInfo[0].tileSize, m_pSelected_Tile->AllTileSize);
	//	GetTargetObject()->TileMap()->SetTileData(0, 0, m_pSelected_Tile->AllTileSize, m_pSelected_Tile->StartPos_px);

	//}


}


