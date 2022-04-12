#pragma once

enum class KEY
{
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,

	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,

	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,

	NUM0,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,

	LSHFT,
	LALT,
	LCTRL,
	SPACE,
	ENTER,
	BACK,
	ESC,
	TAB,

	LBTN,
	RBTN,

	KEY_END,
};

enum class KEY_STATE
{
	TAP,		// 막 눌림
	PRESSED,	// TAP 이후에도 계속 눌려있음
	AWAY,		// 막 뗀 상황
	NONE,		// 안눌린 상태
};


struct tKeyInfo
{	
	KEY_STATE	eState;		// 현재 키 상태
	bool		bPrevCheck;	// 이전 프레임에서 키가 눌렸는지 체크
};

class CKeyMgr
	: public CSingleton<CKeyMgr>
{
	SINGLE(CKeyMgr);
private:
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_vMousePos;
	Vec2				m_vMousePrevPos;


public:
	void init();
	void update();

public:
	KEY_STATE GetKeyState(KEY _eKey)
	{
		return m_vecKey[(UINT)_eKey].eState;
	}

	Vec2 GetMousePos()
	{
		return m_vMousePos;
	}

	Vec2 GetMouseDir()
	{
		return (m_vMousePos - m_vMousePrevPos);
	}
};

