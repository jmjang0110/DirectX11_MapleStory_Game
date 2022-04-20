#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D* m_pCurAnim;

    bool                        m_bRepeat;

public:
    virtual void finalupdate() override;
    virtual void UpdateData() override;
    static void Clear();

public:
    CAnimation2D* FindAnim(const wstring& _strName);

    /*
        CreateAnim 매개변수 설명 
        1. 이름 2. Texture 정보 3. 첫번째 프레임 좌상단 위치
        4. 좌상단 위치로 부터 영역 5. 다음 프레임 위치 간격 
        6. 총 몇장 7. 프레임별 유지 시간 8. 
    */
    void CreateAnim(const wstring& _strName
        , Ptr<CTexture> _pAtlas, Vec2 _vLeftTopPixel
        , Vec2 _vSlicePixel, Vec2 _vStepPixel
        , float _fDuration, int _iFrameCount);

    void Play(const wstring& _strName, bool _bRepeat);

public:
    CLONE(CAnimator2D)

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _origin);
    ~CAnimator2D();
};

