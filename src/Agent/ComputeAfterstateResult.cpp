#include "ComputeAfterstateResult.h"

// Constructors / Destructors
ComputeAfterstateResult::ComputeAfterstateResult() : reward(0.0f), afterstate(nullptr) {}

ComputeAfterstateResult::ComputeAfterstateResult(int reward, State* afterstate) : reward(reward), afterstate(afterstate) {}

ComputeAfterstateResult::ComputeAfterstateResult(const ComputeAfterstateResult& other) : reward(other.reward), afterstate(other.afterstate) {}

ComputeAfterstateResult::~ComputeAfterstateResult() 
{
	afterstate = nullptr;
}