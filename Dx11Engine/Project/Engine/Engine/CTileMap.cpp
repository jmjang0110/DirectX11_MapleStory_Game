#include "pch.h"
#include "CTileMap.h"


#include "CTransform.h"
#include "CResMgr.h"


CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
{
	// Mesh Mateirial
	
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	GetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));

	
}

CTileMap::~CTileMap()
{
}




void CTileMap::finalupdate()
{
}

void CTileMap::UpdateData()
{
}



void CTileMap::render()
{





}