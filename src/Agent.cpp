#pragma once
#include "Agent.h"

inline const State& Agent::getState() const
{
	return matrix->getState();
}

Agent::Agent(Playground* playgroundPtr) : playground(playgroundPtr), matrix{ playground->getTileMatrix() } {}

void Agent::update(const float dt)
{
	State current_state = getState();
}