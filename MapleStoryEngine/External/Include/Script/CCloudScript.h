#pragma once
#include <Engine/CScript.h>
class CCloudScript :
    public CScript
{
private:
    float       m_fSpeed;

    Vec3        m_vStartPos;
    Vec3        m_vEndPos;

    int         m_dir;


public:
    void SetSpeed(float _F) { m_fSpeed = _F; }
    float   GetSpeed() { return m_fSpeed; }
    void SetStartEndPos(Vec3 _vstart, Vec3 _vend, int _dir) { m_vStartPos = _vstart; m_vEndPos = _vend; m_dir = _dir; }

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    virtual void OnCollision(CGameObject* _OtherObject) override;
    virtual void OnCollisionExit(CGameObject* _OtherObject) override;


    CLONE(CCloudScript);


public:
    CCloudScript();
    CCloudScript(const CCloudScript& _origin);
    ~CCloudScript();



};

