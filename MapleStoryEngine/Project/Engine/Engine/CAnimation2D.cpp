#include "pch.h"
#include "CAnimation2D.h"

#include "CAnimator2D.h"
#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CResMgr.h"


CAnimation2D::CAnimation2D()
	: m_iCurFrmIdx(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
	, m_iHorizontal_Flip(0)
	, m_iVertical_Flip(0)
{
}

CAnimation2D::~CAnimation2D()
{
}



void CAnimation2D::finalupdate()
{

	if (m_bFinish)
		return;

	m_fAccTime += DT;

	if (m_vecFrm[m_iCurFrmIdx].fDuration < m_fAccTime)
	{
		m_fAccTime -= m_vecFrm[m_iCurFrmIdx].fDuration;
		
		if (m_vecFrm.size() - 1 <= m_iCurFrmIdx)
		{
			m_bFinish = true;
		}
		else
		{
			++m_iCurFrmIdx;
		}		
	}
}

void CAnimation2D::UpdateData()
{

	static CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::ANIM2D);

	tAnim2D info = {};
	info.useAnim2D = 1;
	info.Atlas_Width = m_pAtlasTex->Width();
	info.Atlas_Height = m_pAtlasTex->Height();

	info.vBackgroundSize = m_vBackgroundSize;
	info.vLT = m_vecFrm[m_iCurFrmIdx].vLT;	
	info.vSlice = m_vecFrm[m_iCurFrmIdx].vSlice;
	info.vOffset = m_vecFrm[m_iCurFrmIdx].vOffset;

	pBuffer->SetData(&info, sizeof(tAnim2D));
	pBuffer->UpdateData(); // b2


	m_pAtlasTex->UpdateData((int)PIPELINE_STAGE::PS, 10);

}

