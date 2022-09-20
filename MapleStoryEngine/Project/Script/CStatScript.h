#pragma once
#include <Engine/CScript.h>
class CStatScript :
    public CScript
{
private:
    // move
    Vec3            m_Cursor_StartPos;
    Vec3            m_Cursor_Diff;
    Vec3            m_Cursor_CurPos;
    Vec3            m_Cursor_PrevDiff;

    bool            m_bMove;


private:
    CGameObject* m_HP;
    CGameObject* m_MP;
    CGameObject* m_ATT_MIN;
    CGameObject* m_ATT_MAX;
    CGameObject* m_STR;
    CGameObject* m_DEX;
    CGameObject* m_INT;
    CGameObject* m_LUK;



public:
    void InsertNumber(class CGameObject* _pObj, int num );


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


    CLONE(CStatScript)

public:
    CStatScript();
    CStatScript(const CStatScript& _origin);
    ~CStatScript();
};


