#pragma once
#include "TileMatrix.h"

void TileMatrix::initTextures()
{
	for (int i = 1, k = 0; i < 13; i++, ++k)
		m_textures[k] = &AssetManager::GetTexture("bin/Textures/" + std::to_string(static_cast<int>(pow(2.f, static_cast<double>(i)))) + ".png");
}

int TileMatrix::findID(int type) const
{
	int ID = 0;
	while (type != 1) {
		type /= 2;
		++ID;
	}
	return ID - 1;
}

sf::Vector2f TileMatrix::calculateTilePos(const sf::Vector2i& pos) const
{
	sf::Vector2f tilePos{ m_playgroundPosition };

	tilePos.x += *m_outerEdgeWidth + *m_innerEdgeWidth * pos.x + *m_tileWidth * pos.x + *m_tileWidth / 2.f;
	tilePos.y += *m_outerEdgeWidth + *m_innerEdgeWidth * pos.y + *m_tileWidth * pos.y + *m_tileWidth / 2.f;

	return tilePos;
}

int TileMatrix::findFreeSpace(const sf::Vector2i& pos, const Taction& direction)
{
	if (direction == Taction::UP) {
		const int new_y = merge(pos, direction);
		if (new_y != -1)
			return new_y;
		else {
			for (size_t i = 0; i < pos.y; ++i)
				if ((m_matrix[pos.x][i] == nullptr || m_matrix[pos.x][i]->getIsMoving()) && !willBeOccupied(sf::Vector2i(pos.x, i)))
					return i;
			return pos.y;
		}
	}
	else if (direction == Taction::DOWN) {
		const int new_y = merge(pos, direction);
		if (new_y != -1)
			return new_y;
		else {
			for (int i = m_matrixHeight - 1; i > pos.y; --i)
				if ((m_matrix[pos.x][i] == nullptr || m_matrix[pos.x][i]->getIsMoving()) && !willBeOccupied(sf::Vector2i(pos.x, i)))
					return i;
			return pos.y;
		}
	}
	else if (direction == Taction::LEFT) {
		const int new_x = merge(pos, direction);
		if (new_x != -1)
			return new_x;
		else {
			for (size_t i = 0; i < pos.x; ++i)
				if ((m_matrix[i][pos.y] == nullptr || m_matrix[i][pos.y]->getIsMoving()) && !willBeOccupied(sf::Vector2i(i, pos.y)))
					return i;
			return pos.x;
		}
	}
	else if (direction == Taction::RIGHT) {
		const int new_x = merge(pos, direction);
		if (new_x != -1)
			return new_x;
		else {
			for (size_t i = m_matrixWidth - 1; i > pos.x; --i)
				if ((m_matrix[i][pos.y] == nullptr || m_matrix[i][pos.y]->getIsMoving()) && !willBeOccupied(sf::Vector2i(i, pos.y)))
					return i;
			return pos.x;
		}
	}
	else
		return -1;
}

