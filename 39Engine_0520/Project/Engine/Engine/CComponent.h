#pragma once
#include "CEntity.h"

#include "CGameObject.h"

#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return GetOwner()->Type(); }


class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE    m_eComType;
    CGameObject*            m_pOwner;

    bool                    m_bActive;

public:
    virtual void start() {}
    virtual void update() {}
    virtual void lateupdate() {}
    virtual void finalupdate() = 0;   

    void Activate();
    void Deactivate();

    bool IsActive() { return m_bActive; }

public:
    COMPONENT_TYPE GetType() { return m_eComType; }
    CGameObject* GetOwner() { return m_pOwner; }

    GET_OTHER_COMPONENT(Transform)
    GET_OTHER_COMPONENT(MeshRender)
    GET_OTHER_COMPONENT(Camera)
    GET_OTHER_COMPONENT(Collider2D)
    GET_OTHER_COMPONENT(Animator2D)
    GET_OTHER_COMPONENT(TileMap)
    GET_OTHER_COMPONENT(ParticleSystem)

    CComponent* Clone() = 0;

protected:
    virtual void active() { m_bActive = true; };
    virtual void deactive() { m_bActive = false; };


public:
    CComponent(COMPONENT_TYPE _eType);
    virtual ~CComponent();

    friend class CGameObject;
    friend class CEventMgr;
};