void CAnimation2D::Create(Ptr<CTexture> _Atlas, Vec2 _vBackgroundSizePixel, Vec2 _vLT, Vec2 _vSlice, Vec2 _vStep
	, float _fDuration, int _iFrameCount)
{
	assert(_Atlas.Get());

	m_pAtlasTex = _Atlas;
	
	float fWidth = m_pAtlasTex->Width();
	float fHeight = m_pAtlasTex->Height();

	// 픽셀 좌표를 0~1 UV 로 전환
	Vec2 vLT = _vLT / Vec2(fWidth, fHeight);
	Vec2 vSlice = _vSlice / Vec2(fWidth, fHeight);
	Vec2 vStep = _vStep / Vec2(fWidth, fHeight);

	m_vBackgroundSize = _vBackgroundSizePixel / Vec2(fWidth, fHeight);

	// 프레임 정보 생성
	for (int i = 0; i < _iFrameCount; ++i)
	{
		tAnim2DFrame frm = {};
		
		frm.vLT = vLT + (vStep * (float)i);
		frm.vSlice = vSlice;
		frm.fDuration = _fDuration;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation2D::Create(Ptr<CTexture> _Atlas, vector<tAnim2DFrame> _vecFrm, Vec2 _vBackgroundSizePixel)
{
	m_pAtlasTex = _Atlas;
	m_vecFrm = _vecFrm;

	float fWidth = m_pAtlasTex->Width();
	float fHeight = m_pAtlasTex->Height();

	m_vBackgroundSize = _vBackgroundSizePixel / Vec2(fWidth, fHeight);
	


}

// 프레임의 행과 열의 개수를 추가해서 
// 세로 프레임까지 이어서 Frame 을 추가
void CAnimation2D::Create(Ptr<CTexture> _Atlas, Vec2 _vBackgroundSizePixel,
	Vec2 _vLT, Vec2 _vSlice, Vec2 _vStep, float _fDuration
	, int _iFrameCount, int RowNum, int ColumnNum)
{
	assert(_Atlas.Get());

	m_pAtlasTex = _Atlas;

	float fWidth = m_pAtlasTex->Width();
	float fHeight = m_pAtlasTex->Height();

	// 픽셀 좌표를 0~1 UV 로 전환
	Vec2 vLT = _vLT / Vec2(fWidth, fHeight);
	Vec2 vSlice = _vSlice / Vec2(fWidth, fHeight);
	Vec2 vStep = _vStep / Vec2(fWidth, fHeight);

	m_vBackgroundSize = _vBackgroundSizePixel / Vec2(fWidth, fHeight);

	// 프레임 정보 생성
	tAnim2DFrame frm = {};


	for (int i = 0; i < RowNum; ++i)
	{

		int colNum = ColumnNum;
		if (_iFrameCount < ColumnNum)
			colNum = _iFrameCount;
		for (int j = 0; j < colNum; ++j)
		{

			frm = {};
			frm.vLT.y += vLT.y + (vStep.y * (float)i);
			frm.vLT.x = vLT.x + (vStep.x * (float)j);
			frm.vSlice = vSlice;
			frm.fDuration = _fDuration;

			m_vecFrm.push_back(frm);
		}

		_iFrameCount -= ColumnNum;
	}



}

void CAnimation2D::SaveToFile(FILE* _pFile)
{
	CEntity::SaveToScene(_pFile);

	// ============== m_vecFrm ===================
	tAnim2DFrame frm;
	int vecFrmCnt = m_vecFrm.size();
	fwrite(&vecFrmCnt, sizeof(int), 1, _pFile);

	for (int i = 0; i < m_vecFrm.size(); ++i)
	{
		frm = m_vecFrm[i];
		fwrite(&frm.vLT, sizeof(Vec2), 1, _pFile);
		fwrite(&frm.vSlice, sizeof(Vec2), 1, _pFile);
		fwrite(&frm.vOffset, sizeof(Vec2), 1, _pFile);
		fwrite(&frm.fDuration, sizeof(Vec2), 1, _pFile);
	}

	// Texture Key 값만 저장 
	SaveWStringToFile(m_pAtlasTex->GetKey(), _pFile);
	fwrite(&m_vBackgroundSize, sizeof(Vec2), 1, _pFile);

	fwrite(&m_iCurFrmIdx, sizeof(int), 1, _pFile);
	fwrite(&m_fAccTime, sizeof(float), 1, _pFile);
	fwrite(&m_bFinish, sizeof(bool), 1, _pFile);


	fwrite(&m_iHorizontal_Flip, sizeof(int), 1, _pFile);
	fwrite(&m_iVertical_Flip, sizeof(int), 1, _pFile);

}

void CAnimation2D::LoadFromFile(FILE* _pFile, bool IsPrevRead)
{
	CEntity::LoadFromScene(_pFile);

	int vecFrmCnt = m_vecFrm.size();
	fread(&vecFrmCnt, sizeof(int), 1, _pFile);
	tAnim2DFrame frm;

	for (int i = 0; i < vecFrmCnt; ++i)
	{
		fread(&frm.vLT, sizeof(Vec2), 1, _pFile);
		fread(&frm.vSlice, sizeof(Vec2), 1, _pFile);
		fread(&frm.vOffset, sizeof(Vec2), 1, _pFile);
		fread(&frm.fDuration, sizeof(Vec2), 1, _pFile);
		m_vecFrm.push_back(frm);

	}
	
	wstring key;
	LoadWStringFromFile(key, _pFile);
	m_pAtlasTex = CResMgr::GetInst()->FindRes<CTexture>(key);

	fread(&m_vBackgroundSize, sizeof(Vec2), 1, _pFile);

	fread(&m_iCurFrmIdx, sizeof(int), 1, _pFile);
	fread(&m_fAccTime, sizeof(float), 1, _pFile);
	fread(&m_bFinish, sizeof(bool), 1, _pFile);

	fread(&m_iHorizontal_Flip, sizeof(int), 1, _pFile);
	fread(&m_iVertical_Flip, sizeof(int), 1, _pFile);
}
