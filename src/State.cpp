#pragma once
#include "State.h"

State::State(const size_t& width, const size_t& height) : WIDTH(width), HEIGHT(height)
{
	board = new int* [HEIGHT];
	for (size_t i = 0; i < HEIGHT; ++i)
		board[i] = new int[WIDTH];
}

State::State(const State& other) : WIDTH(other.WIDTH), HEIGHT(other.HEIGHT)
{
	board = new int* [HEIGHT];
	for (size_t i = 0; i < HEIGHT; ++i)
		board[i] = new int[WIDTH];

	for (size_t i = 0; i < HEIGHT; ++i)
		for (size_t j = 0; j < WIDTH; ++j)
			board[i][j] = other.board[i][j];
}

State::~State()
{
	for (size_t i = 0; i < HEIGHT; ++i) 
		delete[] board[i];
	delete[] board;
}
