#pragma once

#include <cstdint>


namespace coco {

/**
 * Simple xor shift pseudo random number generator
 */
struct XorShiftRandom {
	uint32_t a;

	/**
	 * Constructor
	 * @param seed seed must not be zero
	 */
	explicit XorShiftRandom(uint32_t seed = 362436000) {
		assert(seed != 0);
		a = seed;
	}

	/**
	 * Reset the random number generator
	 * @param seed seed must not be zero
	 */
	void reset(uint32_t seed = 362436000) {
		assert(seed != 0);
		a = seed;
	}

	uint32_t draw() {
		// Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
		uint32_t x = this->a;
		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 5;
		return this->a = x;
	}
};


/**
 * KISS random number generator
 * http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
 * https://link.springer.com/content/pdf/10.1007/s12095-017-0225-x.pdf
 */
struct KissRandom {
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t c;

	/**
	 * Constructor
	 * @param seed1 first seed
	 * @param seed2 second seed, must not be zero
	 * @param seed3 third seed
	 */
	explicit KissRandom(uint32_t seed1 = 123456789, uint32_t seed2 = 362436000, uint32_t seed3 = 521288629) {
		x = seed1;
		y = seed2;
		z = seed3;
		c = 7654321;
	}

	/**
	 * Reset the random number generator
	 * @param seed1 first seed
	 * @param seed2 second seed, must not be zero
	 * @param seed3 third seed
	 */
	void reset(uint32_t seed1 = 123456789, uint32_t seed2 = 362436000, uint32_t seed3 = 521288629) {
		x = seed1;
		y = seed2;
		z = seed3;
		c = 7654321;
	}

	uint32_t draw() {
		// Linear congruence generator
		x = 69069 * x + 12345;

		// Xor shift
		y ^= y << 13;
		y ^= y >> 17;
		y ^= y << 5;

		// Multiply-with-carry
		uint64_t t = 698769069ULL * z + c;
		c = t >> 32;
		z = (uint32_t) t;

		return x + y + z;
	}
};

} // namespace coco
