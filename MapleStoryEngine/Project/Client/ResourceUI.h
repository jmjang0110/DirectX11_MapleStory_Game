#pragma once
#include "UI.h"

class TreeUI;

class ResourceUI :
    public UI
{
private:
    TreeUI* m_TreeUI;

    vector<wstring> m_vecResPath;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();
private:
    void Reload();
    void FindFileName(const wstring& _strFolderPath);

    void ItemClicked(DWORD_PTR _dwNode);
    void ItemDBClicked(DWORD_PTR _dwNode);

    RES_TYPE GetResTypeFromExt(const wstring& _strExt);



public:
    ResourceUI();
    ~ResourceUI();
};

