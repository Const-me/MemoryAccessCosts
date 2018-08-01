#pragma once

class Random
{
	// https://stackoverflow.com/a/1640399/126995
	uint32_t x, y, z;

public:
	Random()
	{
		reset();
	}

	void reset()
	{
		x = 123456789;
		y = 362436069;
		z = 521288629;
	}

	uint32_t next()
	{
		uint32_t t;
		x ^= x << 16;
		x ^= x >> 5;
		x ^= x << 1;

		t = x;
		x = y;
		y = z;
		z = t ^ x ^ y;
		return z;
	}
};