int TileMatrix::merge(const sf::Vector2i& pos, const Taction& direction)
{
	if (direction == Taction::UP) {
		Tile& tile = *m_matrix[pos.x][pos.y];
		if (tile == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			Tile& neighbourMovingTile = *m_matrix[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().x == pos.x &&
				neighbourMovingTile == tile &&
				neighbourMovingTile.getMerging() == false &&
				tile.getMerging() == false) {

				checkWin(tile);
				m_mergedTiles = true;
				neighbourMovingTile.setIsMerging(true);
				tile.setIsMerging(true);
				return m_moveInstructions.back()->getNewPos().y;
			}
		}

		// Merge if only one is moving
		for (int i = pos.y - 1; i >= 0; --i)
			if (m_matrix[pos.x][i] != nullptr) {
				if (*m_matrix[pos.x][i] == tile && !m_matrix[pos.x][i]->getMerging() && !tile.getMerging()) {

					checkWin(tile);
					m_mergedTiles = true;
					m_matrix[pos.x][i]->setIsMerging(true);
					tile.setIsMerging(true);
					return i;
				}
				else
					break;
			}

		m_mergedTiles = false;
		return -1;
	}
	else if (direction == Taction::DOWN) {
		Tile& tile = *m_matrix[pos.x][pos.y];
		if (tile == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			Tile& neighbourMovingTile = *m_matrix[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().x == pos.x &&
				neighbourMovingTile == tile &&
				neighbourMovingTile.getMerging() == false &&
				tile.getMerging() == false) {

				checkWin(tile);
				m_mergedTiles = true;
				neighbourMovingTile.setIsMerging(true);
				tile.setIsMerging(true);
				return m_moveInstructions.back()->getNewPos().y;
			}
		}

		// Merge if only one is moving
		for (int i = pos.y + 1; i < m_matrixHeight; ++i)
			if (m_matrix[pos.x][i] != nullptr) {
				if (*m_matrix[pos.x][i] == tile && !m_matrix[pos.x][i]->getMerging() && !tile.getMerging()) {

					checkWin(tile);
					m_mergedTiles = true;
					m_matrix[pos.x][i]->setIsMerging(true);
					tile.setIsMerging(true);
					return i;
				}
				else
					break;
			}

		m_mergedTiles = false;
		return -1;
	}
	else if (direction == Taction::LEFT) {
		Tile& tile = *m_matrix[pos.x][pos.y];
		if (tile == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			Tile& neighbourMovingTile = *m_matrix[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().y == pos.y &&
				neighbourMovingTile == tile &&
				neighbourMovingTile.getMerging() == false &&
				tile.getMerging() == false) {

				checkWin(tile);

				m_mergedTiles = true;
				neighbourMovingTile.setIsMerging(true);
				tile.setIsMerging(true);
				return m_moveInstructions.back()->getNewPos().x;
			}
		}

		// Merge if only one is moving
		for (int i = pos.x - 1; i >= 0; --i)
			if (m_matrix[i][pos.y] != nullptr) {
				if (*m_matrix[i][pos.y] == tile && !m_matrix[i][pos.y]->getMerging() && !tile.getMerging()) {

					checkWin(tile);
					m_mergedTiles = true;
					m_matrix[i][pos.y]->setIsMerging(true);
					tile.setIsMerging(true);
					return i;
				}
				else
					break;
			}

		m_mergedTiles = false;
		return -1;
	}
	else if (direction == Taction::RIGHT) {
		Tile& tile = *m_matrix[pos.x][pos.y];
		if (tile == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			Tile& neighbourMovingTile = *m_matrix[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().y == pos.y &&
				neighbourMovingTile == tile &&
				neighbourMovingTile.getMerging() == false &&
				tile.getMerging() == false) {

				checkWin(tile);

				m_mergedTiles = true;
				neighbourMovingTile.setIsMerging(true);
				tile.setIsMerging(true);
				return m_moveInstructions.back()->getNewPos().x;
			}
		}

		// Merge if only one is moving
		for (int i = pos.x + 1; i < m_matrixWidth; ++i)
			if (m_matrix[i][pos.y] != nullptr) {
				if (*m_matrix[i][pos.y] == tile && !m_matrix[i][pos.y]->getMerging() && !tile.getMerging()) {

					checkWin(tile);
					m_mergedTiles = true;
					m_matrix[i][pos.y]->setIsMerging(true);
					tile.setIsMerging(true);
					return i;
				}
				else
					break;
			}

		m_mergedTiles = false;
		return -1;
	}
	else
		return -1;
}

void TileMatrix::checkWin(const Tile& tile)
{
	if (tile == 1024 && !m_isWin) {
		m_isWin = true;

		for (size_t x = 0; x < m_matrixWidth; ++x)
			for (size_t y = 0; y < m_matrixHeight; ++y)
				if (m_matrix[x][y])
					m_matrix[x][y]->startWin();
	}
}

