#pragma once
#include <Engine/CScript.h>
#include "CItemScript.h"


struct QuickSlotInfo
{
    ITEM_TYPE       eType; // Item Type / None : skill
    CGameObject*    pObj = nullptr; // �ּҰ��� ������ �ִ´�. 

};




class CQuickSlotScript :
    public CScript
{
    wstring m_wProtoName;

    
    CGameObject*    m_pProtoObj; // ���� 
    CGameObject*    m_pCloneObj; // ���纻 
    
    KEY             m_eKey;

    bool            m_bExistedItem;

public:
    void ClearObj() { m_pProtoObj = nullptr; m_pCloneObj = nullptr; }

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


    CLONE(CQuickSlotScript)


public:
    CQuickSlotScript();
    CQuickSlotScript(const CQuickSlotScript& _origin);
    ~CQuickSlotScript();

    friend class CItemScript;

};

