#pragma once
#include "UI.h"

typedef void (UI::* DBCLKED)(DWORD_PTR _param);

class ListUI :
    public UI
{
private:
    vector<string>  m_vecList;

    // 더블클릭 이벤트   
    UI* m_Inst;
    DBCLKED         m_DBCEvent;

public:
    void AddList(const string& _str) { m_vecList.push_back(_str); }
    void AddList(vector<string>& _vecStr) { m_vecList.insert(m_vecList.cend(), _vecStr.begin(), _vecStr.end()); }
    void Clear() { m_vecList.clear(); }

    void SetDBCEvent(UI* _pUI, DBCLKED _func)
    {
        m_Inst = _pUI;
        m_DBCEvent = _func;
    }


public:
    virtual void update() override;
    virtual void render_update() override;
    virtual void Activate() override;

public:
    ListUI();
    ~ListUI();
};

