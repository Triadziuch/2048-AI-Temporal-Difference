#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include "State.h"

class NTuples
{
protected:
	constexpr static size_t MIN_TYPE = 0;
	constexpr static size_t MAX_TYPE = 14;

public:
	constexpr static size_t NUM_VERTICAL = 4;
	constexpr static size_t NUM_HORIZONTAL = 4;
	constexpr static size_t NUM_SQUARES = 9;

	double V_LUT[NUM_VERTICAL][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1]{ 0.f };
	double H_LUT[NUM_HORIZONTAL][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1]{ 0.f };
	double SQUARE_LUT[NUM_SQUARES][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1]{ 0.f };

	void save(const std::string& filename) const;
	void load(const std::string& filename);

	const void updateVFunction(const State* const state, const double& delta);
};