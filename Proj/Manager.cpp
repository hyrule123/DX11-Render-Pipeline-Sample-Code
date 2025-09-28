#include "Manager.h"

#include "DX11.h"

Manager::Manager()
	: m_dx()
{
}

Manager::~Manager()
{
	if (m_dx) { delete m_dx; }
}

bool Manager::init()
{
	 RECT WINSIZE = {};
	 WINSIZE.left = 0;
	 WINSIZE.right = (LONG)DEFAULT_WIDTH;
	 WINSIZE.top = 0;
	 WINSIZE.bottom = (LONG)DEFAULT_HEIGHT;

	//윈도우 사이즈 조절 
	AdjustWindowRect(&WINSIZE, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(g_hWnd, nullptr, 100, 100, WINSIZE.right - WINSIZE.left, WINSIZE.bottom - WINSIZE.top, 0);
	ShowWindow(g_hWnd, true);

	m_dx = new DX11;

	return true;
}

void Manager::run()
{
}