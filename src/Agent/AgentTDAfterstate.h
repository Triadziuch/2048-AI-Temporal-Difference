#pragma once
#include "AgentTD.h"

class AgentTDAfterstate : public AgentTD
{
private:
	// EVALUATE(s, A) -> a
	const Taction chooseBestAction(const State* const state, const std::vector<Taction>& available_actions) const;

	// Learn evaluation function
	void updateValueFunction(const State* const afterstate, double expectedValue, double learningRate);

public:
	// Constructors / Destructors
	AgentTDAfterstate() = delete;
	AgentTDAfterstate(Playground* playground);
	AgentTDAfterstate(const AgentTDAfterstate& agent);
	~AgentTDAfterstate();

	// Update function
	void update(const float dt);
};