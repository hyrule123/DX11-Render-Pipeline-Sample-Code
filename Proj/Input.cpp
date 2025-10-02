#include "Input.h"

int g_arrKey[(UINT16)eKey::END]
{
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,

	'W',
	'A',
	'S',
	'D',

	VK_LBUTTON,
	VK_RBUTTON
};

extern HWND g_hWnd;
extern RECT WINSIZE;

Input::Input()
	: m_key_statuses()
	, m_cur_mouse_pos()
	, m_prev_mouse_pos()
	, m_mouse_dir()
{
}

void Input::init()
{
	m_key_statuses.resize(eKey::END);
	for (UINT16 i = 0; i < eKey::END; ++i)
	{
		m_key_statuses[i].myKey = (eKey)(UINT16)i;
	}
}

Input::~Input()
{
}

void Input::update()
{
	if (g_hWnd == GetFocus())
	{
		//Compute keyboard and mouse button status
		for (UINT16 i = 0; i < eKey::END; ++i)
		{
			//If key pressed
			if (GetAsyncKeyState(g_arrKey[i]) & 0x8000)
			{
				//It means pressed on this frame
				if (eKeyState::None == m_key_statuses[i].State)
				{
					m_key_statuses[i].State |= eKeyState::Tap;
					m_key_statuses[i].State |= eKeyState::Pressed;
				}
				
				//It means keep pressing the key
				else if(eKeyState::Pressed & m_key_statuses[i].State)
				{
					m_key_statuses[i].State &= ~eKeyState::Tap;
				}

			}
			else
			{
				//It means the key released right before
				if (eKeyState::Pressed & m_key_statuses[i].State)
				{
					m_key_statuses[i].State = eKeyState::Release;
				}
				else
				{
					m_key_statuses[i].State = eKeyState::None;
				}
			}
		}


		//Compute mouse position and direction
		m_prev_mouse_pos = m_cur_mouse_pos;

		POINT ptMousePos = {};
		GetCursorPos(&ptMousePos);

		ScreenToClient(g_hWnd, &ptMousePos);

		m_cur_mouse_pos.x = (float)ptMousePos.x;
		m_cur_mouse_pos.y = (float)ptMousePos.y;

		m_mouse_dir = m_cur_mouse_pos - m_prev_mouse_pos;
		m_mouse_dir.y *= -1.f;	
	}

	//If the window is not focused
	else
	{
		for (UINT16 i = 0; i < eKey::END; ++i)
		{
			if (eKeyState::Pressed & m_key_statuses[i].State)
			{
				m_key_statuses[i].State = eKeyState::Release;
			}
			else if (eKeyState::Release & m_key_statuses[i].State)
			{
				m_key_statuses[i].State = (UINT8)0;
			}
		}

		m_mouse_dir.x = 0.f;
		m_mouse_dir.y = 0.f;
	}
}
