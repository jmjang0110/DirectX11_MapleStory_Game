#pragma once

#include <Engine\CGameObject.h>

class CGameObjectEx :
    public CGameObject
{
public:
    void finalupdate();

public:
    CGameObjectEx();
    ~CGameObjectEx();
};

