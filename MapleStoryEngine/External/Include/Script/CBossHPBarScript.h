#pragma once
#include <Engine/CScript.h>
#include "CBossNPCScript.h"


class CBossHPBarScript :
    public CScript
{
private:
    class CGameObject* m_pSubObj;
    class CGameObject* m_pBoss;

    // SIZE  |  min : (0, 10 ) -> max ( 758 , 10 )  |
    float              m_fMaxHPsize;
    float              m_fCurHP;

    bool m_bDie;

    // 
    class CBossNPCScript* m_pBossNPC;


public:
    void SetBossNPCAddress(class CBossNPCScript* _pBossNPCscipt) { m_pBossNPC = _pBossNPCscipt; }

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CBossHPBarScript);


public:
    CBossHPBarScript();
    CBossHPBarScript(const CBossHPBarScript& _origin);
    ~CBossHPBarScript();



};

