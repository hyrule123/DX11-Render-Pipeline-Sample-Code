#include "Manager.h"

#include "DX11.h"
#include "Input.h"
#include "Timer.h"
#include "Scene.h"



Manager::Manager()
{
}

void Manager::init()
{
	m_input = new Input;
	m_timer = new Timer;
	m_dx = new DX11;
	m_scene = new Scene;

	m_input->init();
	m_timer->init();
	m_dx->init();
	m_scene->init();

	set_winsize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

Manager::~Manager()
{
	SAFE_DELETE(m_scene);
	SAFE_DELETE(m_dx);
	SAFE_DELETE(m_input);
	SAFE_DELETE(m_timer);
}

void Manager::set_winsize(UINT _width, UINT _height)
{
	assert(0u < _width);
	assert(0u < _height);

	m_width = _width;
	m_height = _height;

	RECT WINSIZE = {};
	WINSIZE.left = 0;
	WINSIZE.right = (LONG)m_width;
	WINSIZE.top = 0;
	WINSIZE.bottom = (LONG)m_height;

	//윈도우 사이즈 조절 
	AdjustWindowRect(&WINSIZE, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(g_hWnd, nullptr, 100, 100, WINSIZE.right - WINSIZE.left, WINSIZE.bottom - WINSIZE.top, 0);
	ShowWindow(g_hWnd, true);

	m_dx->set_resolution(m_width, m_height);
	m_scene->on_resolution_change(m_width, m_height);
}

void Manager::run()
{
	m_timer->update();
	m_input->update();
	m_scene->update();

	m_dx->update();

	m_scene->render();
	m_dx->render();
}