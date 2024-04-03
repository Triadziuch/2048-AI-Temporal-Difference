#pragma once
#include <SFML/Graphics.hpp>
#include <stdio.h>

enum class Taction { UP, DOWN, LEFT, RIGHT };

struct Instructions {
	sf::Vector2i m_newPos, m_oldPos;
	bool m_merge;

	Instructions(sf::Vector2i new_pos, sf::Vector2i old_pos, bool merge = false) : m_newPos{ new_pos }, m_oldPos{ old_pos }, m_merge{ merge } {}

	const sf::Vector2i& getNewPos() const { return m_newPos; }
	const sf::Vector2i& getOldPos() const { return m_oldPos; }
};

class State
{
private:
	std::vector <Instructions*>	m_moveInstructions;
	bool willBeOccupied(const sf::Vector2i& pos) const;
	const int merge(const sf::Vector2i& pos, const Taction& direction);
	const int findFreeSpace(const sf::Vector2i& pos, const Taction& direction);
	void addMoveInstructions(const sf::Vector2i& newPos, const sf::Vector2i& oldPos);
	bool m_mergedTiles = false;

public:
	size_t WIDTH{}, HEIGHT{};

	State() = delete;
	State(const size_t& width = 4, const size_t& height = 4);
	State(const State& other);
	~State();

	float move(Taction direction);

	int **board;
	bool **board_moving;
	bool **board_merging;
};

