#pragma once
#include "Agent.h"

class AgentQLearning : public Agent
{
private:
	NTuples m_LUTs[4];

	// Evaluate function
	// EVALUATE(s, A) -> a

	void loadLatestLUTs();

	double getFunctionValue(const State* const state, const Taction a) const;
	const Taction chooseBestAction(const State* const state, const std::vector<Taction> available_actions) const;
	const double getBestFunctionValue(const State* const state, const std::vector<Taction> available_actions) const;

	// Learn evaluation function
	void updateValueFunction(const State* const state, double delta, const Taction action);

	bool randomNextAction = true;
	Taction nextAction = Taction::UP;

public:
	// Constructors / Destructors
	AgentQLearning() = delete;
	AgentQLearning(Playground* playground);
	AgentQLearning(const AgentQLearning& agent);
	~AgentQLearning();

	// Update function
	void update(const float dt);
};