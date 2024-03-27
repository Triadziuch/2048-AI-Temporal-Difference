#pragma once
#include "Agent.h"

Agent::Agent(Playground* playgroundPtr) : playground(playgroundPtr), matrix{ playground->getTileMatrix() } {}

void Agent::update(const float dt)
{
}