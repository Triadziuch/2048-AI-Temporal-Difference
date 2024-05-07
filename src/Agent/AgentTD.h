#pragma once
#include "Agent.h"

class AgentTD : public Agent
{
protected:
	void loadLatestLUTs();

public:
	// Constructors / Destructors
	AgentTD() = delete;
	AgentTD(Playground* playground);
	AgentTD(const AgentTD& agent);
	~AgentTD();
};