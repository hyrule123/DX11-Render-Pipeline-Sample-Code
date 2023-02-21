#include "CTimer.h"

float g_DeltaTime;

CTimer::CTimer()
{
	LARGE_INTEGER Freq;
	QueryPerformanceFrequency(&Freq);
	m_Freq = 1.f / (float)Freq.QuadPart;

	QueryPerformanceCounter(&m_PrevFreq);
	QueryPerformanceCounter(&m_CurFreq);

	g_DeltaTime = 0.f;

	SetFPSLimit(60);
}

CTimer::~CTimer()
{
}

void CTimer::tick()
{
	QueryPerformanceCounter(&m_CurFreq);

	g_DeltaTime = (m_CurFreq.QuadPart - m_PrevFreq.QuadPart) * m_Freq;

	if (g_DeltaTime < m_FrameTimeLimit)
	{
		Sleep((DWORD)((m_FrameTimeLimit - g_DeltaTime) * 1000.f));
	}
	else
	{
		g_DeltaTime = m_FrameTimeLimit;
	}

	m_PrevFreq = m_CurFreq;
}

void CTimer::SetFPSLimit(UINT16 _FPSLimit)
{
	m_FPSLimit = _FPSLimit;
	m_FrameTimeLimit = 1.f / (float)m_FPSLimit;
}
