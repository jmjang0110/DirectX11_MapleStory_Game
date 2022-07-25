#pragma once
#include <Engine/CScript.h>


class CMobGroundScript :
    public CScript
{
private:
    Vec3 m_vScale;
    Vec3 m_vPos;

    UINT m_iMaxMonsterCnt;
    UINT m_iCurMosterCnt;



public:
    Vec3 GetScale() { return m_vScale; }
    Vec3 GetPos() { return m_vPos; }

    UINT GetMaxMonsterCnt() { return m_iMaxMonsterCnt; }
    UINT GetCurMonsterCnt() { return m_iCurMosterCnt; }
    UINT GetPossibleCreateCnt() { return m_iMaxMonsterCnt - m_iCurMosterCnt; } // 생성 가능 개수 

    void SubMonsterCnt() { if (m_iCurMosterCnt -1 >= 0) m_iCurMosterCnt--; }
    void AddMonsterCnt() { if (m_iCurMosterCnt + 1 <= m_iMaxMonsterCnt) m_iMaxMonsterCnt++; }
    void AddMonsterCnt(int _cnt) { if (m_iCurMosterCnt + _cnt <= m_iMaxMonsterCnt) m_iCurMosterCnt += _cnt; }

    void SetMaxMonsterCnt(UINT _num) { m_iMaxMonsterCnt = _num; }


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


    CLONE(CMobGroundScript)


public:
    CMobGroundScript();
    CMobGroundScript(const CMobGroundScript& _origin);
    ~CMobGroundScript();


};

