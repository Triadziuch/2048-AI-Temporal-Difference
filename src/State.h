#pragma once
#include <stdio.h>
class State
{
private:
	size_t WIDTH{}, HEIGHT{};

public:
	State() = delete;
	State(const size_t& width = 4, const size_t& height = 4);
	State(const State& other);
	~State();

	int **board;
};

