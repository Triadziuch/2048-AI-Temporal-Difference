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

struct MoveResult
{
	float reward;
	State* afterstate, *next_state;

	MoveResult() : reward(0.0f), afterstate(nullptr), next_state(nullptr) {}
	MoveResult(float reward, State* afterstate, State* next_state) : reward(reward), afterstate(afterstate), next_state(next_state) {}
	MoveResult(const MoveResult& other) : reward(other.reward), afterstate(other.afterstate), next_state(other.next_state) {}
};

struct ComputeAfterstateResult
{
	float reward;
	State* afterstate;

	ComputeAfterstateResult() : reward(0.0f), afterstate(nullptr) {}
	ComputeAfterstateResult(float reward, State* afterstate) : reward(reward), afterstate(afterstate) {}
	ComputeAfterstateResult(const ComputeAfterstateResult& other) : reward(other.reward), afterstate(other.afterstate) {}
};

class Agent
{
private:
	Playground* playground;
	TileMatrix* matrix;
	LUTContainer m_LUTs;

	bool learning_enabled = true;

	State* const getState() const;

	
	float get_state_value(const State* const state) const;
	Taction evaluate(const State* const state) const;
	MoveResult make_move(const State* const state, const Taction action);

	void learn_evaluation(const State* const state, const Taction action, const float reward, const State* const afterstate, const State* const next_state);
	ComputeAfterstateResult compute_afterstate(const State* const state, const Taction action);

public:
	Agent(Playground* playgroundPtr);

	void update(const float dt);
};