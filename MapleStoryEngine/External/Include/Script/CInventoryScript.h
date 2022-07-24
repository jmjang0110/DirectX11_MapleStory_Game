#pragma once
#include <Engine/CScript.h>
#include "CItemScript.h"

class CGameObject;

#define INVENTORY_ROW 8
#define INVENTORY_COL 16

struct ItemInven
{
    CGameObject* obj;
    CGameObject* Num;

    UINT         num = 1;

    UINT         row = 1;
    UINT         col = 1;

    Vec2         RelativePos;

    // 정확한 위치를 건내 받지 않더라도 가장 가까운 곳에 아이템 배치 
    void SetRowCol(Vec2 _pos)
    {
        RelativePos = _pos;
    }


    //ItemInven() {};
    //~ItemInven() { SAFE_DELETE(obj); }
};


class CInventoryScript :
    public CScript
{
private:
    CPrefab* m_pNumPrefab;

private:
    ITEM_TYPE       m_eEnabledTabType;
    Vec2            m_vStartPos;
    Vec2            m_vInterval;

private:
    bool            m_bActivate;
    bool            m_bMove;


private:
    // move
    Vec3            m_Cursor_StartPos;
    Vec3            m_Cursor_Diff;
    Vec3            m_Cursor_CurPos;
    Vec3            m_Cursor_PrevDiff;

private:
    map<wstring, ItemInven*>  m_Equip;
    map<wstring, ItemInven*>  m_Consume;
    map<wstring, ItemInven*>  m_Etc;

private:
    class CButtonScript* m_pEquipBtnScript;
    class CButtonScript* m_pConsumeBtnScript;
    class CButtonScript* m_pEtcBtnScript;

private:
    // Check Empty
    bool m_EquipSpot[INVENTORY_ROW][INVENTORY_COL] = { false, };          // false : empty / true : Exist
    bool m_ConsumeSpot[INVENTORY_ROW][INVENTORY_COL] = { false, };
    bool m_EtcSpot[INVENTORY_ROW][INVENTORY_COL] = { false, };


public:
    // _param : nullptr
    void ShowEquip(DWORD_PTR _param);
    void ShowConsume(DWORD_PTR _param);
    void ShowEtc(DWORD_PTR _param);

    void ShowOffAllItem();





public:
    void PushItem(wstring _name, ITEM_TYPE _type, CGameObject* _item);
    void DeleteItem(ITEM_TYPE _Type, int row, int col);

    void SetItemType(ITEM_TYPE _type) { m_eEnabledTabType = _type; }
    bool GetActivate() { return m_bActivate; }
    void SetMove(bool _B) { m_bMove = _B; }


    void Show(bool _b);



public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CInventoryScript);


public:
    CInventoryScript();
    CInventoryScript(const CInventoryScript& _origin);
    ~CInventoryScript();


    friend class CItemScript;

private:
    //CGameObject* m_Equip[INVENTORY_ROW][INVENTORY_COL] = { nullptr,};
    //CGameObject* m_Consume[INVENTORY_ROW][INVENTORY_COL] = { nullptr, };;
    //CGameObject* m_Etc[INVENTORY_ROW][INVENTORY_COL] = { nullptr, };;

//public:
//    void RegisterEquip();
//    void RegisterConsume();
//    void RegisterEtc();
//public:
//    void DeRegisterEquip();
//    void DeRegisterConsume();
//    void DeRegisterEtc();
private:
    //void AllDeregister(CGameObject* _deObj);
   // void AllRegister(CGameObject* _reObj);
};

