#pragma once
#include <Engine/CScript.h>
class CLaunchSkillScript :
    public CScript
{

private:
    bool m_bOnGorund;


    bool m_UseGround; // true : ¶¥¿¡ ´ê¾ÒÀ» ¶§ false : ±×³É ¸ÂÀ¸¸é ÅÍÁü 
    bool m_bHit;

    float m_fTimer;

    bool m_bAttackSuccess;



public:
    float GetTime() { return m_fTimer; }


public:
    void UseGround(bool _b) { m_UseGround = _b; }
    void SetHit(bool _B) { m_bHit = _B; }

    void SetAttackSuccess(bool _b) { m_bAttackSuccess = _b; }
    bool GetATtackSuccess() { return m_bAttackSuccess; }

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


    CLONE(CLaunchSkillScript)


public:
    CLaunchSkillScript();
    CLaunchSkillScript(const CLaunchSkillScript& _origin);
    ~CLaunchSkillScript();

};

