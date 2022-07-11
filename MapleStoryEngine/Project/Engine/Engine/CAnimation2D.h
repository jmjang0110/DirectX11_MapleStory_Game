#pragma once

#include "CEntity.h"

#include "CTexture.h"

class CAnimator2D;
 
class CAnimation2D
	: public CEntity
{
private:
	vector<tAnim2DFrame>	m_vecFrm;
	Ptr<CTexture>			m_pAtlasTex;
	Vec2					m_vBackgroundSize;

	CAnimator2D*			m_pOwner;
	int						m_iCurFrmIdx;
	float					m_fAccTime;
	bool					m_bFinish;

	// ======================= Flip
private:
	int                         m_iHorizontal_Flip;     // 1 : yes 0 : No
	int                         m_iVertical_Flip;       // 1 : yes 0 : No

public:
	void SetHorizontalFlip(int _horizonFlip) 
	{  
		m_iHorizontal_Flip = _horizonFlip; 

	}
	void SetVerticalFlip(int _verticalFlip) {  m_iVertical_Flip = _verticalFlip; }

	int GetHorizontalFlip() { return m_iHorizontal_Flip; }
	int GetVerticalFlip() { return m_iVertical_Flip; }

	// ======================= Flip

public:
	tAnim2DFrame& GetFrame(int _iFrmIdx) { return m_vecFrm[_iFrmIdx]; }
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }

	// ============
	Ptr<CTexture> GetTex() { return m_pAtlasTex; }

	void Create(Ptr<CTexture> _Atlas
		, vector<tAnim2DFrame>	_vecFrm
		, Vec2 _vBackgroundSizePixel);

	// ============
public:
	void finalupdate();

	virtual void UpdateData() override;

public:
	void Create(Ptr<CTexture> _Atlas, Vec2 _vBackgroundSizePixel
		, Vec2 _vLT, Vec2 _vSlice, Vec2 _vStep
		, float _fDuration, int _iFrameCount);
	bool IsFinish() { return m_bFinish; }
	void Reset()
	{
		m_fAccTime = 0.f;
		m_bFinish = false;
		m_iCurFrmIdx = 0;			
	}


	// ================================ 
public:
	// 프레임의 행과 열 개수를 추가한다. 
	void Create(Ptr<CTexture> _Atlas, Vec2 _vBackgroundSizePixel, Vec2 _vLT, Vec2 _vSlice,
		Vec2 _vStep, float _fDuration, int _iFrameCount, int RowNum, int ColumnNum);






public:
	virtual void SaveToScene(FILE* _pFile) override;
	virtual void LoadFromScene(FILE* _pFile) override;
	CLONE(CAnimation2D)

public:
	CAnimation2D();
	~CAnimation2D();

	friend class CAnimator2D;
};