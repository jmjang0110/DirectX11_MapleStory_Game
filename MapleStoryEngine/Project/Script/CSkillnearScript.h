#pragma once
#include <Engine/CScript.h>

/*
        * Only For "Near" Range Skill *
*/

class CGameObject;
class CPrefab;



class CSkillnearScript :
    public CScript
{
private:
    bool                m_bDelete;
    KEY                 m_eKey;

    CGameObject*        m_SkillObj;
    CGameObject*        m_pSkillUser; // Skill 을 사용하는 User

    int                 m_iAttackCnt;
    int                 m_iAttackMaxCnt;

    float               m_fMAxAttack;
    float               m_fMinAttack;

    CGameObject*        m_MyHitObjAddress;
    float               m_fTimer;       // damage 출력 타이밍 

    int                 m_iHitMonsterCnt; 
    int                 m_iHitMaxMonsterCnt;
    bool                m_bHitPossible;

    map<int, CGameObject*> m_mapHitMob;


private:
    Ptr<CSound>         m_pUseBgm;
    Ptr<CSound>         m_pHitBgm;


public:
    void PlayHItBgm(int _iroopCnt, float volume, bool overlap) { m_pHitBgm->Play(_iroopCnt, volume, overlap); }

public:
    void AddAttackCnt() { if (m_iAttackMaxCnt >= m_iAttackCnt) m_iAttackCnt += 1; }
    int     GetAttackCnt() { return m_iAttackCnt; }
    int     GetMaxAttackCnt() { return m_iAttackMaxCnt; }
    void SetMaxAttackCnt(int _i) { m_iAttackMaxCnt = _i; }


    bool IsAddMonsterHitPossible() { if (m_iHitMonsterCnt < m_iHitMaxMonsterCnt) return true; else return false; }
    void AddHitMonster(CGameObject* _p) { m_mapHitMob.insert(make_pair(m_iHitMonsterCnt, _p)); }
    void AddMonsterCnt() { if (m_iHitMonsterCnt < m_iHitMaxMonsterCnt) m_iHitMonsterCnt++; }
    int GetMonsterCnt() { return m_iHitMonsterCnt; }
    CGameObject* GetHitMonseterObjAddress(int _hitNum);
    void SetHitMaxMonsterCnt(int _Cnt) { m_iHitMaxMonsterCnt = _Cnt; }


    void SetMaxAttack(float _f) { m_fMAxAttack = _f; }
    void SetMinAttack(float _f) { m_fMinAttack = _f; }
    float GetMaxAttack() { return m_fMAxAttack; }
    float GetMinAttack() { return m_fMinAttack; }

    void SetHitObjAddress(CGameObject* _obj) { m_MyHitObjAddress = _obj; }
    CGameObject* GetHitObjAddress() { return m_MyHitObjAddress; }

    float   GetTimer() { return m_fTimer; }


public:
    void SetUser(CGameObject* _pUser) { m_pSkillUser = _pUser; }
    void SetKey(KEY _key) { m_eKey = _key; }
    void SetSkillObjByPrefab(CPrefab* _Pref = nullptr, wstring _SkillName = L"defaultSkill");


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


    CLONE(CSkillnearScript)


public:
    CSkillnearScript();
    CSkillnearScript(const CSkillnearScript& _origin);
    ~CSkillnearScript();


};

