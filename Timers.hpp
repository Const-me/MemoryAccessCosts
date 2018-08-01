#pragma once
// For the __rdtsc() intrinsic
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

// TODO: change to your CPU spec. I don't know portable way to query base CPU frequency.
constexpr double cpuFrequencyGHz = 3.3;
constexpr double cpuFrequencyHertz = cpuFrequencyGHz * 1000000000;

// A timer that measures CPU cycles as reported by RDTSC instruction. It uses cpuFrequencyGHz constant to derive wall clock time.
class CyclesTimer
{
	const uint64_t started;

public:
	CyclesTimer() : started( __rdtsc() ) { }

	uint64_t elapsed() const
	{
		const uint64_t now = __rdtsc();
		return now - started;
	}

	static double seconds( uint64_t val )
	{
		return (double)val / cpuFrequencyHertz;
	}

	static int cyclesPerIteration( uint64_t val, size_t iterations )
	{
		const double res = (double)val / double( iterations );
		return lround( res );
	}
};

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// A timer that measures with QueryPerformanceCounter. It uses cpuFrequencyGHz constant to derive CPU cycles from that.
class QpcTimer
{
	const uint64_t started;

	static uint64_t qpc()
	{
		LARGE_INTEGER res;
		QueryPerformanceCounter( &res );
		return res.QuadPart;
	}

public:
	QpcTimer() : started( qpc() ) { }

	uint64_t elapsed() const
	{
		const uint64_t now = qpc();
		return now - started;
	}

	static double seconds( uint64_t val )
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency( &freq );
		return (double)val / (double)freq.QuadPart;
	}

	static int cyclesPerIteration( uint64_t val, size_t iterations )
	{
		const double sec = seconds( val ) / iterations;
		return lround( sec * cpuFrequencyHertz );
	}
};
#endif