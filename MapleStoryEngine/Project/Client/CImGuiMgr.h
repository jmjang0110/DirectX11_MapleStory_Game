#pragma once
// 함수 포인터를 typedef 로 선언했다. 
// 이경우 UI 의 정체를 정확히 알아야 하기 때문에 
// 전방선언을 하면 안먹히고 UI.h 를 직접 include 시켜야한다. 
#include "UI.h"


/*
	ImGUi 에 Delgegate 들을 등록시켜놓고 
	Progress() 중 마지막에 일괄처리 시키기 위해 
	만들었다. 

*/
typedef void (UI::* PARAM_1)(DWORD_PTR);

struct tUIDelegate
{
	UI* pInst;
	PARAM_1		pFunc;
	DWORD_PTR	dwParam;
};

class CImGuiMgr
	: public CSingleton<CImGuiMgr>
{
	SINGLE(CImGuiMgr)
private:
	map<string, UI*>	m_mapUI;
	vector<tUIDelegate>	m_vecDelegate;


	HANDLE				m_hNotify; //Content 파일 아래 변경사항 있을 시 이 핸들을 통해 파악 가능



public:
	void init(HWND _hwnd);

	void progress();
	void render();
	void clear();

private:
	void CreateUI();
	void ObserveContent();

public:
	UI* FindUI(const string& _strKey);
	void AddDelegate(tUIDelegate _del) { m_vecDelegate.push_back(_del); }


};

