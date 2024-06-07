#ifndef B2_TIMER_H
#define B2_TIMER_H

#include "./b2Settings.h"

/// Timer for profiling. This has platform specific code and may
/// not work on every platform.
class b2Timer
{
public:

	/// Constructor
	b2Timer();

	/// Reset the timer.
	void Reset();

	/// Get the time since construction or the last reset.
	float32 GetMilliseconds() const;

private:

#if defined(_WIN32)
	float64 m_start;
	static float64 s_invFrequency;
#elif defined(__linux__) || defined (__APPLE__)
	unsigned long m_start_sec;
	unsigned long m_start_usec;
#endif
};

#endif
