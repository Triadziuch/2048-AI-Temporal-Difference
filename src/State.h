#pragma once

class State
{
private:
	size_t WIDTH{}, HEIGHT{};
public:
	State() = delete;
	State(const size_t& width = 4, const size_t& height = 4);
	~State();

	int **board;
};

