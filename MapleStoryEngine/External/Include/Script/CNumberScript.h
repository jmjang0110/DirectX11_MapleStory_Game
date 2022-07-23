#pragma once
#include <Engine/CScript.h>

enum class NUMBER_TYPE
{
    ITEM,
    MESO,
    SHIP_TIMER,

    END,

};

class CTexture;


class CNumberScript :
    public CScript
{
private:
    float           m_fTimer;

private:
    Vec2            m_vStartLT;
    Vec2            m_vSlice;

    Ptr<CTexture>   m_pTex;
    UINT            m_iNum;

    NUMBER_TYPE     m_eType;

    vector<UINT>    m_vecNum;

public:
    void Init(NUMBER_TYPE _type = NUMBER_TYPE::ITEM, Vec2 _startLT = Vec2(0.f, 0.f), Vec2 _slice = Vec2(0.f, 0.f));
    void UpdateNumbber(UINT _num);


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CNumberScript);


public:
    CNumberScript();
    CNumberScript(const CNumberScript& _origin);
    ~CNumberScript();

    friend class CInventoryScript;

};

