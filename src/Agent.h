#pragma once
#include "Playground.h"
#include <sstream>
#include <filesystem>
#include <random>

class LUTContainer
{
private:
	constexpr static size_t MIN_TYPE = 0;
	constexpr static size_t MAX_TYPE = 12;
	

public:
	constexpr static size_t NUM_VERTICAL = 4;
	constexpr static size_t NUM_HORIZONTAL = 4;
	constexpr static size_t NUM_SQUARES = 9;

	float V_LUT[NUM_VERTICAL][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1]{ 0.f };
	float H_LUT[NUM_HORIZONTAL][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1]{ 0.f };
	float SQUARE_LUT[NUM_SQUARES][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1][MAX_TYPE + 1]{ 0.f };

	void save(const std::string& filename) const;
	void load(const std::string& filename);
};

struct Transition
{
	int reward;
	State* afterstate, *nextState;

	Transition() : reward(0.0f), afterstate(nullptr), nextState(nullptr) {}
	Transition(float reward, State* afterstate, State* nextState) : reward(reward), afterstate(afterstate), nextState(nextState) {}
	Transition(const Transition& other) : reward(other.reward), afterstate(other.afterstate), nextState(other.nextState) {}
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

	// Configuration
	constexpr static double learningRate    = 0.01;
	constexpr static double explorationRate = 0.001f;
	bool loggingEnabled = true;
	bool resumeLearning = true;

	// Logging variables
	std::ofstream logFile;
	const std::string logFilename = "log.txt";
	
	// LUT logging variables
	const std::string logTupleFolder	= "LUTs/";
	const std::string logTupleFilename	= "LUT_";
	const std::string logTupleExtension = ".txt";
	long long int lastSavedGame	= -1;
	

	// Statistics variables
	long long int total_steps = 0;
	void loadLatestLUTs();

	State* const getState() const;
	float generateRandomFloat(float min = 0.f, float max = 1.f);
	
	float get_state_value(const State* const state) const;
	Taction evaluate(const State* const state) const;
	float evaluate_getbestactionvalue(const State* const state) const;
	double getBestValueAction(const State* const state, Taction& best_action) const;
	void updateValueFunction(const State* const afterstate, double expectedValue, double learningRate);

	Transition make_move(const State* const state, const Taction action);

	ComputeAfterstateResult compute_afterstate(const State* const state, const Taction action);
	

public:
	long long int total_games = 0;

	Agent(Playground* playgroundPtr);

	void update(const float dt);

	void displayProgress(int score) const;
	void log(int score);
};