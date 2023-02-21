#pragma once

#include <windows.h>


class CTimer
{
public:
	CTimer();
	~CTimer();
	void tick();

private:
	LARGE_INTEGER m_PrevFreq;
	LARGE_INTEGER m_CurFreq;
	float m_Freq;
	UINT16	m_FPSLimit;
	float m_FrameTimeLimit;

public:
	void SetFPSLimit(UINT16 _FPSLimit);
};

