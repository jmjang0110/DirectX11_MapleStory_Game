#pragma once
#include <Engine/CScript.h>




enum class ITEM_TYPE
{
    NONE,
    EQUIP,
    CONSUME,
    ETC,
    END,

};

enum class CONSUME_TYPE
{
    MP,         // HP Raise
    HP,         // MP Raise
    HPMP,       // HP/ MP Raise
    ATTACK,     // Attack Raise
    NONE,


    END,


};
class CItemScript :
    public CScript
{

private:
    ITEM_TYPE       m_eType;
    CONSUME_TYPE    m_Consume_Type;


    UINT            m_iClickedCnt;
    float           m_fTimer;

private:
    float           m_fHpRaise;
    float           m_fMpRaise;
    float           m_fAttackRaise;

    int             m_iCnt;

private:
    int             m_iInventoryIdx_row;
    int             m_iInventoryIdx_col;

private:
    // move
    Vec3            m_Cursor_StartPos;
    Vec3            m_Cursor_Diff;
    Vec3            m_Cursor_CurPos;
    Vec3            m_Cursor_PrevDiff;
    
    bool            m_bCollide_Cursor;

public:
    void setItemType(ITEM_TYPE _etype, CONSUME_TYPE _eCtype = CONSUME_TYPE::NONE){ m_eType = _etype; m_Consume_Type = _eCtype;}
    void SetHpRaise(float _f) { m_fHpRaise = _f; }
    void SetMpRaise(float _f) { m_fMpRaise = _f; }

    ITEM_TYPE GetItemType() { return m_eType; }
    CONSUME_TYPE GetItemConsumeType() { return m_Consume_Type; }

public:
    void AddCnt();
    void SubCnt();



public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override; 
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CItemScript);


public:
    CItemScript();
    CItemScript(const CItemScript& _origin);
    ~CItemScript();

    friend class CInventoryScript;
    friend class CPlayerScript;


};

