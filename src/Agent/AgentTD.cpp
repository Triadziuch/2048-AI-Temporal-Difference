#include "Agent.h"
#include "AgentTD.h"

void AgentTD::loadLatestLUTs()
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


// Constructors / Destructors
AgentTD::AgentTD(Playground* playground) : Agent(playground)
{
	if (resumeLearning)
		loadLatestLUTs();
}

AgentTD::AgentTD(const AgentTD& agent) : Agent(agent) {}

AgentTD::~AgentTD() {}
