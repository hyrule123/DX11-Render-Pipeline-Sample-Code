#include "Manager.h"

#include "DX11.h"
#include "Input.h"
#include "Timer.h"

Manager::Manager()
	: m_dx()
	, m_timer()
	, m_input()
{
}

Manager::~Manager()
{
	SAFE_DELETE(m_input);
	SAFE_DELETE(m_timer);
	SAFE_DELETE(m_dx);
}

void Manager::init()
{
	m_input = new Input;
	m_input->init();

	m_timer = new Timer;
	m_timer->init();

	m_dx = new DX11;
	m_dx->init();

	 RECT WINSIZE = {};
	 WINSIZE.left = 0;
	 WINSIZE.right = (LONG)DEFAULT_WIDTH;
	 WINSIZE.top = 0;
	 WINSIZE.bottom = (LONG)DEFAULT_HEIGHT;

	//윈도우 사이즈 조절 
	AdjustWindowRect(&WINSIZE, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(g_hWnd, nullptr, 100, 100, WINSIZE.right - WINSIZE.left, WINSIZE.bottom - WINSIZE.top, 0);
	ShowWindow(g_hWnd, true);
}

void Manager::run()
{
	m_timer->update();
	m_input->update();

}