#pragma once
#include "Agent.h"

inline const State* const Agent::getState() const
{
	return matrix->getState();
}

float Agent::get_state_value(const State* const state, const LUTContainer* const LUT) const
{
    float value = 0.0f;
    
    // Displaying state
    for (size_t i = 0; i < state->HEIGHT; ++i) {
		for (size_t j = 0; j < state->WIDTH; ++j) {
			std::cout << state->board[i][j] << " ";
		}
		std::cout << std::endl;
	}

    for (size_t i = 0; i < LUTContainer::NUM_VERTICAL; ++i) 
        for (size_t j = 0; j < state->WIDTH; ++j)
            value += LUT->V_LUT[i][state->board[0][j]][state->board[1][j]][state->board[2][j]][state->board[3][j]];
    

    for (size_t i = 0; i < LUTContainer::NUM_HORIZONTAL; ++i) 
		for (size_t j = 0; j < state->HEIGHT; ++j)
			value += LUT->H_LUT[i][state->board[j][0]][state->board[j][1]][state->board[j][2]][state->board[j][3]];
	

    size_t square_index = 0;
    for (size_t i = 0; i < state->HEIGHT - 1; ++i) 
        for (size_t j = 0; j < state->WIDTH - 1; ++j) 
            value += LUT->SQUARE_LUT[square_index][state->board[i][j]][state->board[i][j + 1]][state->board[i + 1][j]][state->board[i + 1][j + 1]];
            square_index++;

    return value;
}

Agent::Agent(Playground* playgroundPtr) : playground(playgroundPtr), matrix{ playground->getTileMatrix() } {}

void Agent::update(const float dt)
{
	const State* current_state = getState();

	if (!playground->getIsGameOver()) {
        printf("Value: %f\n", get_state_value(current_state, &m_LUTs));
	}

	delete current_state;
}