void TileMatrix::move(const Taction& direction)
{
	if (m_state == Tstate::MOVING) {
		endMove();
		endMerge();
	}
	else if (m_state == Tstate::MERGING)
		endMerge();

	if (direction == Taction::UP) {
		for (int i = 0; i < m_matrixWidth; ++i)
			for (int j = 0; j < m_matrixHeight; ++j)
				if (m_matrix[i][j] != nullptr) {
					const sf::Vector2i new_pos{ i, findFreeSpace(sf::Vector2i(i, j), direction) };
					const int distance = j - new_pos.y;

					if (distance > 0) {
						const float pixel_distance = static_cast<float>(distance) * (*m_innerEdgeWidth + *m_tileWidth);
						m_matrix[i][j]->smoothMove(sf::Vector2f(0.f, -pixel_distance), m_timeMovingMax);
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
					}
				}
	}
	else if (direction == Taction::DOWN) {
		for (int i = 0; i < m_matrixWidth; ++i)
			for (int j = m_matrixHeight - 1; j >= 0; --j)
				if (m_matrix[i][j] != nullptr) {
					const sf::Vector2i new_pos{ i, findFreeSpace(sf::Vector2i(i, j), direction) };
					const int distance = new_pos.y - j;

					if (distance > 0) {
						const float pixel_distance = static_cast<float>(distance) * (*m_innerEdgeWidth + *m_tileWidth);
						m_matrix[i][j]->smoothMove(sf::Vector2f(0.f, pixel_distance), m_timeMovingMax);
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
					}
				}
	}
	else if (direction == Taction::LEFT) {
		for (int j = 0; j < m_matrixHeight; ++j)
			for (int i = 0; i < m_matrixWidth; ++i)
				if (m_matrix[i][j] != nullptr) {
					const sf::Vector2i new_pos{ findFreeSpace(sf::Vector2i(i, j), direction), j };
					const int distance = i - new_pos.x;

					if (distance > 0) {
						const float pixel_distance = static_cast<float>(distance) * (*m_innerEdgeWidth + *m_tileWidth);
						m_matrix[i][j]->smoothMove(sf::Vector2f(-pixel_distance, 0.f), m_timeMovingMax);
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
					}
				}
	}
	else if (direction == Taction::RIGHT) {
		for (int j = 0; j < m_matrixHeight; ++j)
			for (int i = m_matrixWidth - 1; i >= 0; --i)
				if (m_matrix[i][j] != nullptr) {
					const sf::Vector2i new_pos{ findFreeSpace(sf::Vector2i(i, j), direction), j };
					const int distance = new_pos.x - i;

					if (distance > 0) {
						const float pixel_distance = static_cast<float>(distance) * (*m_innerEdgeWidth + *m_tileWidth);
						m_matrix[i][j]->smoothMove(sf::Vector2f(pixel_distance, 0.f), m_timeMovingMax);
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
					}
				}
	}
}

