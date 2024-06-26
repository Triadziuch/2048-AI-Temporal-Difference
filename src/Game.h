#pragma once
#include "Agent/AgentTDAfterstate.h"
#include "Agent/AgentQLearning.h"
#include "Agent/AgentTDState.h"
#include "Playground.h"
#include "StandardCursor.h"
#include "AssetManager.h"
#include "math.h"
#include <chrono>

using namespace std::chrono;

class Game 
{
private:
	// Window variables
	const std::string			WINDOW_TITLE		= "2048";
	const unsigned int			WINDOW_WIDTH		= 1280;
	const unsigned int			WINDOW_HEIGHT		= 720;
	const unsigned int			FRAMERATE			= 60;
	const bool					VERTICAL_SYNC		= true;
	sf::StandardCursor::TYPE	cursor_type = sf::StandardCursor::NORMAL;
	sf::Vector2i				mouse_pos_window;
	sf::Vector2f				mouse_pos_view;
	sf::RenderWindow*			window;

	// Other variables
	bool isEnd		= false;
	bool isGameOver	= false;
	Playground*	playground;
	AssetManager manager;

	// Clock variables
	sf::Clock dt_clock;
	float dt;

	// Initialization functions 
	void initWindow();
	void initVariables();
	void initAssets();

	time_point<high_resolution_clock> start_time{}, end_time{};
	long long int games = 0;
	double measured_time = 0;

	// AI Agent
	AgentTDState *agent;

public:
	// Constructors / Destructors
	Game();
	virtual ~Game();

	// Run functions
	void run();
	
	// Update functions
	void update();
	void updateMousePositions();
	void updatePollEvents();
	void updateGameOver();

	// Rendering the game
	void render();
};