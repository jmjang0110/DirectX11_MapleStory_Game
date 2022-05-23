#pragma once


class CCore
	: public CSingleton<CCore>
{
	SINGLE(CCore);
private:
	HWND	m_hWnd;
	POINT	m_ptResolution;

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();
	HWND GetMainHwnd() { return m_hWnd; }

private:
	void Frame_Init();
	void Frame_Clear();
};

