#include "pch.h"
#include "TileMapUI.h"

#include "TreeUI.h"

#include <Engine/CScene.h>
#include <Engine/CLayer.h>


#include <Engine/CTileMap.h>
#include <Engine/CTexture.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>


#include <Engine/CSceneMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CResMgr.h>


/*
	
	* 메이플 스토리 타일을 만들거라서 한 칸을 30x30 으로 만들것이다. 
	
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
	, m_bEditMode(false)
	, m_bUseMapleStory_ImgFile(false)

{
	SetSize(Vec2(0.f, 300.f));


	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("Tile");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);

	m_TreeUI->UseDragDropSelf(false);
	m_TreeUI->UseDragDropOuter(false);
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&TileMapUI::TileClicked);

	//AddChild(m_TreeUI);

	m_tile = new Tile;
	m_tile->iRow = 1;
	m_tile->iCol = 1;

	m_tilePackage = new TilePackage;
	
	m_tile->_parent = m_tilePackage;
	m_tilePackage->_parent = &m_ImgFile;


	m_ImgFile.imgFile.insert(make_pair(L"Tile", m_tile));



	Reset();

}

TileMapUI::~TileMapUI()
{
	//UI* DelTreeUI = (UI*)m_TreeUI;

	Safe_Del_Vec(m_StoreimgFile);
	Safe_Del_Vec(m_StorePackage);

	SAFE_DELETE(m_TreeUI);
	SAFE_DELETE(m_tilePackage);

	//SAFE_DELETE(m_tile);


}

void TileMapUI::update()
{


	ComponentUI::update();

}

void TileMapUI::render_update()
{
	if (m_bimgFIleChange)
	{
		Reset();
		m_bimgFIleChange = false;
	}

	ComponentUI::render_update();


	CGameObject* pTargetObj = GetTargetObject();
	CTileMap* pTileMap = pTargetObj->TileMap();

	//m_iMapCountX = pTileMap->GetTileCountX();
	//m_iMapCountY = pTileMap->GetTileCountY();

	if (nullptr == pTargetObj)
		return;


	// Use MapleStory ImgFile Button
//ImGui::Checkbox("Use MapleStory ImgFile Tile", &m_bUseMapleStory_ImgFile);

	if (ImGui::Button("MapleStory ImgFile"))
	{
		m_bUseMapleStory_ImgFile = !m_bUseMapleStory_ImgFile;
		if (false == m_bUseMapleStory_ImgFile)
		{
			m_pSelected_Tile = nullptr;
			m_pSelected_imgFile = nullptr;
			m_Selected_imgFIle_Name = "";
			

		}
		else
		{

			pTileMap->SetTileMapCount(m_iMapCountX, m_iMapCountY);
			m_vTileSize = Vec2(30.f, 30.f);
			pTargetObj->TileMap()->SetTileSize(m_vTileSize);
			// 타일 개수 / 사이즈 만큼 타일맵 크기를 늘려야한다. 
			pTargetObj->Transform()->SetRelativeScale(Vec3(m_iMapCountX * m_vTileSize.x, m_iMapCountY * m_vTileSize.y, 1.f));
		}
	}
	if (m_bUseMapleStory_ImgFile)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.f, 0.5f, 1.f, 1.f), "Use ImgFile");
	}
	else
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Not Use ImgFile");
	}

	// Not Maple Story ImgFile - Select Atalas Texture
	if (!m_bUseMapleStory_ImgFile)
	{
		SelectAtlasTexture();
		SelectTile();

	}



	// TileCount
	ImGui::PushItemWidth(100);
	ImGui::Text("Tile %dx%d",pTileMap->GetTileCountX(), pTileMap->GetTileCountY());
	ImGui::SameLine(100);
	ImGui::InputInt("##TileCountX", &m_iMapCountX);
	ImGui::SameLine();
	ImGui::InputInt("##TileCountY", &m_iMapCountY);




	
	// Slice pixel Size 
	ImGui::PushItemWidth(200);
	//Vec2 SlicePixel = pTargetObj->TileMap()->GetTileSize();
	static float pxSlice[2] = { m_vTileSize.x, m_vTileSize.y};

	ImGui::Text("Size %dx%d px", (UINT)pTileMap->GetTileSize().x, (UINT)pTileMap->GetTileSize().y);
	ImGui::SameLine(100);
	ImGui::InputFloat2("##TilepxSize", &pxSlice[0]);
	
	ImGui::PopItemWidth();
	
	

	if (ImGui::Button("Setting"))
	{
		m_tile->vTilesInfo.clear();
		m_tile->iCol = 0;
		m_tile->iRow = 0;
		m_tile->iImgIdxNum = 0;

		m_ImgFile.pAtlasTex = nullptr;

		m_bUseThisTile = false;

		m_vTileSize.x = pxSlice[0];
		m_vTileSize.y = pxSlice[1];


		m_pSelected_Tile = nullptr;
		m_pSelected_imgFile = nullptr;
		m_Selected_imgFIle_Name = "";

		pTileMap->ClearTileData();
		pTileMap->SetTileMapCount(m_iMapCountX, m_iMapCountY);
		// 타일 개수 / 사이즈 만큼 타일맵 크기를 늘려야한다. 
		pTargetObj->TileMap()->SetTileSize(m_vTileSize);
		Vec2 SlicePixel = pTileMap->GetTileSize();
		pTargetObj->Transform()->SetRelativeScale(Vec3(m_iMapCountX * SlicePixel.x, m_iMapCountY * SlicePixel.y, 1.f));

		

		if (m_bUseMapleStory_ImgFile) // 내가 짜놓은 형식에 맞게 
		{
			m_vTileSize = Vec2(30.f, 30.f);
			pTargetObj->TileMap()->SetTileSize(m_vTileSize);
			pTargetObj->Transform()->SetRelativeScale(Vec3(m_iMapCountX * m_vTileSize.x, m_iMapCountY * m_vTileSize.y, 1.f));

		}


	}

	ImGui::PopItemWidth();


	// Tilemap list
	if(m_bUseMapleStory_ImgFile)
		m_TreeUI->render();
	

	// Show Grid Button 
	int grid = pTileMap->GetShowGrid();
	bool bGrid = (bool)grid;
	ImGui::Checkbox("Show Grid", &bGrid);
	if (bGrid == true)
		pTileMap->SetShowGrid(1);
	else
		pTileMap->SetShowGrid(0);


	// EditorMode Button 
	ImGui::Checkbox("EditorMode", &m_bEditMode);
	ImGui::Checkbox("Auto Set Next Tile", &m_bAutoSetNextTile);

	if(m_bEditMode)
		EditorUpdate();






	// Draw Tile Image 
	//ImGui::BeginChild("Selected Tile", ImVec2(200.f, 200.f), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	if (nullptr != m_pSelected_Tile)
	{
		Ptr<CTexture> SelectedTile_Atlas = m_pSelected_Tile->_parent->_parent->pAtlasTex;
		ImTextureID _texid = SelectedTile_Atlas->GetSRV().Get();

		int MaxRow = SelectedTile_Atlas->Height() / m_vTileSize.y;  // 행
		int MaxCol = SelectedTile_Atlas->Width() / m_vTileSize.x;  // 열 


		if (SelectedTile_Atlas->Width() < m_vTileSize.x)
			MaxCol = 1;
		if (SelectedTile_Atlas->Height() < m_vTileSize.y)
			MaxRow = 1;

		ImVec2 size(m_vTileSize.y * m_pSelected_Tile->iCol, m_vTileSize.x * m_pSelected_Tile->iRow);

		if (size.x >= 1.f && size.y >= 1.f)
		ImGui::InvisibleButton("##empty", size);
		const ImVec2 p0 = ImGui::GetItemRectMin();
		const ImVec2 p1 = ImGui::GetItemRectMax();

		ImGui::PushClipRect(p0, p1, true);
		draw_list->AddRect(p0, p1, IM_COL32(120, 120, 120, 255)); // RGBA

		int imgeIDx = m_pSelected_Tile->vTilesInfo[0].iImgIdx; // 처음것을 기준 a
		Vec2 vLT = Vec2((imgeIDx % MaxCol) * m_vTileSize.x, (imgeIDx / MaxCol) * m_vTileSize.y);
		Vec2 vSlice = Vec2(size.x, size.y);

		ImVec2 uv0 = ImVec2((vLT.x) / SelectedTile_Atlas->Width(), (vLT.y) / SelectedTile_Atlas->Height());
		ImVec2 uv1 = ImVec2((vLT.x + vSlice.x) / SelectedTile_Atlas->Width()
			, (vLT.y + vSlice.y) / SelectedTile_Atlas->Height());

		draw_list->AddImage(_texid, p0, p1, uv0, uv1);
		ImGui::PopClipRect();
	}
	//ImGui::EndChild();


}

void TileMapUI::EditorUpdate()
{

	if (KEY_PRESSED(KEY::LBTN))
	{

		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		// find Camera Obj !!  
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Default");
		CGameObject* pCameraObj = nullptr;

		if (nullptr != pLayer)
		{
			vector<CGameObject*> vObjs = pLayer->GetRootObjects();
			for (int i = 0; i < vObjs.size(); ++i)
			{
				if (vObjs[i]->GetName() == L"MainCamera")
				{
					pCameraObj = vObjs[i];
					break;
				}

			}
		}

		if (nullptr != pCameraObj)
		{
			const Vec3 vCameraPos = pCameraObj->Transform()->GetRelativePos();
			float fWidth = pCameraObj->Camera()->GetWidth();
			float fHeight = fWidth / pCameraObj->Camera()->GetAspectRatio();

			// 현재 window 화면창에서 정중앙을 (0.f ,0.f) 한 것을 기준으로 한 좌표값 
			Vec2 vDx11Pos = Vec2(vMousePos.x - (fWidth / 2.f), (fHeight / 2.f) - vMousePos.y);

			// 실제 월드상에서의 좌표값 
			Vec2 vRealPos = vDx11Pos + vCameraPos;

			Vec3 TilePos = GetTargetObject()->Transform()->GetRelativePos();
			Vec3 TileScale = GetTargetObject()->Transform()->GetRelativeScale();

			// 타일맵 전체의 좌상단 좌표값 
			Vec3 vTileMapLT = Vec3(TilePos.x - (TileScale.x / 2), TilePos.y + (TileScale.y / 2), TilePos.z);


			// 타일맵 내부에 클릭이 있었다면 
			if (0 <= vRealPos.x - vTileMapLT.x && vRealPos.x - vTileMapLT.x <= TileScale.x &&
				0 <= vTileMapLT.y - vRealPos.y && vTileMapLT.y - vRealPos.y <= TileScale.y)
			{
				m_bTilemapInnerClicked = true;

				int iCol = (vRealPos.x - vTileMapLT.x) / (GetTargetObject()->TileMap()->GetTileSize().x);
				int iRow = (vTileMapLT.y - vRealPos.y) / (GetTargetObject()->TileMap()->GetTileSize().y);

				int iTileXCount = GetTargetObject()->TileMap()->GetTileCountX();
				UINT iIdx = iTileXCount * iRow + iCol;

				// test 클릭되면일단 무조건 0번이미지 인덱스로 타일 세팅 ** Test =
				//GetTargetObject()->TileMap()->SetTileData(iIdx, 0);
				tTileData tileData = GetTargetObject()->TileMap()->GetTileData(iIdx);
				bool bTileSet = true;
				if (-1 != tileData.iImgIdx)
					bTileSet = false;

				// 타일 세팅 ! ====================== - Maple Img 전용 
				if (m_pSelected_Tile && bTileSet)
				{
					
						for (int i = 0; i < m_pSelected_Tile->iRow; ++i)
						{
							for (int k = 0; k < m_pSelected_Tile->iCol; ++k)
							{
								int TileIdx = iIdx + (iTileXCount * i) + k;
								if (TileIdx >= GetTargetObject()->TileMap()->GetTileCountX() * GetTargetObject()->TileMap()->GetTileCountY())
									continue;

								GetTargetObject()->TileMap()->SetTileData(TileIdx,m_pSelected_Tile->vTilesInfo[i * m_pSelected_Tile->iCol + k].iImgIdx);

							}
						}
				}
				// ======================
			}
		}


	}

	if (KEY_AWAY(KEY::LBTN))
	{
		if (m_bAutoSetNextTile && m_bTilemapInnerClicked)
		{
			m_bTilemapInnerClicked = false;
			if (m_pSelected_Tile)
			{
				// 자동으로 다음 타일로 변환 
				wstring tileName = m_pSelected_Tile->Name;

				int infonum = _wtoi(&tileName[0]); //  "0_bsc" -> "1_bsc" -> "2_bsc" ~~~
				if (infonum + 1 < m_pSelected_Tile->_parent->num)
				{
					wstring nextTileNum = std::to_wstring(infonum + 1);
					tileName[0] = nextTileNum[0];
					m_pSelected_Tile->_parent->_parent->imgFile.find(tileName);

					map<wstring, Tile*>::iterator iter = m_pSelected_Tile->_parent->_parent->imgFile.find(tileName);

					if (iter != m_pSelected_Tile->_parent->_parent->imgFile.end())
						m_pSelected_Tile = iter->second;
				}
				else
				{
					wstring nextTileNum = std::to_wstring(0);
					tileName[0] = nextTileNum[0];

					m_pSelected_Tile->_parent->_parent->imgFile.find(tileName);

					map<wstring, Tile*>::iterator iter = m_pSelected_Tile->_parent->_parent->imgFile.find(tileName);

					if (iter != m_pSelected_Tile->_parent->_parent->imgFile.end())
						m_pSelected_Tile = iter->second;

				}
			}
		}
		
	
	}

	if (KEY_PRESSED(KEY::RBTN))
	{

		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		// find Camera Obj !!  
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CLayer* pLayer = pCurScene->GetLayer(L"Default");
		CGameObject* pCameraObj = nullptr;

		if (nullptr != pLayer)
		{
			vector<CGameObject*> vObjs = pLayer->GetRootObjects();
			for (int i = 0; i < vObjs.size(); ++i)
			{
				if (vObjs[i]->GetName() == L"MainCamera")
				{
					pCameraObj = vObjs[i];
					break;
				}

			}
		}

		if (nullptr != pCameraObj)
		{
			const Vec3 vCameraPos = pCameraObj->Transform()->GetRelativePos();
			float fWidth = pCameraObj->Camera()->GetWidth();
			float fHeight = fWidth / pCameraObj->Camera()->GetAspectRatio();

			// 현재 window 화면창에서 정중앙을 (0.f ,0.f) 한 것을 기준으로 한 좌표값 
			Vec2 vDx11Pos = Vec2(vMousePos.x - (fWidth / 2.f), (fHeight / 2.f) - vMousePos.y);

			// 실제 월드상에서의 좌표값 
			Vec2 vRealPos = vDx11Pos + vCameraPos;

			Vec3 TilePos = GetTargetObject()->Transform()->GetRelativePos();
			Vec3 TileScale = GetTargetObject()->Transform()->GetRelativeScale();

			// 타일맵 전체의 좌상단 좌표값 
			Vec3 vTileMapLT = Vec3(TilePos.x - (TileScale.x / 2), TilePos.y + (TileScale.y / 2), TilePos.z);

			// 타일맵 내부에 클릭이 있었다면 
			if (0 <= vRealPos.x - vTileMapLT.x && vRealPos.x - vTileMapLT.x <= TileScale.x &&
				0 <= vTileMapLT.y - vRealPos.y && vTileMapLT.y - vRealPos.y <= TileScale.y)
			{
				int iCol = (vRealPos.x - vTileMapLT.x) / (GetTargetObject()->TileMap()->GetTileSize().x);
				int iRow = (vTileMapLT.y - vRealPos.y) / (GetTargetObject()->TileMap()->GetTileSize().y);

				int iTileXCount = GetTargetObject()->TileMap()->GetTileCountX();
				UINT iIdx = iTileXCount * iRow + iCol;
				// 지운다 
				GetTargetObject()->TileMap()->SetTileData(iIdx, -1);
			}
		}
	}


}


void TileMapUI::Reset()
{
	m_TreeUI->Clear();
	Safe_Del_Vec(m_StoreimgFile);
	Safe_Del_Vec(m_StorePackage);

	
	TreeNode* node1 = PushimgFiletoTree(L"YellowToyCastle", m_TreeUI->GetDummyNode());
	TreeNode* node2 = PushimgFiletoTree(L"WoodMarble", m_TreeUI->GetDummyNode());
	TreeNode* node3 = PushimgFiletoTree(L"VicportTown", m_TreeUI->GetDummyNode());
	TreeNode* node4 = PushimgFiletoTree(L"GrassySoil", m_TreeUI->GetDummyNode());


	if (m_Selected_imgFIle_Name == "YellowToyCastle")
		ResetimgFile(m_Selected_imgFIle_Name, node1);
	else if (m_Selected_imgFIle_Name == "WoodMarble")
		ResetimgFile(m_Selected_imgFIle_Name, node2);
	else if (m_Selected_imgFIle_Name == "VicportTown")
		ResetimgFile(m_Selected_imgFIle_Name, node3);
	else if (m_Selected_imgFIle_Name == "GrassySoil")
		ResetimgFile(m_Selected_imgFIle_Name, node4);

}

void TileMapUI::ResetimgFile(string _imgFileName, TreeNode* _imgFIleNode)
{
	CGameObject* Obj = GetTargetObject();
	 
	TileImgFile* imgNode = (TileImgFile*)_imgFIleNode->GetData();
	if (nullptr != Obj)
	{
		Obj->TileMap()->SetAtlasTex(imgNode->pAtlasTex);
		Obj->TileMap()->SetTileSize(Vec2(m_vTileSize.y, m_vTileSize.x));
	}

	FillimgFIleinfo(_imgFIleNode, (TileImgFile*)_imgFIleNode->GetData());


}



TreeNode* TileMapUI::PushimgFiletoTree(const wstring _FileName, TreeNode* _pDestNode)
{
	TileImgFile* pimgFile = new TileImgFile;
	pimgFile->Name = _FileName;
	wstring TilemapRelativePath = L"texture//tilemap//" + pimgFile->Name + L"//";
	pimgFile->pAtlasTex = CResMgr::GetInst()->Load<CTexture>(TilemapRelativePath + pimgFile->Name + L"Tile.png", TilemapRelativePath + pimgFile->Name + L"Tile.png");

	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_FileName.begin(), _FileName.end())
		, (DWORD_PTR)pimgFile);
	pNode->SetObjType(OBJECT_TYPE::DUMMY);


	m_StoreimgFile.push_back(pimgFile);

	return pNode;
}

TreeNode* TileMapUI::PushPackageFiletoTree(TilePackage * pPack, TreeNode* _pDestNode)
{
	
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(pPack->name.begin(), pPack->name.end())
		, (DWORD_PTR)pPack);

	return pNode;
}

TreeNode* TileMapUI::PushTiletoTree(Tile* _pTile, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_pTile->Name.begin(), _pTile->Name.end())
		, (DWORD_PTR)_pTile);
	pNode->SetObjType(OBJECT_TYPE::END);

	return pNode;

}


// delegate
void TileMapUI::TileClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;
	if (OBJECT_TYPE::DUMMY == pNode->GetObjType()) // 더미노드 라고 정함 - img
	{
		m_Selected_imgFIle_Name = pNode->GetName();
		m_pSelected_Tile = nullptr;
		//Reset(); -> 이때 Reset 을 하면 터진다. 다음 프레임에 해야됨  
		m_bimgFIleChange = true;

		return;
	}
	else if (OBJECT_TYPE::NONE == pNode->GetObjType()) // - package 
		return;

	DWORD_PTR data = pNode->GetData();
	m_pSelected_Tile = (Tile*)data;

}



// Store Tile Info 
TreeNode* TileMapUI::FillimgFIleinfo(TreeNode* _pDestNode, TileImgFile* pimgFile)
{

	// img File
	//TileImgFile* pimgFile = new TileImgFile;
	//pimgFile->Name = L"YellowToyCastle";

	// "bsc" - Tile Package
	TilePackage* pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"bsc";
	pPack->num = 6;
	TreeNode* PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	m_StorePackage.push_back(pPack);


	// "enH0" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enH0";
	pPack->num = 3;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);
	m_StorePackage.push_back(pPack);

	// "enH1" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enH1";
	pPack->num = 3;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);
	m_StorePackage.push_back(pPack);

	// "enV0" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enV0";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);
	m_StorePackage.push_back(pPack);

	// "enV1" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enV1";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);
	m_StorePackage.push_back(pPack);

	// "edU" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"edU";
	pPack->num = 4;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);
	m_StorePackage.push_back(pPack);

	// "edD" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"edD";
	pPack->num = 4;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);
	m_StorePackage.push_back(pPack);


	// "sIL" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"sIL";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);
	m_StorePackage.push_back(pPack);

	// "sIR" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"sIR";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);
	m_StorePackage.push_back(pPack);

	return nullptr;

}

void TileMapUI::CreateNewTilesInfo(TreeNode* _pDestNode, TilePackage* _pPackage, TileImgFile* _pimgFile)
{
	if (_pPackage->name == L"bsc")
	{
		int Startimgidx = 0;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = FillTileInfo(i, _pPackage->name, _pPackage, Startimgidx, 6, 2, 3);
			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);
			Startimgidx += 3;
			if (i == 2)
				Startimgidx = 24;
		}
	}

	else if (_pPackage->name == L"enH0")
	{
		int Startimgidx = 48;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = FillTileInfo(i, _pPackage->name, _pPackage, Startimgidx, 3, 2, 3);
			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);
			Startimgidx += 3;
		}
	}
	else if (_pPackage->name == L"enH1")
	{
		int Startimgidx = 72;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = FillTileInfo(i, _pPackage->name, _pPackage, Startimgidx, 3, 2, 3);
			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);
			Startimgidx += 3;
		}
	}
	else if (_pPackage->name == L"enV0")
	{
		int Startimgidx = 132;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = FillTileInfo(i, _pPackage->name, _pPackage, Startimgidx, 4, 2, 2);
			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);

			Startimgidx += 2;
		}
	}
	else if (_pPackage->name == L"enV1")
	{
		// Create enH0 info
		int Startimgidx = 136;
		for (int i = 0; i < _pPackage->num; ++i)
		{
			Tile* pTile = FillTileInfo(i, _pPackage->name, _pPackage, Startimgidx, 4, 2, 2);
			_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
			PushTiletoTree(pTile, _pDestNode);

			Startimgidx += 2;
		}
	}
	else if (_pPackage->name == L"edU")
	{
	int Startimgidx = 96;
	for (int i = 0; i < _pPackage->num; ++i)
	{
		Tile* pTile = FillTileInfo(i, _pPackage->name, _pPackage, Startimgidx, 4, 1, 2);
		_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
		PushTiletoTree(pTile, _pDestNode);

		Startimgidx += 2;
	}
	}

	else if (_pPackage->name == L"edD")
	{
	int Startimgidx = 108;
	for (int i = 0; i < _pPackage->num; ++i)
	{
		Tile* pTile = FillTileInfo(i, _pPackage->name, _pPackage, Startimgidx, 4, 1, 2);
		_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
		PushTiletoTree(pTile, _pDestNode);

		Startimgidx += 2;
	}
	}

	else if (_pPackage->name == L"sIL")
	{
	// Create enH0 info
	int Startimgidx = 168;
	for (int i = 0; i < _pPackage->num; ++i)
	{

		Tile* pTile = FillTileInfo(i, _pPackage->name, _pPackage, Startimgidx, 9, 3, 3);
		_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
		PushTiletoTree(pTile, _pDestNode);

		Startimgidx += 36;
	}
	}

	else if (_pPackage->name == L"sIR")
	{
	// Create enH0 info
	int Startimgidx = 171;
	for (int i = 0; i < _pPackage->num; ++i)
	{
		Tile* pTile = FillTileInfo(i, _pPackage->name, _pPackage, Startimgidx, 9, 3, 3);
		_pimgFile->imgFile.insert(make_pair(pTile->Name, pTile));
		PushTiletoTree(pTile, _pDestNode);

		Startimgidx += 36;
	}
	}

}

// row : 행 col : 열 
Tile* TileMapUI::FillTileInfo(int _TileID_Number, wstring PackageName, TilePackage* _pPackage
	, int _startImgIdx, int _iAllImgIdxNum, int _row, int _col)
{

		Tile* pTile = new Tile;
		string number;
		pTile->Name = std::to_wstring(_TileID_Number) + L"_" + PackageName;
		pTile->_parent = _pPackage;
		pTile->iImgIdxNum = _iAllImgIdxNum;
		pTile->iRow = _row;
		pTile->iCol = _col;


		int ImgAllColNum = 12;
		for (int i = 0; i < _row; ++i)
		{
			for (int k = 0; k < _col; ++k)
			{
				pTile->vTilesInfo.push_back(TileInfo((_startImgIdx + k) + (ImgAllColNum * i)));
			}
		}


		return pTile;

}


void TileMapUI::SelectAtlasTexture()
{

	const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::TEXTURE);

	vector<string> vecTexList;
	vecTexList.clear();
	for (const auto& pair : mapRes)
	{
		vecTexList.push_back(string(pair.first.begin(), pair.first.end()));
	}


	ImGui::PushItemWidth(200.f);
	string ImgFIleName = string(m_ImgFile.Name.begin(), m_ImgFile.Name.end());

	if (ImGui::BeginCombo("Atlas Texture", ImgFIleName.c_str(), ImGuiComboFlags_NoArrowButton))
	{

		for (int n = 0; n < vecTexList.size(); ++n)
		{
			if (ImGui::Selectable(vecTexList[n].c_str()))
			{

				// Tex 선택 
				wstring TexKey = wstring(vecTexList[n].begin(), vecTexList[n].end());
				m_ImgFile.pAtlasTex = CResMgr::GetInst()->FindRes<CTexture>(TexKey);
				m_ImgFile.Name = TexKey;

				if (m_tile)
					m_tile->vTilesInfo.clear();


			}

		}
		ImGui::EndCombo();
	}


	ImGui::PopItemWidth();


}

void TileMapUI::SelectTile()
{
	if (nullptr == m_ImgFile.pAtlasTex)
		return;

	ImGui::Begin("Atlas Tile");

	ImVec2			pos = ImGui::GetCursorScreenPos(); // Image 의 시작점 위치 (SCreen) LT
	ImGuiIO&		io = ImGui::GetIO();
	static Vec3		color = { 120.f, 120.f, 120.f };
	static Vec2		Clicked_px = Vec2(0.f, 0.f);

	float			width = m_ImgFile.pAtlasTex->Width();
	float			height = m_ImgFile.pAtlasTex->Height();
	float			ratio = 0.5f;

	ImVec2 Texsize = ImVec2(width * ratio, height * ratio);
	ImColor lineColor = ImColor(0.4f, 0.4f, 0.4f, 1.f);

	if (Texsize.x >= 1.f && Texsize.y >= 1.f)
	ImGui::InvisibleButton("##empty", Texsize);

	const ImVec2 p0 = ImGui::GetItemRectMin();
	const ImVec2 p1 = ImGui::GetItemRectMax();  // Pixel RB
	ImDrawList* draw_list = ImGui::GetWindowDrawList();


	// Image 
	ImGui::PushClipRect(p0, p1, true);
	draw_list->AddRect(p0, p1, IM_COL32(50.f, color.x, 50.f, 255)); // RGBA
	ImVec2 uv0 = ImVec2(0.f, 0.f);
	ImVec2 uv1 = ImVec2(1.f, 1.f);
	draw_list->AddImage(m_ImgFile.pAtlasTex->GetSRV().Get(), p0, p1, uv0, uv1);

	// 이미지의 행열 총 개수 
	int ImgIdx_Row = height / m_vTileSize.y; // 행 
	int ImgIdx_Col = width / m_vTileSize.x; // 열 

	if (width < m_vTileSize.x)
		ImgIdx_Col = 1;
	if (height < m_vTileSize.y)
		ImgIdx_Row = 1;


	ImVec2 Tilesize = m_vTileSize * ratio;

	// draw Column line 
	for (int i = 0; i < ImgIdx_Col; ++i)
	{
		draw_list->AddLine(ImVec2(pos.x + Tilesize.x * (i + 1), pos.y)
			, ImVec2(pos.x + Tilesize.x * (i + 1), pos.y + Texsize.y * (i + 1))
			, lineColor);
	}

	// draw Row line 
	for (int i = 0; i < ImgIdx_Row; ++i)
	{
	
		draw_list->AddLine(ImVec2(pos.x, pos.y + Tilesize.y * (i + 1))
			, ImVec2(pos.x + Texsize.x * (i + 1), pos.y + Tilesize.y * (i + 1))
			, lineColor);
	}

	ImGui::PopClipRect();

	if (ImGui::IsItemHovered())
	{

		ImGui::BeginTooltip();
		float PIxelPos_x = io.MousePos.x - pos.x;
		float PixelPos_y = io.MousePos.y - pos.y; // io.MousePos.y;

		m_iRow =	PixelPos_y / Tilesize.y; // 행 
		m_iCol =	PIxelPos_x / Tilesize.x; // 열

		ImVec2 LT = ImVec2(pos.x + m_iCol * Tilesize.x, pos.y + m_iRow * Tilesize.y);
		ImVec2 RB = ImVec2(LT.x + Tilesize.x, LT.y + Tilesize.y);

		ImColor Color = ImColor(0.2f, 1.f, 1.f, 1.f);
		draw_list->AddRect(LT, RB, Color, 0.5f, 1.f);

		ImGui::Text("%d Col %d Row", m_iCol, m_iRow);
		ImGui::Text("px:(%.2f, %.2f)", PIxelPos_x, height);
		ImGui::EndTooltip();
	}

	if (ImGui::IsItemClicked())
	{
		if (m_bUseThisTile)
		{
			m_bUseThisTile = false;
			m_pSelected_Tile = nullptr;

		}

		TileInfo NewTile(m_iRow * (ImgIdx_Col) + m_iCol);
		if (m_tile)
		{
			m_tile->vTilesInfo.push_back(NewTile);
			m_tile->iImgIdxNum += 1;
		}
	}

	// draw selected tile - Rect line in img
	if (nullptr != m_tile)
	{
		for (int i = 0; i < m_tile->vTilesInfo.size(); ++i)
		{
			int row = m_tile->vTilesInfo[i].iImgIdx / ImgIdx_Col;
			int col = m_tile->vTilesInfo[i].iImgIdx % ImgIdx_Col;

			ImVec2 LT = ImVec2(pos.x + col * Tilesize.x, pos.y + row * Tilesize.y);
			ImVec2 RB = ImVec2(LT.x + Tilesize.x, LT.y + Tilesize.y);

			ImColor Color = ImColor(0.f, 1.f, 0.1f, 1.f);
			draw_list->AddRect(LT, RB, Color, 0.5f, 1.f);

		}

	}
	ImGui::SameLine();

	ImGui::BeginChild("Button", ImVec2(300.f, 100.f), true, ImGuiWindowFlags_HorizontalScrollbar);

	if (m_tile)
	{
		ImGui::PushItemWidth(100.f);

		ImGui::InputInt("Row", &m_tile->iRow);
		ImGui::SameLine();
		ImGui::InputInt("Col", &m_tile->iCol);

		ImGui::PopItemWidth();

	}
	
	ImGui::TextColored(ImVec4(0.2f, 1.f, 1.f, 1.f), "Num = %d", m_tile->iImgIdxNum);
	ImGui::Checkbox("Use This Tile", &m_bUseThisTile);
	if (m_bUseThisTile)
	{
		
		if (m_tile->iImgIdxNum > 0 && m_tile->iCol * m_tile->iRow == m_tile->iImgIdxNum)
		{
			m_pSelected_Tile = nullptr;
			m_pSelected_imgFile = &m_ImgFile;

			GetTargetObject()->TileMap()->SetAtlasTex(m_ImgFile.pAtlasTex);
			m_pSelected_Tile = m_tile;

		}
		// 잘못 입력 
		else
		{
			m_bUseThisTile = false;
			m_pSelected_Tile = nullptr;
		}
			
	
	}


	ImGui::EndChild();
	
	ImGui::SameLine();
	if (ImGui::Button("clear"))
	{
		if (m_tile)
		{
			m_tile->vTilesInfo.clear();
			m_tile->iImgIdxNum = 0;
			m_tile->iRow = 0;
			m_tile->iCol = 0;
			m_pSelected_Tile = nullptr;
			m_bUseThisTile = false;


		}

	}
	ImGui::SameLine();
	// Draw Tile Image 
	ImGui::BeginChild("Selected Tile", ImVec2(300.f, 300.f), true, ImGuiWindowFlags_HorizontalScrollbar);

		if (nullptr != m_ImgFile.pAtlasTex && (m_vTileSize.x >= 0.f && m_vTileSize.y >= 0.f)
			&& m_tile && m_tile->vTilesInfo.size() > 0) 
		{
			Ptr<CTexture> SelectedTile_Atlas = m_ImgFile.pAtlasTex;
			ImTextureID _texid = SelectedTile_Atlas->GetSRV().Get();

			int MaxRow = SelectedTile_Atlas->Height() / m_vTileSize.y;  // 행
			int MaxCol = SelectedTile_Atlas->Width() / m_vTileSize.x;  // 열 

			if (width < m_vTileSize.x)
				MaxCol = 1;
			if (height < m_vTileSize.y)
				MaxRow = 1;



			//ImVec2 size(m_vTileSize.y * m_pSelected_Tile->iCol, m_vTileSize.x * m_pSelected_Tile->iRow);
	
			for (int i = 0; i < m_tile->vTilesInfo.size(); ++i)
			{
				ImGui::PushID(i);
		
		
				ImVec2 size(m_vTileSize);
				if(size.x >= 1.f && size.y >= 1.f)
				ImGui::InvisibleButton("##empty", size);
				const ImVec2 p0 = ImGui::GetItemRectMin();
				const ImVec2 p1 = ImGui::GetItemRectMax();


				ImGui::PushClipRect(p0, p1, true);
				draw_list->AddRect(p0, p1, IM_COL32(120, 120, 120, 255)); // RGBA

			
				int imgeIDx = m_tile->vTilesInfo[i].iImgIdx; // 처음것을 기준 a
				Vec2 vLT = Vec2((imgeIDx % MaxCol) * m_vTileSize.x, (imgeIDx / MaxCol) * m_vTileSize.y);
				Vec2 vSlice = Vec2(size.x, size.y);

				uv0 = ImVec2((vLT.x) / SelectedTile_Atlas->Width(), (vLT.y) / SelectedTile_Atlas->Height());
				uv1 = ImVec2((vLT.x + vSlice.x) / SelectedTile_Atlas->Width()
					, (vLT.y + vSlice.y) / SelectedTile_Atlas->Height());

				draw_list->AddImage(_texid, p0, p1, uv0, uv1);

				
				 if((i + 1) % 4 != 0)
					ImGui::SameLine();
				ImGui::PopClipRect();

				ImGui::PopID();

			}
		
		}

		ImGui::EndChild();

		ImGui::Text("tex size : (%.2f, %.2f)", width, Texsize.y);


	
		/*
			// 버튼 형식 타일맵 고르기 
	// image index 행열 개수 
	// 내가 생각한대로 되긴 하지만 프레임이 너무 떨어진다.... 
		for (int i = 0; i < ImgIdx_Col; ++i)
	{
		for (int k = 0; k < ImgIdx_Row; ++k)
		{
			int imgIdx = i * ImgIdx_Col + k;
			ImGui::PushID(imgIdx);

			ImVec2 Tilesize = m_vTileSize;												// Size of the image we want to make visible
			ImVec2 uv0 = ImVec2((Tilesize.y * k) / width, (Tilesize.x * i) / height);	// UV coordinates for lower-left
			ImVec2 uv1 = uv0;
			uv1.x += Tilesize.x / width;
			uv1.y += Tilesize.x / height;

			ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint
			
			Tilesize.x *= ratio;
			Tilesize.y *= ratio;
			ImGui::ImageButton(m_ImgFile.pAtlasTex->GetSRV().Get(), Tilesize, uv0, uv1, 1.f, bg_col, tint_col);
		
			// 시작점이 7.f 이네 
			const float startOffsetDefault = 8.f;

			 if (k != ImgIdx_Row - 1)
				ImGui::SameLine(startOffsetDefault + (Tilesize.x  + 3.f) * (k + 1));

			ImGui::PopID();
		}
	}

		
		*/
	
	ImGui::End();
}

