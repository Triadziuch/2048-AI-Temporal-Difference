#pragma once
#include "State.h"

class Transition
{
public:
	int reward;
	State* afterstate, * nextState;

	// Constructors / Destructors
	Transition();
	Transition(int reward, State* afterstate, State* nextState);
	Transition(const Transition& other);
};