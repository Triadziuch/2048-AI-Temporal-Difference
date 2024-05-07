#pragma once
#include "AgentTD.h"

class AgentTDState : public AgentTD
{
private:
	// EVALUATE(s, A) -> a
	const Taction chooseBestAction(const State* const state, const std::vector<Taction>& available_actions) const;

	// Learn evaluation function
	void updateValueFunction(const State* const state, double delta);

public:
	// Constructors / Destructors
	AgentTDState() = delete;
	AgentTDState(Playground* playground);
	AgentTDState(const AgentTDState& agent);
	~AgentTDState();

	// Update function
	void update(const float dt);
};