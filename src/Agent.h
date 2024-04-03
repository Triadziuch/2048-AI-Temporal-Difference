#pragma once
#include "Playground.h"

class LUTContainer
{
private:
	constexpr static size_t MIN_TYPE = 0;
	constexpr static size_t MAX_TYPE = 12;
	

public:
	constexpr static size_t NUM_VERTICAL = 4;
	constexpr static size_t NUM_HORIZONTAL = 4;
	constexpr static size_t NUM_SQUARES = 9;

	float V_LUT[NUM_VERTICAL][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1]{ 0.5f };
	float H_LUT[NUM_HORIZONTAL][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1]{ 0.3f };
	float SQUARE_LUT[NUM_SQUARES][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1]{ 0.1f };
};

class Agent
{
private:
	Playground* playground;
	TileMatrix* matrix;

	LUTContainer m_LUTs;

	const State* const getState() const;

	float get_state_value(const State* const state, const LUTContainer* const LUT) const;

public:
	Agent(Playground* playgroundPtr);

	void update(const float dt);
};