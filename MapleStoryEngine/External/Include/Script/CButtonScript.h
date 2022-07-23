#pragma once
#include <Engine/CScript.h>

typedef void (CScript::* CLIKCED)(DWORD_PTR _param);

class CButtonScript :
    public CScript
{

private:
    bool    m_bEnable;

    // Click
    CScript* m_pInst;
    CLIKCED         m_BCEvent;


public:
    void UpdateButton(bool _bEnable);
    void SetEnable(bool _B) { m_bEnable = _B; }

    void SetBtnClickedFunc(CScript* _pScript, CLIKCED _bcFunc)
    {
        m_pInst = _pScript;
        m_BCEvent = _bcFunc;

    }

    void Show(bool _b);

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CButtonScript);


public:
    CButtonScript();
    CButtonScript(const CButtonScript& _origin);
    ~CButtonScript();


};

