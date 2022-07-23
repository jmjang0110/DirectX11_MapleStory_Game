#pragma once
#include <Engine/CScript.h>
#include "CItemScript.h"

class CGameObject;

#define INVENTORY_ROW 8
#define INVENTORY_COL 16


class CInventoryScript :
    public CScript
{

private:
    ITEM_TYPE       m_eEnabledTabType;
    Vec2            m_vStartPos;
    Vec2            m_vInterval;

private:
    bool            m_bActivate;
    bool            m_bMove;


private:
    // move
    Vec3            m_Cursor_StartPos       ;
    Vec3            m_Cursor_Diff           ;
    Vec3            m_Cursor_CurPos         ;
    Vec3            m_Cursor_PrevDiff       ;


private:
    CGameObject* m_Equip[INVENTORY_ROW][INVENTORY_COL] = { nullptr,};
    CGameObject* m_Consume[INVENTORY_ROW][INVENTORY_COL] = { nullptr, };;
    CGameObject* m_Etc[INVENTORY_ROW][INVENTORY_COL] = { nullptr, };;

public:
    void RegisterEquip();
    void RegisterConsume();
    void RegisterEtc();
public:
    void DeRegisterEquip();
    void DeRegisterConsume();
    void DeRegisterEtc();
private:
    void AllDeregister(CGameObject* _deObj);
    void AllRegister(CGameObject* _reObj);



public:
    void PushItem(ITEM_TYPE _type, CGameObject* _item);
    void SetItemType(ITEM_TYPE _type) { m_eEnabledTabType = _type; }
    void UpdateButton(ITEM_TYPE _type);
    void Activate() { m_bActivate = true;  GetOwner()->Activate(); AllRegister(GetOwner()); }
    void Deactivate() { m_bActivate = false; GetOwner()->Deactivate(); AllDeregister(GetOwner()); }
    bool GetActivate() { return m_bActivate; }
    void SetMove(bool _B) { m_bMove = _B; }


    void DeleteItem(ITEM_TYPE _Type, int row, int col);



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


};

