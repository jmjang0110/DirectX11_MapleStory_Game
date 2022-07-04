#pragma once
#include "CRes.h"

class CLayer;

class CLayerFile :
    public CRes
{
public:
    virtual int Load(const wstring& _strFilePath) override;

public:
    CLayerFile();
    ~CLayerFile();
};

