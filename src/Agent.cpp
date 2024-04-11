#pragma once
#include "Agent.h"

void LUTContainer::save(const std::string& filename) const
{
	std::ofstream file;
	file.open(filename);

	if (file.good()) {
		std::stringstream buffer;

		// Zapisz dane V_LUT
		for (size_t i = 0; i < NUM_VERTICAL; ++i) 
			for (size_t j = 0; j < MAX_TYPE + 1; ++j) 
				for (size_t k = 0; k < MAX_TYPE + 1; ++k) 
					for (size_t l = 0; l < MAX_TYPE + 1; ++l) 
						for (size_t m = 0; m < MAX_TYPE + 1; ++m) 
							buffer << V_LUT[i][j][k][l][m] << ' ';


		// Zapisz dane H_LUT
		for (size_t i = 0; i < NUM_HORIZONTAL; ++i) 
			for (size_t j = 0; j < MAX_TYPE + 1; ++j) 
				for (size_t k = 0; k < MAX_TYPE + 1; ++k) 
					for (size_t l = 0; l < MAX_TYPE + 1; ++l) 
						for (size_t m = 0; m < MAX_TYPE + 1; ++m) 
							buffer << H_LUT[i][j][k][l][m] << ' ';


		// Zapisz dane SQUARE_LUT
		for (size_t i = 0; i < NUM_SQUARES; ++i) 
			for (size_t j = 0; j < MAX_TYPE + 1; ++j) 
				for (size_t k = 0; k < MAX_TYPE + 1; ++k) 
					for (size_t l = 0; l < MAX_TYPE + 1; ++l) 
						for (size_t m = 0; m < MAX_TYPE + 1; ++m) 
							buffer << SQUARE_LUT[i][j][k][l][m] << ' ';


		// Zapisz dane z bufora do pliku
		file << buffer.str();

		file.close();
	}
}

void LUTContainer::load(const std::string& filename)
{
	std::ifstream file;
	file.open(filename);

	if (file.good()) {
		for (size_t i = 0; i < NUM_VERTICAL; ++i) {
			for (size_t j = 0; j < MAX_TYPE + 1; ++j) {
				for (size_t k = 0; k < MAX_TYPE + 1; ++k) {
					for (size_t l = 0; l < MAX_TYPE + 1; ++l) {
						for (size_t m = 0; m < MAX_TYPE + 1; ++m) {
							file >> V_LUT[i][j][k][l][m];
						}
					}
				}
			}
		}

		for (size_t i = 0; i < NUM_HORIZONTAL; ++i) {
			for (size_t j = 0; j < MAX_TYPE + 1; ++j) {
				for (size_t k = 0; k < MAX_TYPE + 1; ++k) {
					for (size_t l = 0; l < MAX_TYPE + 1; ++l) {
						for (size_t m = 0; m < MAX_TYPE + 1; ++m) {
							file >> H_LUT[i][j][k][l][m];
						}
					}
				}
			}
		}

		for (size_t i = 0; i < NUM_SQUARES; ++i) {
			for (size_t j = 0; j < MAX_TYPE + 1; ++j) {
				for (size_t k = 0; k < MAX_TYPE + 1; ++k) {
					for (size_t l = 0; l < MAX_TYPE + 1; ++l) {
						for (size_t m = 0; m < MAX_TYPE + 1; ++m) {
							file >> SQUARE_LUT[i][j][k][l][m];
						}
					}
				}
			}
		}

		file.close();
	}
}

void Agent::loadLatestLUTs()
{
	std::string path = logTupleFolder;

	std::string latest_file = "";
	long long int max_games = 0, max_steps = 0;

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		std::string filepath = entry.path().string();

		size_t start_game = filepath.find("_") + 1;
		size_t end_game = filepath.find("_", start_game);
		size_t start_steps = end_game + 1;
		size_t end_steps = filepath.find(".", start_steps);

		std::string game_str = filepath.substr(start_game, end_game - start_game);
		std::string steps_str = filepath.substr(start_steps, end_steps - start_steps);

		long long int games = std::stoll(game_str);
		long long int steps = std::stoll(steps_str);

		if (games > max_games) {
			max_games = games;
			max_steps = steps;
			latest_file = filepath;
		}
	}

	if (!latest_file.empty()) {
		total_games = max_games;
		total_steps = max_steps;
		m_LUTs.load(latest_file);
	}
}



inline State* const Agent::getState() const
{
	return matrix->getState();
}

float Agent::generateRandomFloat(float min, float max) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dis(min, max);

	return dis(gen);
}

float Agent::get_state_value(const State* const state) const
{
    float value = 0.0f;

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

		float reward = new_state->move(action);
		
		float value = get_state_value(new_state);
		if (value + reward > best_value) {
			best_value = value + reward;
			best_action = action;
		}
		delete new_state;
	}

	return best_action;
}

