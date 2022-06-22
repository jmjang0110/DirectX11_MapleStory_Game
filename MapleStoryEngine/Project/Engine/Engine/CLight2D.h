#pragma once
#include "CComponent.h"

class CLight2D :
    public CComponent
{
private:
    tLightInfo  m_LightInfo;
    int         m_iLightIdx;


public:
    const tLightInfo& GetLightInfo() { return m_LightInfo; }

    void SetDiffuse(Vec3 _vDiffuse) { m_LightInfo.color.vDiff = _vDiffuse; }
    void SetSpecular(Vec3 _vSpec) { m_LightInfo.color.vSpec = _vSpec; }
    void SetAmbient(Vec3 _vAmb) { m_LightInfo.color.vAmb = _vAmb; }
    void SetLightDir(Vec3 _vDir) { m_LightInfo.vLightDir = _vDir; }
    void SetRange(float _fRange) { m_LightInfo.fRange = _fRange; }
    void SetAngle(float _fAngle) { m_LightInfo.fAngle = _fAngle; }
    void SetLightType(LIGHT_TYPE _eType) { m_LightInfo.iLightType = (int)_eType; }

public:
    virtual void update() override;
    virtual void finalupdate() override;


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CLight2D);
public:
    CLight2D();
    ~CLight2D();
};

