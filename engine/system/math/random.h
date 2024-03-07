#pragma once

#include <cstdint>
#include <algorithm>

namespace raid
{

// xorshift*
// https://en.wikipedia.org/wiki/Xorshift

class BasicRNG
{
public:
	BasicRNG(uint64_t seed = 88172645463325252ull)
		: m_State(seed)
	{
	}

	uint64_t GetNext()
	{
		m_State ^= m_State >> 12;
		m_State ^= m_State << 25;
		m_State ^= m_State >> 27;
		return m_State * 0x2545F4914F6CDD1DULL;
	}

	void Reset(uint64_t seed)
	{
		m_State = seed;
	}

	int GetInt(int min, int max) 
	{
		// Ensure min <= max
		if (min > max)
			std::swap(min, max);

		// Calculate range and get random value
		uint64_t range = static_cast<uint64_t>(max) - min + 1;
		uint64_t random_value = GetNext();

		// Map the random value to the range [min, max]
		return min + static_cast<int>(random_value % range);
	}

	float GetFloat(float min, float max)
	{
		// Ensure min <= max
		if (min > max)
			std::swap(min, max);

		// Calculate range and get random value
		double range = static_cast<double>(max) - min;
		uint64_t random_value = GetNext();

		// Map the random value to the range [min, max]
		return min + static_cast<float>((static_cast<double>(random_value) / UINT64_MAX) * range);
	}

private:
	uint64_t m_State;
};

} // namespace raid