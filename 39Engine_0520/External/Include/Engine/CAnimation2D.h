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


public:
	tAnim2DFrame& GetFrame(int _iFrmIdx) { return m_vecFrm[_iFrmIdx]; }
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }

public:
	void finalupdate();

	virtual void UpdateData() override;

public:
	void Create(Ptr<CTexture> _Atlas, Vec2 _vBackgroundSizePixel, Vec2 _vLT, Vec2 _vSlice, Vec2 _vStep, float _fDuration, int _iFrameCount);
	bool IsFinish() { return m_bFinish; }
	void Reset()
	{
		m_fAccTime = 0.f;
		m_bFinish = false;
		m_iCurFrmIdx = 0;			
	}


public:
	CLONE(CAnimation2D)

public:
	CAnimation2D();
	~CAnimation2D();

	friend class CAnimator2D;
};