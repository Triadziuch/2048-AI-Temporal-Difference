#pragma once
#include "Agent.h"

inline const State* const Agent::getState() const
{
	return matrix->getState();
}

float Agent::get_state_value(const State* const state) const
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
            value += m_LUTs.V_LUT[i][state->board[0][j]][state->board[1][j]][state->board[2][j]][state->board[3][j]];
    

    for (size_t i = 0; i < LUTContainer::NUM_HORIZONTAL; ++i) 
		for (size_t j = 0; j < state->HEIGHT; ++j)
			value += m_LUTs.H_LUT[i][state->board[j][0]][state->board[j][1]][state->board[j][2]][state->board[j][3]];
	

    size_t square_index = 0;
    for (size_t i = 0; i < state->HEIGHT - 1; ++i) 
        for (size_t j = 0; j < state->WIDTH - 1; ++j) 
            value += m_LUTs.SQUARE_LUT[square_index][state->board[i][j]][state->board[i][j + 1]][state->board[i + 1][j]][state->board[i + 1][j + 1]];
            square_index++;

    return value;
}

Taction Agent::getBestAction(const State* const state) const
{
    float best_value = -std::numeric_limits<float>::infinity();
	Taction best_action = Taction::UP;

    for (size_t i = 0; i < 4; ++i) {
		State* new_state = new State(*state);
		Taction action = static_cast<Taction>(i);

		float reward = new_state->move(action);

		float value = get_state_value(new_state);
        if (value + reward > best_value) {
			best_value = value;
			best_action = action;
		}

		delete new_state;
	}

	return best_action;
}

Agent::Agent(Playground* playgroundPtr) : playground(playgroundPtr), matrix{ playground->getTileMatrix() } {}

void Agent::update(const float dt)
{
	const State* current_state = getState();

	if (!playground->getIsGameOver()) {
        printf("Value: %f\n", get_state_value(current_state));
		Taction best_action = getBestAction(current_state);
	}

	delete current_state;
}