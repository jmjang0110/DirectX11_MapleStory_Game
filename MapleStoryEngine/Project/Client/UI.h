#pragma once


#include "ImGui/imgui.h"

class UI
{
private:
	const string	m_strName;
	string			m_strTitle;
	vector<UI*>		m_vecChildUI;
	UI* m_pParentUI;

	Vec2			m_vPos;
	Vec2			m_vSize;
	bool			m_bOpen;
	bool			m_bModal;

public:
	const string& GetName() { return m_strName; }
	void SetSize(Vec2 _vSize) { m_vSize = _vSize; }
	Vec2 GetSize() { return m_vSize; }

	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	Vec2 GetPos() { return m_vPos; }

	void SetTitle(const string& _strTitle) { m_strTitle = _strTitle; }
	const string& GetTitle() { return m_strTitle; }

	void SetModalUI(bool _bTrue) { m_bModal = _bTrue; }
	bool IsModal() { return m_bModal; }

	UI* GetParentUI() { return m_pParentUI; }

	void AddChild(UI* _pChildUI)
	{
		_pChildUI->m_pParentUI = this;
		m_vecChildUI.push_back(_pChildUI);
	}

	// ======== Todo =============
	void DeleteChild(UI* _pChildUI)
	{
		for (int i = 0; i < m_vecChildUI.size(); ++i)
		{
			if (m_vecChildUI[i] == _pChildUI)
			{
				SAFE_DELETE(_pChildUI);
				m_vecChildUI.erase(m_vecChildUI.begin() + i);
				return;
			}
		}
	}

	virtual void Activate() { m_bOpen = true; }
	virtual void Deactivate() { m_bOpen = false; };

	bool IsActive() { return m_bOpen; }




public:
	virtual void update();
	virtual void render();
	virtual void render_update() = 0;

public:
	UI(const string& _strName);
	virtual ~UI();
};