void TileMatrix::instantMove(const Taction& direction)
{
	if (m_state != Tstate::IDLE) {
		endMove();
		endMerge();
	}

	if (direction == Taction::UP) {
		for (int i = 0; i < m_matrixWidth; ++i)
			for (int j = 0; j < m_matrixHeight; ++j)
				if (m_matrix[i][j] != nullptr) {
					const sf::Vector2i new_pos{ i, findFreeSpace(sf::Vector2i(i, j), direction) };
					const int distance = j - new_pos.y;

					if (distance > 0) {
						const float pixel_distance = static_cast<float>(distance) * (*m_innerEdgeWidth + *m_tileWidth);
						m_matrix[i][j]->instantMove(sf::Vector2f(0.f, -pixel_distance));
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
					}
				}
	}
	else if (direction == Taction::DOWN) {
		for (int i = 0; i < m_matrixWidth; ++i)
			for (int j = m_matrixHeight - 1; j >= 0; --j)
				if (m_matrix[i][j] != nullptr) {
					const sf::Vector2i new_pos{ i, findFreeSpace(sf::Vector2i(i, j), direction) };
					const int distance = new_pos.y - j;

					if (distance > 0) {
						const float pixel_distance = static_cast<float>(distance) * (*m_innerEdgeWidth + *m_tileWidth);
						m_matrix[i][j]->instantMove(sf::Vector2f(0.f, pixel_distance));
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
					}
				}
	}
	else if (direction == Taction::LEFT) {
		for (int j = 0; j < m_matrixHeight; ++j)
			for (int i = 0; i < m_matrixWidth; ++i)
				if (m_matrix[i][j] != nullptr) {
					const sf::Vector2i new_pos{ findFreeSpace(sf::Vector2i(i, j), direction), j };
					const int distance = i - new_pos.x;

					if (distance > 0) {
						const float pixel_distance = static_cast<float>(distance) * (*m_innerEdgeWidth + *m_tileWidth);
						m_matrix[i][j]->instantMove(sf::Vector2f(-pixel_distance, 0.f));
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
					}
				}
	}
	else if (direction == Taction::RIGHT) {
		for (int j = 0; j < m_matrixHeight; ++j)
			for (int i = m_matrixWidth - 1; i >= 0; --i)
				if (m_matrix[i][j] != nullptr) {
					const sf::Vector2i new_pos{ findFreeSpace(sf::Vector2i(i, j), direction), j };
					const int distance = new_pos.x - i;

					if (distance > 0) {
						const float pixel_distance = static_cast<float>(distance) * (*m_innerEdgeWidth + *m_tileWidth);
						m_matrix[i][j]->instantMove(sf::Vector2f(pixel_distance, 0.f));
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
					}
				}
	}

	endMove();
	endMerge();
}

void TileMatrix::addMoveInstructions(const sf::Vector2i& newPos, const sf::Vector2i& oldPos)
{
	m_state = Tstate::MOVING;
	m_matrix[oldPos.x][oldPos.y]->setIsMoving(true);
	m_moveInstructions.push_back(new MoveInstructions{ newPos, oldPos, m_mergedTiles });
	m_mergedTiles = false;
}

bool TileMatrix::willBeOccupied(const sf::Vector2i& pos) const
{
	for (const auto& instruction : m_moveInstructions)
		if (instruction->getNewPos() == pos)
			return true;
	return false;
}

void TileMatrix::endMove()
{
	if (m_state == Tstate::MOVING) {

		m_addedScore = 0;
		for (size_t i = 0; i < m_moveInstructions.size(); ++i) {
			if (m_moveInstructions[i]->m_merge) {
				m_state = Tstate::MERGING;
				const sf::Vector2i new_pos = m_moveInstructions[i]->getNewPos();
				const sf::Vector2i old_pos = m_moveInstructions[i]->getOldPos();
				delete m_matrix[old_pos.x][old_pos.y];
				m_matrix[old_pos.x][old_pos.y] = nullptr;

				m_tilesToMerge.push_back(m_matrix[new_pos.x][new_pos.y]);
				m_matrix[new_pos.x][new_pos.y] = new Tile(m_tilesToMerge.back()->getType() * 2, m_textures[findID(m_tilesToMerge.back()->getType() * 2)], m_scale, calculateTilePos(new_pos), m_movementManager);
				m_matrix[new_pos.x][new_pos.y]->startMerging();
				--m_tiles;

				m_addedScore += m_tilesToMerge.back()->getType() * 2;
			}
			else {
				const sf::Vector2i new_pos = m_moveInstructions[i]->getNewPos();
				const sf::Vector2i old_pos = m_moveInstructions[i]->getOldPos();
				m_matrix[new_pos.x][new_pos.y] = m_matrix[old_pos.x][old_pos.y];
				m_matrix[old_pos.x][old_pos.y] = nullptr;

				m_matrix[new_pos.x][new_pos.y]->setIsMoving(false);
				m_matrix[new_pos.x][new_pos.y]->setIsMerging(false);
			}
		}
		
		for (size_t i = 0; i < m_moveInstructions.size(); ++i)
			delete m_moveInstructions[i];
		m_moveInstructions.clear();

		if (m_state == Tstate::MOVING) {
			m_state = Tstate::IDLE;
			m_timeMoving = 0.f;
		}

		spawn();
	}
}

