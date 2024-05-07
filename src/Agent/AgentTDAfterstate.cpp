#include "AgentTDAfterstate.h"

const Taction AgentTDAfterstate::chooseBestAction(const State* const state, const std::vector<Taction>& available_actions) const // EVALUATE(s, A) -> a
{
	double bestValue = -std::numeric_limits<double>::infinity();
	Taction bestAction = Taction::UP;

	for (const auto& action : available_actions) {
		State* afterstate = new State(*state);
		double value = static_cast<double>(afterstate->move(action)) + getFunctionValue(afterstate);

		if (value > bestValue) {
			bestValue = value;
			bestAction = action;
		}

		delete afterstate;
	}

	return bestAction;
}

void AgentTDAfterstate::updateValueFunction(const State* const afterstate, double expectedValue, double learningRate) // LEARN EVALUATION(s, a, r, s', s")
{
	// delta = (r + V(s') - V(s)) * alpha
	double delta = (expectedValue - getFunctionValue(afterstate)) * learningRate;

	for (size_t i = 0; i < NTuples::NUM_VERTICAL; ++i)
		for (size_t j = 0; j < afterstate->WIDTH; ++j)
			m_LUTs.V_LUT[i][afterstate->board[0][j]][afterstate->board[1][j]][afterstate->board[2][j]][afterstate->board[3][j]] += delta;

	for (size_t i = 0; i < NTuples::NUM_HORIZONTAL; ++i)
		for (size_t j = 0; j < afterstate->HEIGHT; ++j)
			m_LUTs.H_LUT[i][afterstate->board[j][0]][afterstate->board[j][1]][afterstate->board[j][2]][afterstate->board[j][3]] += delta;

	size_t square_index = 0;
	for (size_t i = 0; i < afterstate->HEIGHT - 1; ++i)
		for (size_t j = 0; j < afterstate->WIDTH - 1; ++j) {
			m_LUTs.SQUARE_LUT[square_index][afterstate->board[i][j]][afterstate->board[i][j + 1]][afterstate->board[i + 1][j]][afterstate->board[i + 1][j + 1]] += delta;
			square_index++;
		}
}



// Constructors / Destructors
AgentTDAfterstate::AgentTDAfterstate(Playground* playground) : AgentTD(playground) {}

AgentTDAfterstate::AgentTDAfterstate(const AgentTDAfterstate& agent): AgentTD(agent) {}

AgentTDAfterstate::~AgentTDAfterstate() {}


// Update function
void AgentTDAfterstate::update(const float dt)
{
	playground->update(dt);
	State* current_state = getState();

	if (total_games % 1000 == 0 && lastSavedGame != total_games) {
		m_LUTs.save(logTupleFolder + logTupleFilename + std::to_string(total_games) + "_" + std::to_string(total_steps) + logTupleExtension);
		lastSavedGame = total_games;
	}

	if (!playground->getIsGameOver() && matrix->getIsIdle()) {

		Taction best_action = Taction::UP;
		std::vector<Taction> possible_actions = current_state->getAvailableMoves();

		if (learningEnabled && generateRandomFloat() < explorationRate && !possible_actions.empty()) {
			size_t random_index = std::rand() % possible_actions.size();
			best_action = possible_actions[random_index];
		}
		else
			best_action = chooseBestAction(current_state, possible_actions);

		const Transition* const transition = move(current_state, best_action);

		double correctActionValue = 0.0;
		if (!transition->nextState->isTerminalState())
			correctActionValue = getBestValueAction(transition->nextState, possible_actions);

		if (learningEnabled) {
			const double delta = (correctActionValue - getFunctionValue(transition->afterstate)) * learningRate;
			updateValueFunction(transition->afterstate, correctActionValue, learningRate);
			++total_steps;
		}

		delete transition;
	}

	delete current_state;
}