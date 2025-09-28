#include "Timer.h"

Timer::Timer()
	: m_prev_freq()
	, m_cur_freq()
	, m_freq()
	, m_FPS_limit()
	, m_frame_time_limit()
	, m_deltatime()
{
}

void Timer::init()
{
	LARGE_INTEGER Freq;
	QueryPerformanceFrequency(&Freq);
	m_freq = 1.f / (float)Freq.QuadPart;

	QueryPerformanceCounter(&m_prev_freq);
	QueryPerformanceCounter(&m_cur_freq);

	m_deltatime = 0.f;

	set_FPS_limit(60);
}

Timer::~Timer()
{
}

void Timer::update()
{
	QueryPerformanceCounter(&m_cur_freq);

	m_deltatime = (m_cur_freq.QuadPart - m_prev_freq.QuadPart) * m_freq;

	if (m_deltatime < m_frame_time_limit)
	{
		Sleep((DWORD)((m_frame_time_limit - m_deltatime) * 1000.f));
	}
	else
	{
		m_deltatime = m_frame_time_limit;
	}

	m_prev_freq = m_cur_freq;
}

void Timer::set_FPS_limit(UINT16 _FPSLimit)
{
	m_FPS_limit = _FPSLimit;
	m_frame_time_limit = 1.f / (float)m_FPS_limit;
}
