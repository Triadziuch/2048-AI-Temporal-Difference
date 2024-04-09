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

// EVALUATE(s) -> a
Taction Agent::evaluate(const State* const state) const
{
	float best_value = -std::numeric_limits<float>::infinity();
	Taction best_action = Taction::UP;

	for (size_t i = 0; i < 4; ++i) {
		State* new_state = new State(*state);
		Taction action = static_cast<Taction>(i);

		if (!new_state->isMovePossible(action)) {
			delete new_state;
			continue;
		}

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

// MAKE MOVE(s, a) -> r, s', s"
MoveResult Agent::make_move(const State* const state, const Taction action)
{
	// current_state = s -> s'
	// state = s -> s"

	MoveResult result;

	ComputeAfterstateResult afterstate_result = compute_afterstate(state, action);
	result.reward = afterstate_result.reward; // r
	result.afterstate = afterstate_result.afterstate; // s'

	if (action == Taction::UP) 
		playground->instantMove(sf::Keyboard::Up);
	else if (action == Taction::DOWN) 
		playground->instantMove(sf::Keyboard::Down);
	else if (action == Taction::LEFT) 
		playground->instantMove(sf::Keyboard::Left);
	else if (action == Taction::RIGHT) 
		playground->instantMove(sf::Keyboard::Right);

	result.next_state = matrix->getState(); // s"

	/*if (total_steps % 500 == 0) {
		printf("\n\n");
		state->display("State:\n");
		result.afterstate->display("Afterstate:\n");
		result.next_state->display("Next_state:\n");
	}*/

	return result;
}

void Agent::learn_evaluation(const State* const state, const Taction action, const float reward, const State* const afterstate, const State* const next_state)
{
	// state		=	s
	// action		=	a
	// reward		=	r
	// afterstate	=	s'
	// next_state	=	s"

	Taction	a_next = evaluate(next_state); // a_next
	ComputeAfterstateResult	compute_afterstate_results = compute_afterstate(next_state, a_next);

	State* afterstate_next = compute_afterstate_results.afterstate; // s'_next
	float reward_next = compute_afterstate_results.reward;			 // r_next

	// V(s) = V(s) + alpha * (r + V(s') - V(s))
	float alpha = 0.001f;

	for (size_t i = 0; i < LUTContainer::NUM_VERTICAL; ++i)
		for (size_t j = 0; j < afterstate->WIDTH; ++j) {
			float v_afterstate = m_LUTs.V_LUT[i][afterstate->board[0][j]][afterstate->board[1][j]][afterstate->board[2][j]][afterstate->board[3][j]];
			float v_aftestate_next = m_LUTs.V_LUT[i][afterstate_next->board[0][j]][afterstate_next->board[1][j]][afterstate_next->board[2][j]][afterstate_next->board[3][j]];
			m_LUTs.V_LUT[i][afterstate->board[0][j]][afterstate->board[1][j]][afterstate->board[2][j]][afterstate->board[3][j]] += alpha * (reward_next + v_aftestate_next - v_afterstate);
		}

	for (size_t i = 0; i < LUTContainer::NUM_HORIZONTAL; ++i)
		for (size_t j = 0; j < afterstate->HEIGHT; ++j) {
			float v_afterstate = m_LUTs.H_LUT[i][afterstate->board[j][0]][afterstate->board[j][1]][afterstate->board[j][2]][afterstate->board[j][3]];
			float v_aftestate_next = m_LUTs.H_LUT[i][afterstate_next->board[j][0]][afterstate_next->board[j][1]][afterstate_next->board[j][2]][afterstate_next->board[j][3]];
			m_LUTs.H_LUT[i][afterstate->board[j][0]][afterstate->board[j][1]][afterstate->board[j][2]][afterstate->board[j][3]] += alpha * (reward_next + v_aftestate_next - v_afterstate);
		}

	size_t square_index = 0;
	for (size_t i = 0; i < state->HEIGHT - 1; ++i)
		for (size_t j = 0; j < state->WIDTH - 1; ++j) {
			float v_afterstate = m_LUTs.SQUARE_LUT[square_index][afterstate->board[i][j]][afterstate->board[i][j + 1]][afterstate->board[i + 1][j]][afterstate->board[i + 1][j + 1]];
			float v_aftestate_next = m_LUTs.SQUARE_LUT[square_index][afterstate_next->board[i][j]][afterstate_next->board[i][j + 1]][afterstate_next->board[i + 1][j]][afterstate_next->board[i + 1][j + 1]];
			m_LUTs.SQUARE_LUT[square_index][afterstate->board[i][j]][afterstate->board[i][j + 1]][afterstate->board[i + 1][j]][afterstate->board[i + 1][j + 1]] += alpha * (reward_next + v_aftestate_next - v_afterstate);
			square_index++;
		}

	/*printf("\n\n");
	state->display("State:\n");
	afterstate->display("Afterstate:\n");
	next_state->display("Next_state:\n");
	afterstate_next->display("Afterstate_next:\n");
	State* current_board_state = matrix->getState();
	current_board_state->display("Current board state:\n");*/


	delete afterstate_next;
}

// COMPUTE AFTERSTATE(s, a) -> r, s'
ComputeAfterstateResult Agent::compute_afterstate(const State* const state, const Taction action)
{
	State *afterstate = new State(*state);
	float reward = afterstate->move(action);

	return ComputeAfterstateResult(reward, afterstate);
}

Agent::Agent(Playground* playgroundPtr) : playground(playgroundPtr), matrix{ playground->getTileMatrix() } {}

void Agent::update(const float dt)
{
	playground->update(dt);
	const State* current_state = getState(); // s

	if (!playground->getIsGameOver() && !playground->getIsMoving()) {

		Taction best_action = evaluate(current_state); // a

		MoveResult result = make_move(current_state, best_action);

		State* afterstate = result.afterstate;	// s'
		State* next_state = result.next_state;	// s"
		float reward = result.reward;			// r

		if (learning_enabled) {
			learn_evaluation(current_state, best_action, reward, afterstate, next_state);
			++total_steps;
			//printf("Step: %lld\tReward: %d\n", total_steps, static_cast<int>(reward));
		}

		delete afterstate;
		delete next_state;
	}

	delete current_state;
}

void Agent::displayProgress(int score) const
{
	printf("Games: %lld\tStep: %lld\tReward: %d\n", total_games, total_steps, static_cast<int>(score));
}

void Agent::log(int score)
{
	if (logging_enabled) {
		log_file.open(log_filename, std::ios::app);
		log_file << total_steps << ";" << score << std::endl;
		log_file.close();
	}
}
