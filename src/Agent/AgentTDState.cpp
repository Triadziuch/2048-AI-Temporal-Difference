#include "AgentTDState.h"

const Taction AgentTDState::chooseBestAction(const State* const state, const std::vector<Taction>& available_actions) const // EVALUATE(s, A) -> a
{
	double bestValue = -std::numeric_limits<double>::infinity();
	Taction bestAction = Taction::UP;

	for (const auto& action : available_actions) {
		State* afterstate = new State(*state);

		double value = static_cast<double>(afterstate->move(action));

		std::vector<std::pair<const State*, const State*>> next_states = afterstate->getAllPossibleNextStates();
		const double tile_selection_probability = 1.0 / static_cast<double>(next_states.size());
		const double tile2_probability = 0.9 * tile_selection_probability;
		const double tile4_probability = 0.1 * tile_selection_probability;

		for (const auto& next_state : next_states) {
			value += (tile2_probability * getFunctionValue(next_state.first));
			value += (tile4_probability * getFunctionValue(next_state.second));

			delete next_state.first;
			delete next_state.second;
		}

		if (value > bestValue) {
			bestValue = value;
			bestAction = action;
		}

		delete afterstate;
	}

	return bestAction;
}

void AgentTDState::updateValueFunction(const State* const state, double delta) // LEARN EVALUATION(s, a, r, s', s")
{
	for (size_t i = 0; i < NTuples::NUM_VERTICAL; ++i)
		for (size_t j = 0; j < state->WIDTH; ++j)
			m_LUTs.V_LUT[i][state->board[0][j]][state->board[1][j]][state->board[2][j]][state->board[3][j]] += delta;

	for (size_t i = 0; i < NTuples::NUM_HORIZONTAL; ++i)
		for (size_t j = 0; j < state->HEIGHT; ++j)
			m_LUTs.H_LUT[i][state->board[j][0]][state->board[j][1]][state->board[j][2]][state->board[j][3]] += delta;

	size_t square_index = 0;
	for (size_t i = 0; i < state->HEIGHT - 1; ++i)
		for (size_t j = 0; j < state->WIDTH - 1; ++j) {
			m_LUTs.SQUARE_LUT[square_index][state->board[i][j]][state->board[i][j + 1]][state->board[i + 1][j]][state->board[i + 1][j + 1]] += delta;
			square_index++;
		}
}



// Constructors / Destructors
AgentTDState::AgentTDState(Playground* playground) : AgentTD(playground) {}

AgentTDState::AgentTDState(const AgentTDState& agent) : AgentTD(agent) {}

AgentTDState::~AgentTDState() {}


// Update function
void AgentTDState::update(const float dt)
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


		if (learningEnabled) {
			const double delta = learningRate * (static_cast<double>(transition->reward) + getFunctionValue(transition->nextState) - getFunctionValue(current_state));
			updateValueFunction(current_state, delta);
			++total_steps;
		}

		delete transition;
	}

	delete current_state;
}