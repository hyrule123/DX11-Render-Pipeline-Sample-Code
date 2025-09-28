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
{
	m_vecKeyInfo.resize(eKey::END);
	for (UINT16 i = 0; i < eKey::END; ++i)
	{
		m_vecKeyInfo[i].myKey = (eKey)(UINT16)i;
	}
}

Input::~Input()
{
}

void Input::tick()
{
	if (NULL != GetFocus())
	{
		//Compute keyboard and mouse button status
		for (UINT16 i = 0; i < eKey::END; ++i)
		{
			//If key pressed
			if (GetAsyncKeyState(g_arrKey[i]) & 0x8000)
			{
				//It means pressed on this frame
				if (eKeyState::None == m_vecKeyInfo[i].State)
				{
					m_vecKeyInfo[i].State |= eKeyState::Tap;
					m_vecKeyInfo[i].State |= eKeyState::Pressed;
				}
				
				//It means keep pressing the key
				else if(eKeyState::Pressed & m_vecKeyInfo[i].State)
				{
					m_vecKeyInfo[i].State &= ~eKeyState::Tap;
				}

			}
			else
			{
				//It means the key released right before
				if (eKeyState::Pressed & m_vecKeyInfo[i].State)
				{
					m_vecKeyInfo[i].State = eKeyState::Release;
				}
				else
				{
					m_vecKeyInfo[i].State = eKeyState::None;
				}
			}
		}


		//Compute mouse position and direction
		m_PrevMousePos = m_MousePos;

		POINT ptMousePos = {};
		GetCursorPos(&ptMousePos);

		ScreenToClient(g_hWnd, &ptMousePos);

		m_MousePos.x = (float)ptMousePos.x;
		m_MousePos.y = (float)ptMousePos.y;

		m_MouseDir = m_MousePos - m_PrevMousePos;
		m_MouseDir.y *= -1.f;	
	}

	//If the window is not focused
	else
	{
		for (UINT16 i = 0; i < eKey::END; ++i)
		{
			if (eKeyState::Pressed & m_vecKeyInfo[i].State)
			{
				m_vecKeyInfo[i].State = eKeyState::Release;
			}
			else if (eKeyState::Release & m_vecKeyInfo[i].State)
			{
				m_vecKeyInfo[i].State = (UINT8)0;
			}
		}

		m_MouseDir.x = 0.f;
		m_MouseDir.y = 0.f;
	}
}
