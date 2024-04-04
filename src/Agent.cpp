#pragma once
#include "Agent.h"

inline State* const Agent::getState() const
{
	return matrix->getState();
}

float Agent::get_state_value(const State* const state) const
{
    float value = 0.0f;
    
    // Displaying state
	//state->display();

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

Taction Agent::evaluate(const State* const state) const
{
	float best_value = -std::numeric_limits<float>::infinity();
	Taction best_action = Taction::UP;

	for (size_t i = 0; i < 4; ++i) {
		State* new_state = new State(*state);
		Taction action = static_cast<Taction>(i);

		float reward = new_state->move(action); // COMPUTE AFTERSTATE

		float value = get_state_value(new_state); // V(s')
		if (value + reward > best_value) {
			best_value = value;
			best_action = action;
		}

		delete new_state;
	}

	return best_action;
}

MoveResult Agent::make_move(State* const state, const Taction action)
{
	// current_state = s -> s'
	// state = s -> s"

	MoveResult result;
	result.reward = state->move(action); // COMPUTE AFTERSTATE
	result.afterstate = state;

	if (action == Taction::UP) 
		playground->move(sf::Keyboard::Up);
	else if (action == Taction::DOWN) 
		playground->move(sf::Keyboard::Down);
	else if (action == Taction::LEFT) 
		playground->move(sf::Keyboard::Left);
	else if (action == Taction::RIGHT) 
		playground->move(sf::Keyboard::Right);

	result.next_state = matrix->getState(); // s"

	return result;
}

void Agent::learn_evaluation(const State* const state, const Taction action, const float reward, const State* const afterstate, const State* const next_state)
{
	// state		=	s
	// action		=	a
	// reward		=	r
	// afterstate	=	s'
	// next_state	=	s"
	
	// V(s) = V(s) + alpha * (r + V(s') - V(s))
	float alpha = 0.1f;
	float value = get_state_value(state);
	float afterstate_value = get_state_value(afterstate);
	float next_state_value = get_state_value(next_state);

	value = value + alpha * (reward + afterstate_value - value);
}

Agent::Agent(Playground* playgroundPtr) : playground(playgroundPtr), matrix{ playground->getTileMatrix() } {}

void Agent::update(const float dt)
{
	State* current_state = getState();
	playground->update(dt);

	if (!playground->getIsGameOver() && !playground->getIsMoving()) {
        printf("Value: %f\n", get_state_value(current_state));
		Taction best_action = evaluate(current_state);
		MoveResult result = make_move(current_state, best_action);

		float reward = result.reward;			// r
		State* afterstate = result.afterstate;	// s'
		State* next_state = result.next_state;	// s"

		if (learning_enabled)
			learn_evaluation(current_state, best_action, reward, afterstate, next_state);
	}

	delete current_state;
}