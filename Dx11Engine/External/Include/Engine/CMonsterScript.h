#pragma once
#include "CScript.h"

enum class ACT_TYPE
{
    DIE,
    HIT,
    MOVE,
    STAND,
    ATTACK1,
    ATTACK2,
    SKILL1,
    SKILL2,
    END,

};

static const wchar_t* acString[(UINT)ACT_TYPE::END] =
{
    L"DIE",
    L"HIT",
    L"MOVE",
    L"STAND",
    L"ATTACK1",
    L"ATTACK2",
    L"SKILL1",
    L"SKILL2",
};

enum class ACT_STRING_TYPE
{

              
};



class CMonsterScript :
    public CScript
{
private:
    float       m_fSpeed;
    float       m_fAccTime;

    int         m_MoveDir;
    float       m_fMoveDist;


    ACT_TYPE    m_CurActType;
    bool        m_bMyActType[(UINT)ACT_TYPE::END];


public:
    virtual void update();

    void OnCollisionEnter(CGameObject* _pOtherObj) override;


public:
    void SetCurActType(ACT_TYPE _actType);



    CLONE(CMonsterScript);
public:
    CMonsterScript();
    ~CMonsterScript();
};

