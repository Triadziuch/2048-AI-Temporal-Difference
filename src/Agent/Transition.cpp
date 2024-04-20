#include "Transition.h"

// Constructors / Destructors
Transition::Transition() : reward(0), afterstate(nullptr), nextState(nullptr) {}

Transition::Transition(int reward, State* afterstate, State* nextState) : reward(reward), afterstate(afterstate), nextState(nextState) {}

Transition::Transition(const Transition& other) : reward(other.reward), afterstate(other.afterstate), nextState(other.nextState) {}

Transition::~Transition()
{
	delete afterstate;
	delete nextState;
}
