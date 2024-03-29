#pragma once
#include "Agent.h"

inline const State* const Agent::getState() const
{
	return matrix->getState();
}

Agent::Agent(Playground* playgroundPtr) : playground(playgroundPtr), matrix{ playground->getTileMatrix() } {}

void Agent::update(const float dt)
{
	const State* current_state = getState();

	delete current_state;
}