void TileMatrix::endMerge()
{
	if (m_state == Tstate::MERGING) {
		m_timeMoving = 0.f;
		m_timeMerging = 0.f;

		for (size_t i = 0; i < m_tilesToMerge.size(); ++i)
			delete m_tilesToMerge[i];
		m_tilesToMerge.clear();

		m_state = Tstate::IDLE;
	}
}

TileMatrix::TileMatrix(const float* scale, const float* outer, const float* inner, const float* tileWidth, const sf::Vector2f& playgroundPos) :
	m_scale{ scale }, m_outerEdgeWidth{ outer }, m_innerEdgeWidth{ inner }, m_tileWidth{ tileWidth }, m_playgroundPosition{ playgroundPos }
{
	m_movementManager = new MovementManager();
	m_movementContainer = MovementContainer::getInstance();

	auto routine = m_movementManager->createScalingRoutine("TILE_SPAWNING");
	routine->addScaling(new scalingInfo(sf::Vector2f(0.1f, 0.1f) * *m_scale, sf::Vector2f(1.f, 1.f) * *m_scale, m_timeSpawningMax, easeFunctions::getFunction(easeFunctions::OUT_QUAD), false, 0.f, 0.f));

	routine = m_movementManager->createScalingRoutine("TILE_MERGING");
	routine->addScaling(new scalingInfo(sf::Vector2f(0.1f, 0.1f) * *m_scale, sf::Vector2f(1.f, 1.f) * *m_scale, m_timeMergingMax, easeFunctions::getFunction(easeFunctions::OUT_BACK), false, 0.f, 0.f));

	initTextures();
}

TileMatrix::~TileMatrix()
{
	for (int i = 0; i < m_matrixHeight; ++i)
		for (int j = 0; j < m_matrixWidth; ++j)
			if (m_matrix[i][j])
				delete m_matrix[i][j];
}

void TileMatrix::update(float dt)
{
	m_movementManager->update(dt);
	m_movementContainer->update(dt);

	for (size_t i = 0; i < m_matrixHeight; ++i)
		for (size_t j = 0; j < m_matrixWidth; ++j)
			if (m_matrix[i][j])
				m_matrix[i][j]->update(dt);

	if (!m_isGameOver) {
		if (m_state == Tstate::MOVING)
			updateMove(dt);

		if (m_state == Tstate::MERGING)
			updateMerge(dt);
	}
}

void TileMatrix::updateMove(float dt)
{
	if (m_timeMoving < m_timeMovingMax)
		m_timeMoving += dt;
	else
		endMove();
}

void TileMatrix::updateMerge(float dt)
{
	if (m_timeMerging < m_timeMergingMax) {
		m_timeMerging += dt;
	}
	else
		endMerge();
}

void TileMatrix::spawn(const int amount)
{
	for (int i = 0; i < amount; ++i) {
		sf::Vector2i pos{ rand() % 4, rand() % 4 };

		while (m_matrix[pos.x][pos.y])
			pos = { rand() % 4, rand() % 4 };

		// Generate random tile, 10% to be type of 4, 90% to be type of 2
		if (rand() % 10 == 0)
			addTile(pos, 4);
		else
			addTile(pos, 2);
	}
}

void TileMatrix::addTile(const sf::Vector2i& pos, const int type)
{
	if (pos.x > m_matrixWidth - 1 || pos.y > m_matrixHeight - 1)
		printf("TileMatrix::addTile ERROR: Tile spawning position out of bounds\n");
	else {

		m_matrix[pos.x][pos.y] = new Tile(type, m_textures[findID(type)], m_scale, calculateTilePos(pos), m_movementManager);
		m_matrix[pos.x][pos.y]->startSpawning();
		++m_tiles;

		m_isGameOver = isGameOver();
	}
}

