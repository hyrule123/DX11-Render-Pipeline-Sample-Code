#pragma once

#include "global.h"

//Used for index of g_arrVK
enum eKey : UINT16
{
	UP,
	DOWN,
	LEFT,
	RIGHT,

	W,
	A,
	S,
	D,

	
	MOUSE_LBUTTON,
	MOUSE_RBUTTON,

	END
};
extern int g_arrKey[(UINT16)eKey::END];

enum eKeyState : UINT8
{
	None = (UINT8)0,
	Tap = (UINT8)1 << (UINT8)0,
	Pressed = (UINT8)1 << (UINT8)1,
	Release = (UINT8)1 << (UINT8)2
};

struct tKeyInfo
{
	eKey myKey;
	UINT8 State;	//Using Bit Flag

	tKeyInfo():
		myKey(),
		State((UINT8)0)
	{
	}
};

class CKeyMgr
{
public:
	CKeyMgr();
	~CKeyMgr();
	void tick();

private:
	std::vector<tKeyInfo> m_vecKeyInfo;
	Vec2 m_MousePos;
	Vec2 m_PrevMousePos;
	Vec2 m_MouseDir;
	
public:
	bool GetKeyPressed(eKey _Key)	{ return (eKeyState::Pressed & m_vecKeyInfo[_Key].State); }
	bool GetKeyTap(eKey _Key)		{ return (eKeyState::Tap & m_vecKeyInfo[_Key].State); }
	bool GetKeyRelease(eKey _Key)	{ return (eKeyState::Release & m_vecKeyInfo[_Key].State); }
	const Vec2& GetMouseDir() { return m_MouseDir; }
};

