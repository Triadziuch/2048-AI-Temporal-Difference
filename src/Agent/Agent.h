#pragma once
#include "../Playground.h"
#include "NTuples.h"
#include "Transition.h"
#include "ComputeAfterstateResult.h"

#include <filesystem>
#include <random>

class Agent
{
private:
	Playground* playground;
	TileMatrix* matrix;
	NTuples m_LUTs;

	// Configuration
	double learningRate    = 0.01;
	double explorationRate = 0.001;
	bool learningEnabled = true;
	bool loggingEnabled = true;
	bool resumeLearning = true;
	const std::string configFilename = "config.txt";

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
	long long int total_games = 0;
	

	// Utility functions
	void loadConfig();
	void loadLatestLUTs();
	State* const getState() const;
	float generateRandomFloat(float min = 0.f, float max = 1.f) const;

	// Logging functions
	void log(int score);
	void displayProgress(int score) const;

	// Learning functions
	Taction chooseBestAction(const State* const state) const;
	double getBestValueAction(const State* const state) const;

	double getFunctionValue(const State* const state) const;
	void updateValueFunction(const State* const afterstate, double expectedValue, double learningRate);

	Transition move(const State* const state, const Taction action);

	ComputeAfterstateResult computeAfterstate(const State* const state, const Taction action);

public:
	// Constructors / Destructors
	Agent() = delete;
	Agent(Playground* playgroundPtr);
	Agent(const Agent& other);
	~Agent();

	// Update function
	void update(const float dt);

	// Public functions
	void episodeEnded(int score);

	// Accessors
	long long int getGamesCount() const;
	long long int getStepsCount() const;
	bool getIsLearning() const;
};