void TileMatrix::clearBoard()
{
	for (size_t i = 0; i < m_moveInstructions.size(); ++i)
		delete m_moveInstructions[i];
	m_moveInstructions.clear();

	for (size_t i = 0; i < m_tilesToMerge.size(); ++i)
		delete m_tilesToMerge[i];
	m_tilesToMerge.clear();

	for (int i = 0; i < m_matrixHeight; ++i)
		for (int j = 0; j < m_matrixWidth; ++j) {
			if (m_matrix[i][j])
				delete m_matrix[i][j];
			m_matrix[i][j] = nullptr;
		}

	m_addedScore = 0;
	m_timeMoving = 0.f;
	m_timeMerging = 0.f;
	m_mergedTiles = false;
	m_isGameOver = false;
	m_tiles = 0;
}

bool TileMatrix::isGameOver()
{
	if (m_tiles >= m_maxTiles){

		for (size_t i = 0; i < m_matrixHeight - 1; ++i)
			for (size_t j = 0; j < m_matrixWidth - 1; ++j) {
				if (*m_matrix[j][i] == *m_matrix[j + 1][i])
					return false;
				if (*m_matrix[j][i] == *m_matrix[j][i + 1])
					return false;
			}

		for (size_t i = 0; i < m_matrixHeight - 1; ++i)
			if (*m_matrix[m_matrixWidth - 1][i] == *m_matrix[m_matrixWidth - 1][i + 1])
				return false;

		for (size_t i = 0; i < m_matrixWidth - 1; ++i)
			if (*m_matrix[i][m_matrixHeight - 1] == *m_matrix[i + 1][m_matrixHeight - 1])
				return false;

		// Game Over
		for (size_t i = 0; i < m_matrixHeight; ++i)
			for (size_t j = 0; j < m_matrixWidth; ++j) {
				m_matrix[i][j]->setScale(*m_scale);
				m_matrix[i][j]->startGameOver();
			}

		endMove();
		endMerge();
		return true;
	}

	return false;
}

// Accessors / Mutators
bool TileMatrix::getIsMoving() const
{
	return m_state == Tstate::MOVING;
}

bool TileMatrix::getIsMerging() const
{
	return m_state == Tstate::MERGING;
}

bool TileMatrix::getIsGameOver() const
{
	return m_isGameOver;
}

bool TileMatrix::getIsIdle() const
{
	return m_state == Tstate::IDLE;
}

int TileMatrix::getAddedScore() const
{
	return m_addedScore;
}

int TileMatrix::getMaxType() const
{
	int max = 0;

	for (size_t i = 0; i < m_matrixHeight; ++i)
		for (size_t j = 0; j < m_matrixWidth; ++j)
			if (m_matrix[i][j])
				if (m_matrix[i][j]->getType() > max)
					max = m_matrix[i][j]->getType();

	return max;
}

void TileMatrix::setAddedScore(const int value)
{
	m_addedScore = value;
}

void TileMatrix::setTimeSpawning(const float time)
{
	m_timeSpawningMax = time;
}

void TileMatrix::setTimeMoving(const float time)
{
	m_timeMovingMax = time;
}

void TileMatrix::setTimeMerging(const float time)
{
	m_timeMergingMax = time;
}

State* const TileMatrix::getState() const
{
	State* state = new State(m_tiles, m_matrixWidth, m_matrixHeight);

	for (size_t i = 0; i < m_matrixWidth; ++i) {
		for (size_t j = 0; j < m_matrixHeight; ++j) {
			if (m_matrix[i][j]) 
				state->board[i][j] = findID(m_matrix[i][j]->getType()) + 1;
			else 
				state->board[i][j] = 0;
		}
	}	

	return state;
}

// Rendering tiles
void TileMatrix::render(sf::RenderTarget& target)
{
	for (auto tile : m_tilesToMerge)
		tile->render(target);

	for (int i = 0; i < m_matrixHeight; ++i)
		for (int j = 0; j < m_matrixWidth; ++j)
			if (m_matrix[i][j])
				m_matrix[i][j]->render(target);
}
