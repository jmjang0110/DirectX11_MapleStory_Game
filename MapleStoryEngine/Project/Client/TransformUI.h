#pragma once
#include "ComponentUI.h"


class TransformUI :
    public ComponentUI
{
private:
    Vec3 m_vRelativePos;
    Vec3 m_vRelativeScale;
    Vec3 m_vRelativeRot;

    bool m_bIgnorantParentScale;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    TransformUI();
    ~TransformUI();
};

