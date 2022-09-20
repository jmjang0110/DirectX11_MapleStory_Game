#pragma once
#include <Engine/CScript.h>
class CBossNPCScript :
    public CScript
{
private:
    int     m_iClickedCnt;
    float   m_fTimer;

    bool    m_bCreateBossMonster;

    bool m_bEndBossMonster;
    float m_fChangeSceneTimer;


public:
    void SetEndBossMonster(bool _B) { m_bEndBossMonster = _B; }


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


    CLONE(CBossNPCScript)


public:
    CBossNPCScript();
    CBossNPCScript(const CBossNPCScript& _origin);
    ~CBossNPCScript();


};

