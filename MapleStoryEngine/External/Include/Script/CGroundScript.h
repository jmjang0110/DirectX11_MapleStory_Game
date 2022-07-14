#pragma once
#include <Engine/CScript.h>

typedef struct Line {
    Vec2 p1;
    Vec2 p2;
    void Init(Vec2 _p1, Vec2 _p2) { p1 = _p1; p2 = _p2; }
}Line;

class CGroundScript :
    public CScript
{
private:
    float       m_fDiff;



public:
    bool    LineInterSection(Line l1, Line l2);
    int     CCW(Vec2 p1, Vec2 p2, Vec2 p3);
    int     Comparator(Vec2 Left, Vec2 Right);
    void    Swap(Vec2* p1, Vec2* p2);



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


    CLONE(CGroundScript)


public:
    CGroundScript();
    CGroundScript(const CGroundScript& _origin);
    ~CGroundScript();
};