float Agent::evaluate_getbestactionvalue(const State* const state) const
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
			best_value = value + reward;
			best_action = action;
		}
		delete new_state;
	}

	return best_value;
}

double Agent::getBestValueAction(const State* const state, Taction& best_action) const
{
	float bestValue = -std::numeric_limits<float>::infinity();

	for (size_t i = 0; i < 4; ++i) {
		State* afterstate = new State(*state);
		Taction action = static_cast<Taction>(i);

		if (afterstate->isMovePossible(action)) {

			double value = static_cast<double>(afterstate->move(action)) + get_state_value(afterstate);

			if (value > bestValue)
				bestValue = value;
		}

		delete afterstate;
	}

	return bestValue;
}

void Agent::updateValueFunction(const State* const afterstate, double expectedValue, double learningRate)
{
	double val = get_state_value(afterstate);
	double error = expectedValue - val;
	double delta = error * learningRate;

	for (size_t i = 0; i < LUTContainer::NUM_VERTICAL; ++i)
		for (size_t j = 0; j < afterstate->WIDTH; ++j) 
			m_LUTs.V_LUT[i][afterstate->board[0][j]][afterstate->board[1][j]][afterstate->board[2][j]][afterstate->board[3][j]] += delta;
		

	for (size_t i = 0; i < LUTContainer::NUM_HORIZONTAL; ++i)
		for (size_t j = 0; j < afterstate->HEIGHT; ++j) 
			m_LUTs.H_LUT[i][afterstate->board[j][0]][afterstate->board[j][1]][afterstate->board[j][2]][afterstate->board[j][3]] += delta;

	size_t square_index = 0;
	for (size_t i = 0; i < afterstate->HEIGHT - 1; ++i)
		for (size_t j = 0; j < afterstate->WIDTH - 1; ++j) {
			m_LUTs.SQUARE_LUT[square_index][afterstate->board[i][j]][afterstate->board[i][j + 1]][afterstate->board[i + 1][j]][afterstate->board[i + 1][j + 1]] += delta;
			square_index++;
		}
}

// MAKE MOVE(s, a) -> r, s', s"
Transition Agent::make_move(const State* const state, const Taction action)
{
	// current_state = s -> s'
	// state = s -> s"

	Transition result;

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

	result.nextState = matrix->getState(); // s"

	/*if (total_steps % 500 == 0) {
		printf("\n\n");
		state->display("State:\n");
		result.afterstate->display("Afterstate:\n");
		result.next_state->display("Next_state:\n");
	}*/

	return result;
}


// COMPUTE AFTERSTATE(s, a) -> r, s'
ComputeAfterstateResult Agent::compute_afterstate(const State* const state, const Taction action)
{
	State *afterstate = new State(*state);
	float reward = afterstate->move(action);

	return ComputeAfterstateResult(reward, afterstate);
}

Agent::Agent(Playground* playgroundPtr) : playground(playgroundPtr), matrix{ playground->getTileMatrix() } {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	if (resumeLearning)
		loadLatestLUTs();
}

void Agent::update(const float dt)
{
	playground->update(dt);
	State* current_state = getState(); // s

	if (!playground->getIsGameOver() && !playground->getIsMoving()) {

		//Taction best_action = evaluate(current_state); // a
		Taction best_action = Taction::UP;

		if (generateRandomFloat() < explorationRate) {
			std::vector<Taction> possible_actions;
			for (size_t i = 0; i < 4; ++i) {
				Taction action = static_cast<Taction>(i);
				if (current_state->isMovePossible(action))
					possible_actions.push_back(action);
			}

			if (!possible_actions.empty()) {
				size_t random_index = std::rand() % possible_actions.size();
				best_action = possible_actions[random_index];
			}
		}
		else
			best_action = evaluate(current_state); // a

		Transition transition = make_move(current_state, best_action);

		double correctActionValue = 0.0;
		if (!transition.nextState->isTerminalState())
			correctActionValue = getBestValueAction(transition.nextState, best_action);

		if (true) {
			updateValueFunction(transition.afterstate, correctActionValue, 0.01);
			++total_steps;
		}

		delete transition.afterstate;
		delete transition.nextState;
	}

	if (total_games % 1000 == 0 && lastSavedGame != total_games) {
		m_LUTs.save(logTupleFolder + logTupleFilename + std::to_string(total_games) + "_" + std::to_string(total_steps) + logTupleExtension);
		lastSavedGame = total_games;
	}

	delete current_state;
}

void Agent::displayProgress(int score) const
{
	printf("Games: %lld\tStep: %lld\tReward: %d\tMax type: %d\n", total_games, total_steps, static_cast<int>(score), matrix->getMaxType());
}

void Agent::log(int score)
{
	if (loggingEnabled) {
		logFile.open(logFilename, std::ios::app);
		logFile << total_games << ";" << total_steps << ";" << score << ";" << matrix->getMaxType() << std::endl;
		logFile.close();
	}
}
