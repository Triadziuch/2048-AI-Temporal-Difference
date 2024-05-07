#include "AgentQLearning.h"

void AgentQLearning::loadLatestLUTs()
{
	std::string path = logTupleFolder;

	std::string latest_file[4] = {};
	long long int max_games[4]{ 0 }, max_steps[4]{ 0 };

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		std::string filepath = entry.path().string();

		size_t start_game = filepath.find("_") + 1;
		size_t end_game = filepath.find("_", start_game);
		size_t start_steps = end_game + 1;
		size_t end_steps = filepath.find("-", start_steps);
		size_t start_tuple = end_steps + 1;
		size_t end_tuple = filepath.find(".", start_tuple);

		std::string game_str = filepath.substr(start_game, end_game - start_game);
		std::string steps_str = filepath.substr(start_steps, end_steps - start_steps);
		std::string tuple_str = filepath.substr(start_tuple, end_tuple - start_tuple);

		long long int games = std::stoll(game_str);
		long long int steps = std::stoll(steps_str);
		long long int tuple = std::stoll(tuple_str);
		tuple -= 1;

		if (games > max_games[tuple]) {
			max_games[tuple] = games;
			max_steps[tuple] = steps;
			latest_file[tuple] = filepath;
		}
	}

	for (size_t i = 0; i < 4; ++i)
		if (!latest_file[i].empty()) {
			total_games = max_games[i];
			total_steps = max_steps[i];
			m_LUTs[i].load(latest_file[i]);
		}
}

double AgentQLearning::getFunctionValue(const State* const state, const Taction action) const
{
	double value = 0.0f;
	size_t tuple_index = static_cast<size_t>(action);

	for (size_t i = 0; i < NTuples::NUM_VERTICAL; ++i)
		for (size_t j = 0; j < state->WIDTH; ++j)
			value += m_LUTs[tuple_index].V_LUT[i][state->board[0][j]][state->board[1][j]][state->board[2][j]][state->board[3][j]];

	for (size_t i = 0; i < NTuples::NUM_HORIZONTAL; ++i)
		for (size_t j = 0; j < state->HEIGHT; ++j)
			value += m_LUTs[tuple_index].H_LUT[i][state->board[j][0]][state->board[j][1]][state->board[j][2]][state->board[j][3]];

	size_t square_index = 0;
	for (size_t i = 0; i < state->HEIGHT - 1; ++i)
		for (size_t j = 0; j < state->WIDTH - 1; ++j) {
			value += m_LUTs[tuple_index].SQUARE_LUT[square_index][state->board[i][j]][state->board[i][j + 1]][state->board[i + 1][j]][state->board[i + 1][j + 1]];
			square_index++;
		}

	return value;
}

const Taction AgentQLearning::chooseBestAction(const State* const state, const std::vector<Taction> available_actions) const
{
	double bestValue = -std::numeric_limits<double>::infinity();
	Taction bestAction = available_actions[rand() % available_actions.size()];

	for (const auto& action : available_actions) {
		State* afterstate = new State(*state);
		afterstate->move(action);

		const double value = getFunctionValue(afterstate, action);

		if (value > bestValue) {
			bestValue = value;
			bestAction = action;
		}

		delete afterstate;
	}

	return bestAction;
}

const double AgentQLearning::getBestFunctionValue(const State* const state, const std::vector<Taction> available_actions) const
{
	double bestValue = -std::numeric_limits<double>::infinity();

	for (const auto& action : available_actions) {
		State* afterstate = new State(*state);
		afterstate->move(action);

		const double value = getFunctionValue(afterstate, action);

		if (value > bestValue)
			bestValue = value;

		delete afterstate;
	}

	return bestValue;
}

void AgentQLearning::updateValueFunction(const State* const state, double delta, const Taction action)
{
	size_t tuple_index = static_cast<size_t>(action);

	for (size_t i = 0; i < NTuples::NUM_VERTICAL; ++i)
		for (size_t j = 0; j < state->WIDTH; ++j)
			m_LUTs[tuple_index].V_LUT[i][state->board[0][j]][state->board[1][j]][state->board[2][j]][state->board[3][j]] += delta;

	for (size_t i = 0; i < NTuples::NUM_HORIZONTAL; ++i)
		for (size_t j = 0; j < state->HEIGHT; ++j)
			m_LUTs[tuple_index].H_LUT[i][state->board[j][0]][state->board[j][1]][state->board[j][2]][state->board[j][3]] += delta;

	size_t square_index = 0;
	for (size_t i = 0; i < state->HEIGHT - 1; ++i)
		for (size_t j = 0; j < state->WIDTH - 1; ++j) {
			m_LUTs[tuple_index].SQUARE_LUT[square_index][state->board[i][j]][state->board[i][j + 1]][state->board[i + 1][j]][state->board[i + 1][j + 1]] += delta;
			square_index++;
		}
}

AgentQLearning::AgentQLearning(Playground* playground) : Agent(playground) {
	if (resumeLearning)
		loadLatestLUTs();
}

AgentQLearning::AgentQLearning(const AgentQLearning& agent) : Agent(agent) {}

AgentQLearning::~AgentQLearning() {}

void AgentQLearning::update(const float dt)
{
	playground->update(dt);
	State* current_state = getState();

	if (total_games % 1000 == 0 && lastSavedGame != total_games) {
		for (size_t i = 0; i < 4; ++i)
			m_LUTs[i].save(logTupleFolder + logTupleFilename + std::to_string(total_games) + "_" + std::to_string(total_steps) + "-" + std::to_string(i + 1) + logTupleExtension);
		lastSavedGame = total_games;
	}

	if (!playground->getIsGameOver() && matrix->getIsIdle()) {
		std::vector<Taction> possible_actions = current_state->getAvailableMoves();
		Taction best_action;

		if (learningEnabled) {
			if (randomNextAction) {
				if (generateRandomFloat() <= explorationRate)
					best_action = possible_actions[rand() % possible_actions.size()];
				else
					best_action = chooseBestAction(current_state, possible_actions);

				randomNextAction = false;
			}
			else
				best_action = nextAction;
		}
		else
			best_action = chooseBestAction(current_state, possible_actions);

		const Transition* const transition = move(current_state, best_action);

		if (learningEnabled) {
			double v_next = 0.0;
			if (!transition->nextState->isTerminalState()) {
				std::vector<Taction> availableMoves = transition->nextState->getAvailableMoves();
				Taction bestAction;

				if (generateRandomFloat() <= explorationRate)
					bestAction = availableMoves[rand() % availableMoves.size()];
				else
					bestAction = chooseBestAction(transition->nextState, availableMoves);

				v_next = getFunctionValue(transition->nextState, bestAction);
				nextAction = bestAction;
			}
			else
				randomNextAction = true;

			double delta = learningRate * (static_cast<double>(transition->reward) + v_next - getFunctionValue(current_state, best_action));
			updateValueFunction(current_state, delta, best_action);
			++total_steps;
		}

		delete transition;
	}

	delete current_state;
}
