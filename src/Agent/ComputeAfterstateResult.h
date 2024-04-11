#pragma once
#include "State.h"

class ComputeAfterstateResult
{
public:
	int reward;
	State* afterstate;

	// Constructors / Destructors
	ComputeAfterstateResult();
	ComputeAfterstateResult(int reward, State* afterstate);
	ComputeAfterstateResult(const ComputeAfterstateResult& other);
	~ComputeAfterstateResult();
};