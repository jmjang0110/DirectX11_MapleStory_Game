#pragma once
#include <Engine/Cscript.h>

enum class CURSOR_STATE
{
    DOWN,
    UP,
    END,
};


class CCursorScript :
    public CScript
{
private:
    CURSOR_STATE    m_CursorState;

private:
    Vec3            m_vPos;

public:
    Vec3 GetPos() { return m_vPos;}


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;

private:
    void Update_CursorPos();
    void Update_CursorState();
    void Update_CursorAnimation();

    CLONE(CCursorScript);
public:
    CCursorScript();   
    CCursorScript(const CCursorScript& _origin);
    ~CCursorScript();

};

