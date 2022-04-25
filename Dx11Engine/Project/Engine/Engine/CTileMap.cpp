#include "pch.h"
#include "CTileMap.h"

#include "CTransform.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"


CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_iRowCount(0)
	, m_iColCount(0)
	, m_iTileCountX(2)
	, m_iTileCountY(2)
{
	// 메쉬, 재질
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));

	m_vecTileData.resize(m_iTileCountX * m_iTileCountY); // vector 의 공간을 미리 늘려놓는다. 


}

CTileMap::~CTileMap()
{

}



void CTileMap::finalupdate()
{
	if (nullptr == m_pAtlasTex)
		return;

	// 가로 세로 pixel 값을 UV 값을 전환 
	// m_vSliceUV : 한칸 자를 UV 사이즈 
	m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());

	
	
}

void CTileMap::UpdateData()
{
	if (nullptr == m_pAtlasTex)
		return;

	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_pAtlasTex);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &m_iTileCountX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_iTileCountY);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_vSliceUV);

	// 모든 타일 데이터를 상수버퍼를 통해 레지스터로 바인딩
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TILEMAP);
	pCB->SetData(m_vecTileData.data(), sizeof(tTileData) * m_iTileCountX * m_iTileCountY);
	pCB->UpdateData(); // register binding

}


void CTileMap::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial() || nullptr == m_pAtlasTex)
		return;

	UpdateData();

	Transform()->UpdateData();
	GetMaterial()->UpdateData();
	GetMesh()->render();
}


void CTileMap::SetTileMapCount(UINT _iCountX, UINT _iCountY)
{
	m_iTileCountX = _iCountX;
	m_iTileCountY = _iCountY;

	ClearTileData();
}

void CTileMap::SetTileData(int _iTileIdx, int _imgIdx)
{
	if (nullptr == m_pAtlasTex)
	{
		return;
	}


	m_vecTileData[_iTileIdx].iImgIdx = _imgIdx;

	// 아틀라스에서 타일의 행, 렬 개수 구하기
	m_iColCount = (UINT)m_pAtlasTex->Width() / (UINT)m_vSlicePixel.x;
	m_iRowCount = (UINT)m_pAtlasTex->Height() / (UINT)m_vSlicePixel.y;

	
	// 각각의 타일 정보를 m_vecTileData[] 에 저장한다. 
	int iRow = m_vecTileData[_iTileIdx].iImgIdx / m_iColCount; // 행 
	int iCol = m_vecTileData[_iTileIdx].iImgIdx % m_iColCount; // 열 

	m_vecTileData[_iTileIdx].vLTUV = Vec2(m_vSliceUV.x * iCol, m_vSliceUV.y * iRow);
	


}

void CTileMap::ClearTileData()
{
	// m_vecTileData 데이터를 버린다. 
	vector<tTileData> vecTileData;
	m_vecTileData.swap(vecTileData);

	m_vecTileData.resize(m_iTileCountX * m_iTileCountY);
	

}
