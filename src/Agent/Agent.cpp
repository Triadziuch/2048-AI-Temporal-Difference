#pragma once
#include "Agent.h"

void Agent::loadConfig()
{
	std::ifstream configFile(configFilename);

	if (configFile.is_open()) {
		std::string line;
		while (std::getline(configFile, line)) {
			if (line[0] != '#' && !line.empty()) {
				size_t delimiter = line.find("=") - 1;
				std::string key = line.substr(0, delimiter);

				size_t value_start = line.find("\"") + 1;
				size_t value_end = line.find("\"", value_start);
				std::string value = line.substr(value_start, value_end - value_start);

				if (key == "loggingEnabled") 
					loggingEnabled = value == "true";
				else if (key == "learningEnabled") {
					learningEnabled = value == "true";

					if (!learningEnabled) {
						matrix->setTimeSpawning(0.25f);
						matrix->setTimeMoving(0.25f);
						matrix->setTimeMerging(0.25f);
					}
					else {
						matrix->setTimeSpawning(0.0f);
						matrix->setTimeMoving(0.0f);
						matrix->setTimeMerging(0.0f);
					}
				}
					
				else if (key == "resumeLearning")
					resumeLearning = value == "true";
				else if (key == "explorationRate")
					explorationRate = std::stod(value);
				else if (key == "learningRate")
					learningRate = std::stod(value);
			}
		}
	}
	else
		std::cerr << "Could not open config file: " << configFilename << std::endl;
	configFile.close();
}

// Utility functions
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

inline float Agent::generateRandomFloat(float min, float max) const {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dis(min, max);

	return dis(gen);
}



// Logging functions
void Agent::log(int score)
{
	if (loggingEnabled)
		logFile << total_games << ";" << total_steps << ";" << score << ";" << matrix->getMaxType() << std::endl;
}

void Agent::displayProgress(int score) const
{
	if (loggingEnabled)
		printf("Games: %lld\tStep: %lld\tReward: %d\tMax type: %d\n", total_games, total_steps, static_cast<int>(score), matrix->getMaxType());
}

// Learning functions
Taction Agent::chooseBestAction(const State* const state) const // EVALUATE(s) -> a
{
	double bestValue = -std::numeric_limits<double>::infinity();
	Taction bestAction = Taction::UP;

	for (size_t i = 0; i < 4; ++i) {
		State* afterstate = new State(*state);
		Taction action = static_cast<Taction>(i);

		if (afterstate->isMovePossible(action)) {
			double value = static_cast<double>(afterstate->move(action)) + getFunctionValue(afterstate);

			if (value > bestValue) {
				bestValue = value;
				bestAction = action;
			}
		}

		delete afterstate;
	}

	return bestAction;
}

double Agent::getBestValueAction(const State* const state) const
{
	double bestValue = -std::numeric_limits<double>::infinity();

	for (size_t i = 0; i < 4; ++i) {
		State* afterstate = new State(*state);
		Taction action = static_cast<Taction>(i);

		if (afterstate->isMovePossible(action)) {
			double value = static_cast<double>(afterstate->move(action)) + getFunctionValue(afterstate);

			if (value > bestValue)
				bestValue = value;
		}

		delete afterstate;
	}

	return bestValue;
}

double Agent::getFunctionValue(const State* const state) const
{
	double value = 0.0f;

	for (size_t i = 0; i < NTuples::NUM_VERTICAL; ++i)
		for (size_t j = 0; j < state->WIDTH; ++j)
			value += m_LUTs.V_LUT[i][state->board[0][j]][state->board[1][j]][state->board[2][j]][state->board[3][j]];

	for (size_t i = 0; i < NTuples::NUM_HORIZONTAL; ++i)
		for (size_t j = 0; j < state->HEIGHT; ++j)
			value += m_LUTs.H_LUT[i][state->board[j][0]][state->board[j][1]][state->board[j][2]][state->board[j][3]];

	size_t square_index = 0;
	for (size_t i = 0; i < state->HEIGHT - 1; ++i)
		for (size_t j = 0; j < state->WIDTH - 1; ++j) {
			value += m_LUTs.SQUARE_LUT[square_index][state->board[i][j]][state->board[i][j + 1]][state->board[i + 1][j]][state->board[i + 1][j + 1]];
			square_index++;
		}

	return value;
}

void Agent::updateValueFunction(const State* const afterstate, double expectedValue, double learningRate)
{
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

Transition Agent::move(const State* const state, const Taction action) // MAKE MOVE(s, a) -> r, s', s"
{
	Transition transition;

	ComputeAfterstateResult afterstate_result = computeAfterstate(state, action);
	transition.reward = afterstate_result.reward;
	transition.afterstate = afterstate_result.afterstate;

	if (learningEnabled) 
		matrix->instantMove(action);
	else 
		matrix->move(action);
	

	transition.nextState = getState();

	return transition;
}

ComputeAfterstateResult Agent::computeAfterstate(const State* const state, const Taction action) // COMPUTE AFTERSTATE(s, a) -> r, s'
{
	State *afterstate = new State(*state);
	int reward = afterstate->move(action);

	return ComputeAfterstateResult(reward, afterstate);
}



// Constructors / Destructors
Agent::Agent(Playground* playgroundPtr) : playground(playgroundPtr), matrix{ playground->getTileMatrix() } {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	loadConfig();
	if (resumeLearning)
		loadLatestLUTs();
	
	if (loggingEnabled)
		logFile.open(logFilename, std::ios::app);
}

Agent::Agent(const Agent& other) : 
	playground(other.playground), matrix{ other.matrix }, m_LUTs{ other.m_LUTs }, total_games{ other.total_games }, total_steps{ other.total_steps } 
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Agent::~Agent() 
{
	if (logFile.is_open())
		logFile.close();
}



// Update function
void Agent::update(const float dt)
{
	playground->update(dt);
	State* current_state = getState();

	if (total_games % 1000 == 0 && lastSavedGame != total_games) {
		m_LUTs.save(logTupleFolder + logTupleFilename + std::to_string(total_games) + "_" + std::to_string(total_steps) + logTupleExtension);
		lastSavedGame = total_games;
	}

	if (!playground->getIsGameOver() && matrix->getIsIdle()) {

		Taction best_action = Taction::UP;

		if (learningEnabled && generateRandomFloat() < explorationRate) {
			std::vector<Taction> &possible_actions = current_state->getAvailableMoves();

			if (!possible_actions.empty()) {
				size_t random_index = std::rand() % possible_actions.size();
				best_action = possible_actions[random_index];
			}
		}
		else 
			best_action = chooseBestAction(current_state);
			
		Transition transition = move(current_state, best_action);

		double correctActionValue = 0.0;
		if (!transition.nextState->isTerminalState())
			correctActionValue = getBestValueAction(transition.nextState);

		if (learningEnabled) {
			updateValueFunction(transition.afterstate, correctActionValue, learningRate);
			++total_steps;
		}

		delete transition.afterstate;
		delete transition.nextState;
	}

	delete current_state;
}



// Public functions
void Agent::episodeEnded(int score)
{
	displayProgress(score);
	log(score);
	total_games++;
}

long long int Agent::getGamesCount() const
{
	return total_games;
}

long long int Agent::getStepsCount() const
{
	return total_steps;
}

bool Agent::getIsLearning() const
{
	return learningEnabled;
}
