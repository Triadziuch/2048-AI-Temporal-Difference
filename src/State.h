#pragma once
#include <stdio.h>
class State
{
public:
	size_t WIDTH{}, HEIGHT{};

	State() = delete;
	State(const size_t& width = 4, const size_t& height = 4);
	State(const State& other);
	~State();

	int **board;
};

