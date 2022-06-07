#pragma once

#include "UI.h"

#include <Engine/CRes.h>

class ResInfoUI :
    public UI
{
private:
    CRes* m_pRes;     // 정보를 출력할 리소스
    const RES_TYPE      m_eResType;    // 해당 리소스 타입

public:
    void SetTargetRes(CRes* _pRes) { m_pRes = _pRes; }
    CRes* GetTargetRes() { return m_pRes; }

public:
    virtual void render_update() override;

public:
    ResInfoUI(const string& _strName, RES_TYPE _eType);
    virtual ~ResInfoUI();
};

