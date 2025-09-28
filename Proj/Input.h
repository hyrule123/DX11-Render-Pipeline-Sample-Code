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

struct tKeyStatus
{
	eKey myKey;
	UINT8 State;	//Using Bit Flag

	tKeyStatus():
		myKey(),
		State((UINT8)0)
	{
	}
};

class Input
{
	friend class Manager;
private:
	Input();

	void init();

	~Input();

	Input(const Input& _other) = delete;
	Input(Input&& _move) = delete;

	void update();

private:
	std::vector<tKeyStatus> m_key_statuses;
	Vec2 m_cur_mouse_pos;
	Vec2 m_prev_mouse_pos;
	Vec2 m_mouse_dir;
	
public:
	bool GetKeyPressed(eKey _Key)	{ return (eKeyState::Pressed & m_key_statuses[_Key].State); }
	bool GetKeyTap(eKey _Key)		{ return (eKeyState::Tap & m_key_statuses[_Key].State); }
	bool GetKeyRelease(eKey _Key)	{ return (eKeyState::Release & m_key_statuses[_Key].State); }
	const Vec2& GetMouseDir() { return m_mouse_dir; }
};

