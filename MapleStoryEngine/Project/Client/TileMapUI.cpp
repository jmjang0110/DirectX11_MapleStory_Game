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



	Reset();

}

TileMapUI::~TileMapUI()
{
	Safe_Del_Vec(m_vimgNode);
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

	// TileCount
	ImGui::PushItemWidth(100);
	ImGui::Text("Tile %dx%d",pTileMap->GetTileCountX(), pTileMap->GetTileCountY());
	ImGui::SameLine(100);
	ImGui::InputInt("##TileCountX", &m_iMapCountX);
	ImGui::SameLine();
	ImGui::InputInt("##TileCountY", &m_iMapCountY);


	if (ImGui::Button("Setting"))
	{
		pTileMap->SetTileMapCount(m_iMapCountX, m_iMapCountY);
		// 타일 개수 / 사이즈 만큼 타일맵 크기를 늘려야한다. 
		Vec2 SlicePixel = pTileMap->GetTileSize();
		pTargetObj->Transform()->SetRelativeScale(Vec3(m_iMapCountX * SlicePixel.x, m_iMapCountY * SlicePixel.y, 1.f));
		m_iMapCountX = 0;
		m_iMapCountY = 0;


	}
	
	ImGui::PopItemWidth();


	// Slice pixel Size 
	ImGui::PushItemWidth(200);
	Vec2 SlicePixel = pTargetObj->TileMap()->GetTileSize();
	float pxSlice[2] = { SlicePixel.x , SlicePixel.y };

	ImGui::Text("Tile px Size");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##TilepxSize", &pxSlice[0]);
	pTargetObj->TileMap()->SetTileSize(Vec2(pxSlice[0], pxSlice[1]));
	ImGui::PopItemWidth();


	// Tilemap list

	
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


	//ImGui::BeginChild("Selected Tile", ImVec2(200.f, 200.f), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();


	if (nullptr != m_pSelected_Tile)
	{
		Ptr<CTexture> SelectedTile_Atlas = m_pSelected_Tile->_parent->_parent->pAtlasTex;
		ImTextureID _texid = SelectedTile_Atlas->GetSRV().Get();

		int MaxRow = SelectedTile_Atlas->Height() / 30.f;  // 행
		int MaxCol = SelectedTile_Atlas->Width() /  30.f ; // 열 

		ImVec2 size(30.f * m_pSelected_Tile->iCol, 30.f * m_pSelected_Tile->iRow);
		ImGui::InvisibleButton("##empty", size);
		const ImVec2 p0 = ImGui::GetItemRectMin();
		const ImVec2 p1 = ImGui::GetItemRectMax();

		ImGui::PushClipRect(p0, p1, true);
		draw_list->AddRect(p0, p1, IM_COL32(120, 120, 120, 255)); // RGBA

		int imgeIDx = m_pSelected_Tile->vTilesInfo[0].iImgIdx; // 처음것을 기준 a
		Vec2 vLT = Vec2((imgeIDx % MaxCol) * 30.f, (imgeIDx / MaxCol) * 30.f);
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

				// 타일 세팅 ! ======================
				if (m_pSelected_Tile && bTileSet)
				{
					
						for (int i = 0; i < m_pSelected_Tile->iRow; ++i)
						{
							for (int k = 0; k < m_pSelected_Tile->iCol; ++k)
							{
								int TileIdx = iIdx + (iTileXCount * i) + k;
								if (TileIdx >= GetTargetObject()->TileMap()->GetTileCountX() * GetTargetObject()->TileMap()->GetTileCountY())
									continue;

								GetTargetObject()->TileMap()->SetTileData(TileIdx, m_pSelected_Tile->vTilesInfo[i * m_pSelected_Tile->iCol + k].iImgIdx);

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
		Obj->TileMap()->SetTileSize(Vec2(30.f, 30.f));
	}

	FillimgFIleinfo(_imgFIleNode, (TileImgFile*)_imgFIleNode->GetData());
}



TreeNode* TileMapUI::PushimgFiletoTree(const wstring _FileName, TreeNode* _pDestNode)
{
	TileImgFile* pimgFile = new TileImgFile;
	pimgFile->Name = _FileName;
	wstring TilemapRelativePath = L"texture//tilemap//" + pimgFile->Name + L"//";
	pimgFile->pAtlasTex = CResMgr::GetInst()->Load<CTexture>(pimgFile->Name, TilemapRelativePath + pimgFile->Name + L"Tile.png");

	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
		, string(_FileName.begin(), _FileName.end())
		, (DWORD_PTR)pimgFile);
	pNode->SetObjType(OBJECT_TYPE::DUMMY);

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

	// "enH0" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enH0";
	pPack->num = 3;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "enH1" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enH1";
	pPack->num = 3;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "enV0" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enV0";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "enV1" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"enV1";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "edU" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"edU";
	pPack->num = 4;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "edD" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"edD";
	pPack->num = 4;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);


	// "sIL" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"sIL";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

	// "sIR" - Tile Package
	pPack = new TilePackage;
	pPack->_parent = pimgFile;
	pPack->name = L"sIR";
	pPack->num = 2;
	PackageNode = PushPackageFiletoTree(pPack, _pDestNode);
	CreateNewTilesInfo(PackageNode, pPack, pimgFile);

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


