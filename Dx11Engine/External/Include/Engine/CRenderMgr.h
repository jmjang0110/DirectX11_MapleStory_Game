#pragma once
#include "singleton.h"



class CRenderMgr 
    : public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);

public:
    void init();
    void render();


};

