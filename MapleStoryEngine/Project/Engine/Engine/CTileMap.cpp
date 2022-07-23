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
	, m_iTileCountX(0)
	, m_iTileCountY(0)
	, m_bBufferUpdated(false)
	, m_iShowGrid(0)
{
	// 메쉬, 재질
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TileMapMtrl.mtrl"));

	m_vecTileData.resize((size_t)(m_iTileCountX * m_iTileCountY));

	m_pBuffer = new CStructuredBuffer;
}

CTileMap::CTileMap(const CTileMap& _origin)
	: CRenderComponent(_origin)
	, m_vSlicePixel(_origin.m_vSlicePixel)
	, m_vSliceUV(_origin.m_vSliceUV)
	, m_iRowCount(_origin.m_iRowCount)
	, m_iColCount(_origin.m_iColCount)
	, m_iTileCountX(_origin.m_iTileCountX)
	, m_iTileCountY(_origin.m_iTileCountY)
	, m_vecTileData(_origin.m_vecTileData)
	, m_pBuffer(nullptr)
	, m_bBufferUpdated(false)
{
	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(tTileData), m_iTileCountX * m_iTileCountY, SB_TYPE::READ_WRITE, true, nullptr);


	// 메쉬, 재질
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TileMapMtrl.mtrl"));

	m_vecTileData.resize((size_t)(m_iTileCountX * m_iTileCountY));

	if (nullptr != _origin.m_pAtlasTex)
		SetAtlasTex(_origin.m_pAtlasTex);

}

CTileMap::~CTileMap()
{
	SAFE_DELETE(m_pBuffer);
}



void CTileMap::finalupdate()
{

}

void CTileMap::UpdateData()
{
	if (nullptr == m_pAtlasTex)
		return;


	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_pAtlasTex);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &m_iTileCountX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_iTileCountY);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_2, &m_iShowGrid);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_vSliceUV);

	// 모든 타일 데이터(m_vecTileData) 를 구조화버퍼를 통해 t16 레지스터로 바인딩
	if (false == m_bBufferUpdated)
	{
		m_pBuffer->SetData(m_vecTileData.data(), m_iTileCountX * m_iTileCountY);
		m_bBufferUpdated = true;
	}

	m_pBuffer->UpdateData(PIPELINE_STAGE::PS, 16);
}

void CTileMap::render()
{
	//if (GetOwner()->GetName() == L"Number")
	//	return;

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

void CTileMap::SetTileData(int _iTileIdx, int _iImgIdx)
{
	if (nullptr == m_pAtlasTex)
	{
		return;
	}
	
	m_vecTileData[_iTileIdx].iImgIdx = _iImgIdx;

	// 아틀라스에서 타일의 행, 렬 개수 구하기
	m_iColCount = (UINT)m_pAtlasTex->Width() / (UINT)m_vSlicePixel.x;
	m_iRowCount = (UINT)m_pAtlasTex->Height() / (UINT)m_vSlicePixel.y;

	if (m_iColCount <= 0)
		m_iColCount = 1;
	if (m_iRowCount <= 0)
		m_iRowCount = 1;

	int iRow = m_vecTileData[_iTileIdx].iImgIdx / m_iColCount;
	int iCol = m_vecTileData[_iTileIdx].iImgIdx % m_iColCount;

	m_vecTileData[_iTileIdx].vLTUV = Vec2(m_vSliceUV.x * iCol, m_vSliceUV.y * iRow);

	m_bBufferUpdated = false;
}

void CTileMap::ClearTileData()
{
	vector<tTileData> vecTileData;
	m_vecTileData.swap(vecTileData);

	m_vecTileData.resize((size_t)(m_iTileCountX * m_iTileCountY));

	// 구조화 버퍼도 크기에 대응한다.
	size_t iBufferSize = (m_iTileCountX * m_iTileCountY) * sizeof(tTileData);

	if (m_pBuffer->GetBufferSize() < iBufferSize)
	{
		m_pBuffer->Create(sizeof(tTileData), m_iTileCountX * m_iTileCountY, SB_TYPE::READ_WRITE, true, nullptr); 
	}

	m_bBufferUpdated = false;
}


/*
		================== Todo ==================
*/
void CTileMap::SetTileSize(Vec2 _vPixelSize, Vec2 _AllTileSize)
{

	m_vSlicePixel = _vPixelSize;
	if (nullptr != m_pAtlasTex)
		m_vSliceUV = m_vSlicePixel / Vec2(_AllTileSize.x , _AllTileSize.y);

}

void CTileMap::SetTileData(int _iTileIdx, int _iImgIdx, Vec2 _AllTileSize, Vec2 StartPos_px)
{
	Vec2 StartUV = Vec2(StartPos_px.x / m_pAtlasTex->Width(), StartPos_px.y / m_pAtlasTex->Height());

	if (nullptr == m_pAtlasTex)
	{
		return;
	}

	m_vecTileData[_iTileIdx].iImgIdx = _iImgIdx;

	// 아틀라스에서 타일의 행, 렬 개수 구하기
	m_iColCount = (UINT)_AllTileSize.x / (UINT)m_vSlicePixel.x;
	m_iRowCount = (UINT)_AllTileSize.y  / (UINT)m_vSlicePixel.y;

	int iRow = m_vecTileData[_iTileIdx].iImgIdx / m_iColCount;
	int iCol = m_vecTileData[_iTileIdx].iImgIdx % m_iColCount;

	m_vecTileData[_iTileIdx].vLTUV = StartUV + Vec2(m_vSliceUV.x * iCol, m_vSliceUV.y * iRow);

	m_bBufferUpdated = false;

}



void CTileMap::SaveToScene(FILE* _pFile)
{
	CRenderComponent::SaveToScene(_pFile);

	SaveResPtr(m_pAtlasTex, _pFile);

	fwrite(&m_vSlicePixel, sizeof(Vec2), 1, _pFile);
	fwrite(&m_vSliceUV, sizeof(Vec2), 1, _pFile);
	fwrite(&m_iRowCount, sizeof(UINT), 1, _pFile);
	fwrite(&m_iColCount, sizeof(UINT), 1, _pFile);
	fwrite(&m_iTileCountX, sizeof(UINT), 1, _pFile);
	fwrite(&m_iTileCountY, sizeof(UINT), 1, _pFile);
	fwrite(m_vecTileData.data(), sizeof(tTileData), m_vecTileData.size(), _pFile);
}

void CTileMap::LoadFromScene(FILE* _pFile)
{
	CRenderComponent::LoadFromScene(_pFile);

	LoadResPtr(m_pAtlasTex, _pFile);

	fread(&m_vSlicePixel, sizeof(Vec2), 1, _pFile);
	fread(&m_vSliceUV, sizeof(Vec2), 1, _pFile);
	fread(&m_iRowCount, sizeof(UINT), 1, _pFile);
	fread(&m_iColCount, sizeof(UINT), 1, _pFile);
	fread(&m_iTileCountX, sizeof(UINT), 1, _pFile);
	fread(&m_iTileCountY, sizeof(UINT), 1, _pFile);

	SetTileMapCount(m_iTileCountX, m_iTileCountY);

	///m_vecTileData.resize((size_t)(m_iTileCountX * m_iTileCountY));
	
	ClearTileData();
	fread(m_vecTileData.data(), sizeof(tTileData), (size_t)(m_iTileCountX * m_iTileCountY), _pFile);
}