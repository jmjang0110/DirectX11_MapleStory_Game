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

enum class ITEM_SPOT
{
    DEFAULT,
    LAND,
    JUMP,
    INVENTORY,
    QUICKSLOT,
    END,

};
class CItemScript :
    public CScript
{
public:
    ITEM_SPOT               m_eSpotType; 
    bool                    m_bOnGround;

private:
    class CQuickSlotScript* m_pQSAddress;
    wstring                 m_EnterQuickSlotName;
    bool                    m_bQuickSlot;
    class CGameObject*      m_pClone_QS; // Address of Clone Obj In QuickSlot 
    class CGameObject*      m_pProto;

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

private:
    float               m_fEatTimer; // Player 가 먹었을 떄 
    bool                m_bEaten;    // 플레이어가 먹음 

    Ptr<CSound>         m_pBgm;
    Ptr<CSound>         m_pPickUpItemBgm;

    bool m_pPickUpItemBgm_Start = false;



private:
    CGameObject* m_pInfoObjADdress;


public:
    void setItemType(ITEM_TYPE _etype, CONSUME_TYPE _eCtype = CONSUME_TYPE::NONE) { m_eType = _etype; m_Consume_Type = _eCtype; }
    void SetHpRaise(float _f) { m_fHpRaise = _f; }
    void SetMpRaise(float _f) { m_fMpRaise = _f; }

    ITEM_TYPE GetItemType() { return m_eType; }
    CONSUME_TYPE GetItemConsumeType() { return m_Consume_Type; }
    bool GetOnGround() { return m_bOnGround; }
    int GetCnt() { return m_iCnt; }

public:
    void AddCnt();
    void SubCnt();
    void SetCnt(int _i) { m_iCnt = _i; }

public:
    void SetQuickSlotName(wstring _name) { m_EnterQuickSlotName = _name; }
    wstring GetQuickSlotName() { return m_EnterQuickSlotName; }

    void SetIsEnterQuickSlot(bool _b) { m_bQuickSlot = _b; }
    bool GetIsEnterQuickSlot() { return m_bQuickSlot; }

    void SetCloneObjAddress(CGameObject* _pObj) { m_pClone_QS = _pObj; }

public:
    void UseThisItem();
    void CheckItemCnt();
    void DeleteThisItem();


    void NotifyToInventory();


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;


    CLONE(CItemScript);


public:
    CItemScript();
    CItemScript(const CItemScript& _origin);
    ~CItemScript();

    friend class CInventoryScript;
    friend class CPlayerScript;
    friend class CQuickSlotScript;
    friend class CGravityScript;
    friend class CRigidBodyScript;


};

