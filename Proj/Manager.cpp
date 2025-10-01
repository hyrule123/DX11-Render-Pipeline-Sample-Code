#include "Manager.h"

#include "DX11.h"
#include "Input.h"
#include "Timer.h"
#include "Scene.h"

Manager::Manager() {}

Manager::~Manager()
{
	m_input.reset();
	m_timer.reset();
	m_dx.reset();
}

void Manager::init()
{
	m_input = std::make_unique<Input>();
	m_input->init();

	m_timer = std::make_unique<Timer>();
	m_timer->init();

	m_dx = std::make_unique<DX11>();
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
	m_scene->update();
}