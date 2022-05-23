#pragma once


#include "ImGui/imgui.h"

class UI
{
private:
	const string	m_strName;
	vector<UI*>		m_vecChildUI;
	UI*				m_pParentUI;

public:	
	const string& GetName() { return m_strName; }

public:
	virtual void update() = 0;
	void render();

	virtual void render_update() = 0;

public:
	UI(const string& _strName);
	~UI();
};

