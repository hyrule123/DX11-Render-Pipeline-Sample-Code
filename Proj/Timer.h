#pragma once

#include <windows.h>


class Timer
{
	friend class Manager;
private:
	Timer();
	void init();

	~Timer();
	Timer(const Timer& _other) = delete;
	Timer(Timer&& _move) = delete;

	void update();

public:
	float get_deltatime() const { return m_deltatime; }

private:
	LARGE_INTEGER m_prev_freq;
	LARGE_INTEGER m_cur_freq;
	float m_freq;
	UINT16	m_FPS_limit;
	float m_frame_time_limit;

	float m_deltatime;

public:
	void set_FPS_limit(UINT16 _FPSLimit);
};

