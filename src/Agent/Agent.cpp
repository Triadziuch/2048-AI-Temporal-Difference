#pragma once
#include "Agent.h"

// Utility functions
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

inline State* const Agent::getState() const
{
	return matrix->getState();
}

float Agent::generateRandomFloat(float min, float max) const {
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
const double Agent::getBestValueAction(const State* const state, const std::vector<Taction>& available_actions) const
{
	double bestValue = -std::numeric_limits<double>::infinity();

	for (const auto& action : available_actions) {
		State* afterstate = new State(*state);
		double value = static_cast<double>(afterstate->move(action)) + getFunctionValue(afterstate);

		if (value > bestValue)
			bestValue = value;

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

Transition* Agent::move(const State* const state, const Taction action) // MAKE MOVE(s, a) -> r, s', s"
{
	ComputeAfterstateResult afterstate_result = computeAfterstate(state, action);

	if (learningEnabled) 
		matrix->instantMove(action);
	else 
		matrix->move(action);
	
	return new Transition{ afterstate_result.reward, afterstate_result.afterstate, getState() };
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
