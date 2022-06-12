#pragma once
#include "CScript.h"
class CTilemapEditorScript :
    public CScript
{
private:
    bool            m_bEditorMode;


public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();


public:
    void SetEditorMode(bool _b) { m_bEditorMode = _b; }
    bool GetEditorMode() { return m_bEditorMode; }


    // ======== File ============
public:
    virtual void SaveToFile(FILE* _pFile);
    virtual void LoadFromFile(FILE* _pFile);

    // ======== File ============


CLONE(CTilemapEditorScript)


public:
    CTilemapEditorScript();
    ~CTilemapEditorScript();
};

