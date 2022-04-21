#pragma once
#include "CRenderComponent.h"
class CTileMap :
    public CRenderComponent
{


private:
    virtual void render() override;

    CLONE(TILEMAP);


public:
    CTileMap();
    ~CTileMap();


};

