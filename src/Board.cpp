#pragma once
#include "Board.h"

bool Board::willBeOccupied(const sf::Vector2i& pos) const
{
	for (const auto& instruction : m_moveInstructions)
		if (instruction->getNewPos() == pos)
			return true;
	return false;
}

const int Board::merge(const sf::Vector2i& pos, const Taction& direction)
{
	int tile = board[pos.x][pos.y];

	if (direction == Taction::UP) {
		int& tile = board[pos.x][pos.y];
		if (tile == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			int& neighbourMovingTile = board[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().x == pos.x &&
				neighbourMovingTile == tile &&
				board_merging[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y] == false &&
				board_merging[pos.x][pos.y] == false) {

				m_mergedTiles = true;
				board_merging[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y] = true;
				board_merging[pos.x][pos.y] = true;
				return m_moveInstructions.back()->getNewPos().y;
			}
		}

		// Merge if only one is moving
		for (int i = pos.y - 1; i >= 0; --i)
			if (board[pos.x][i] != 0) {
				if (board[pos.x][i] == board[pos.x][pos.y] && !board_merging[pos.x][i] && !board_merging[pos.x][pos.y]) {

					m_mergedTiles = true;
					board_merging[pos.x][i] = true;
					board_merging[pos.x][pos.y] = true;
					return i;
				}
				else
					break;
			}

		m_mergedTiles = false;
		return -1;
	}
	else if (direction == Taction::DOWN) {
		int& tile = board[pos.x][pos.y];
		if (tile == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			int& neighbourMovingTile = board[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().x == pos.x &&
				neighbourMovingTile == tile &&
				board_merging[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y] == false &&
				board_merging[pos.x][pos.y] == false) {

				m_mergedTiles = true;
				board_merging[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y] = true;
				board_merging[pos.x][pos.y] = true;
				return m_moveInstructions.back()->getNewPos().y;
			}
		}

		// Merge if only one is moving
		for (int i = pos.y + 1; i < HEIGHT; ++i)
			if (board[pos.x][i] != 0) {
				if (board[pos.x][i] == board[pos.x][pos.y] && !board_merging[pos.x][i] && !board_merging[pos.x][pos.y]) {

					m_mergedTiles = true;
					board_merging[pos.x][i] = true;
					board_merging[pos.x][pos.y] = true;
					return i;
				}
				else
					break;
			}

		m_mergedTiles = false;
		return -1;
	}
	else if (direction == Taction::LEFT) {
		int& tile = board[pos.x][pos.y];
		if (tile == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			int& neighbourMovingTile = board[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().y == pos.y &&
				neighbourMovingTile == tile &&
				board_merging[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y] == false &&
				board_merging[pos.x][pos.y] == false) {

				m_mergedTiles = true;
				board_merging[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y] = true;
				board_merging[pos.x][pos.y] = true;
				return m_moveInstructions.back()->getNewPos().x;
			}
		}

		// Merge if only one is moving
		for (int i = pos.x - 1; i >= 0; --i)
			if (board[i][pos.y] != 0) {
				if (board[i][pos.y] == board[pos.x][pos.y] && !board_merging[i][pos.y] && !board_merging[pos.x][pos.y]) {

					m_mergedTiles = true;
					board_merging[i][pos.y] = true;
					board_merging[pos.x][pos.y] = true;
					return i;
				}
				else
					break;
			}

		m_mergedTiles = false;
		return -1;
	}
	else if (direction == Taction::RIGHT) {
		int& tile = board[pos.x][pos.y];
		if (tile == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			int& neighbourMovingTile = board[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().y == pos.y &&
				neighbourMovingTile == tile &&
				board_merging[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y] == false &&
				board_merging[pos.x][pos.y] == false) {

				m_mergedTiles = true;
				board_merging[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y] = true;
				board_merging[pos.x][pos.y] = true;
				return m_moveInstructions.back()->getNewPos().x;
			}
		}

		// Merge if only one is moving
		for (int i = pos.x + 1; i < WIDTH; ++i)
			if (board[i][pos.y] != 0) {
				if (board[i][pos.y] == board[pos.x][pos.y] && !board_merging[i][pos.y] && !board_merging[pos.x][pos.y]) {

					m_mergedTiles = true;
					board_merging[i][pos.y] = true;
					board_merging[pos.x][pos.y] = true;
					return i;
				}
				else
					break;
			}

		m_mergedTiles = false;
		return -1;
	}
}

const int Board::findFreeSpace(const sf::Vector2i& pos, const Taction& direction)
{
	if (direction == Taction::UP) {
		const int new_y = merge(pos, direction);
		if (new_y != -1)
			return new_y;
		else {
			for (size_t i = 0; i < pos.y; ++i)
				if ((board[pos.x][i] == 0 || board_moving[pos.x][i]) && !willBeOccupied(sf::Vector2i(pos.x, i)))
					return i;
			return pos.y;
		}
	}
	else if (direction == Taction::DOWN) {
		const int new_y = merge(pos, direction);
		if (new_y != -1)
			return new_y;
		else {
			for (int i = HEIGHT - 1; i > pos.y; --i)
				if ((board[pos.x][i] == 0 || board_moving[pos.x][i]) && !willBeOccupied(sf::Vector2i(pos.x, i)))
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
				if ((board[i][pos.y] == 0 || board_moving[i][pos.y]) && !willBeOccupied(sf::Vector2i(i, pos.y)))
					return i;
			return pos.x;
		}
	}
	else if (direction == Taction::RIGHT) {
		const int new_x = merge(pos, direction);
		if (new_x != -1)
			return new_x;
		else {
			for (size_t i = WIDTH - 1; i > pos.x; --i)
				if ((board[i][pos.y] == 0 || board_moving[i][pos.y]) && !willBeOccupied(sf::Vector2i(i, pos.y)))
					return i;
			return pos.x;
		}
	}
}

void Board::addMoveInstructions(const sf::Vector2i& newPos, const sf::Vector2i& oldPos)
{
	state = Tstate::MOVING;
	board_moving[oldPos.x][oldPos.y] = true;
	m_moveInstructions.push_back(new Instructions{ newPos, oldPos, m_mergedTiles });
	m_mergedTiles = false;
}

Board::Board()
{
	board = new int* [HEIGHT];
	board_moving = new bool* [HEIGHT];
	board_merging = new bool* [HEIGHT];

	for (size_t i = 0; i < HEIGHT; ++i) {
		board[i] = new int[WIDTH];
		board_moving[i] = new bool[WIDTH] { false };
		board_merging[i] = new bool[WIDTH] { false };
	}
}

Board::Board(const size_t& width, const size_t& height) : WIDTH(width), HEIGHT(height)
{
	board = new int* [HEIGHT];
	board_moving = new bool* [HEIGHT];
	board_merging = new bool* [HEIGHT];
	for (size_t i = 0; i < HEIGHT; ++i) {
		board[i] = new int[WIDTH];
		board_moving[i] = new bool[WIDTH] { false };
		board_merging[i] = new bool[WIDTH] { false };
	}
}

Board::Board(const Board& other) : WIDTH(other.WIDTH), HEIGHT(other.HEIGHT)
{
	board = new int* [HEIGHT];
	board_moving = new bool* [HEIGHT];
	board_merging = new bool* [HEIGHT];
	for (size_t i = 0; i < HEIGHT; ++i) {
		board[i] = new int[WIDTH];
		board_moving[i] = new bool[WIDTH];
		board_merging[i] = new bool[WIDTH];
	}


	for (size_t i = 0; i < HEIGHT; ++i)
		for (size_t j = 0; j < WIDTH; ++j) {
			board[i][j] = other.board[i][j];
			board_moving[i][j] = other.board_moving[i][j];
			board_merging[i][j] = other.board_merging[i][j];
		}
}

Board::~Board()
{
	for (size_t i = 0; i < HEIGHT; ++i) {
		delete[] board[i];
		delete[] board_moving[i];
		delete[] board_merging[i];
	}
	delete[] board;
	delete[] board_moving;
	delete[] board_merging;

	for (size_t i = 0; i < m_moveInstructions.size(); ++i)
		delete m_moveInstructions[i];
	m_moveInstructions.clear();
}

int Board::move(Taction direction)
{
	if (direction == Taction::UP) {
		for (int i = 0; i < WIDTH; ++i)
			for (int j = 0; j < HEIGHT; ++j)
				if (board[i][j] != 0) {
					const sf::Vector2i new_pos{ i, findFreeSpace(sf::Vector2i(i, j), direction) };
					const int distance = j - new_pos.y;

					if (distance > 0)
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
				}
	}
	else if (direction == Taction::DOWN) {
		for (int i = 0; i < WIDTH; ++i)
			for (int j = HEIGHT - 1; j >= 0; --j)
				if (board[i][j] != 0) {
					const sf::Vector2i new_pos{ i, findFreeSpace(sf::Vector2i(i, j), direction) };
					const int distance = new_pos.y - j;

					if (distance > 0)
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
				}
	}
	else if (direction == Taction::LEFT) {
		for (int j = 0; j < HEIGHT; ++j)
			for (int i = 0; i < WIDTH; ++i)
				if (board[i][j] != 0) {
					const sf::Vector2i new_pos{ findFreeSpace(sf::Vector2i(i, j), direction), j };
					const int distance = i - new_pos.x;

					if (distance > 0)
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
				}
	}
	else if (direction == Taction::RIGHT) {
		for (int j = 0; j < HEIGHT; ++j)
			for (int i = WIDTH - 1; i >= 0; --i)
				if (board[i][j] != 0) {
					const sf::Vector2i new_pos{ findFreeSpace(sf::Vector2i(i, j), direction), j };
					const int distance = new_pos.x - i;

					if (distance > 0)
						addMoveInstructions(new_pos, sf::Vector2i{ i, j });
				}
	}

	int reward = 0;

	for (size_t i = 0; i < m_moveInstructions.size(); ++i) {
		if (m_moveInstructions[i]->m_merge) {
			const sf::Vector2i new_pos = m_moveInstructions[i]->getNewPos();
			const sf::Vector2i old_pos = m_moveInstructions[i]->getOldPos();

			board[new_pos.x][new_pos.y] = board[old_pos.x][old_pos.y] + 1;
			board[old_pos.x][old_pos.y] = 0;

			reward += REWARDS[board[new_pos.x][new_pos.y]];
		}
		else {
			const sf::Vector2i new_pos = m_moveInstructions[i]->getNewPos();
			const sf::Vector2i old_pos = m_moveInstructions[i]->getOldPos();
			board[new_pos.x][new_pos.y] = board[old_pos.x][old_pos.y];
			board[old_pos.x][old_pos.y] = 0;
		}
	}


	for (size_t i = 0; i < HEIGHT; ++i)
		for (size_t j = 0; j < WIDTH; ++j) {
			board_moving[i][j] = false;
			board_merging[i][j] = false;
		}

	for (size_t i = 0; i < m_moveInstructions.size(); ++i)
		delete m_moveInstructions[i];
	m_moveInstructions.clear();

	return reward;
}

bool Board::isMovePossible(Taction direction)
{
	if (direction == Taction::UP) {
		for (int i = 0; i < WIDTH; ++i)
			for (int j = 0; j < HEIGHT; ++j)
				if (board[i][j] != 0) {
					const sf::Vector2i new_pos{ i, findFreeSpace(sf::Vector2i(i, j), direction) };
					const int distance = j - new_pos.y;

					if (distance > 0) {
						for (size_t i = 0; i < HEIGHT; ++i)
							for (size_t j = 0; j < WIDTH; ++j) {
								board_moving[i][j] = false;
								board_merging[i][j] = false;
							}

						for (size_t i = 0; i < m_moveInstructions.size(); ++i)
							delete m_moveInstructions[i];
						m_moveInstructions.clear();

						return true;
					}
				}
	}
	else if (direction == Taction::DOWN) {
		for (int i = 0; i < WIDTH; ++i)
			for (int j = HEIGHT - 1; j >= 0; --j)
				if (board[i][j] != 0) {
					const sf::Vector2i new_pos{ i, findFreeSpace(sf::Vector2i(i, j), direction) };
					const int distance = new_pos.y - j;

					if (distance > 0) {
						for (size_t i = 0; i < HEIGHT; ++i)
							for (size_t j = 0; j < WIDTH; ++j) {
								board_moving[i][j] = false;
								board_merging[i][j] = false;
							}

						for (size_t i = 0; i < m_moveInstructions.size(); ++i)
							delete m_moveInstructions[i];
						m_moveInstructions.clear();

						return true;
					}
				}
	}
	else if (direction == Taction::LEFT) {
		for (int j = 0; j < HEIGHT; ++j)
			for (int i = 0; i < WIDTH; ++i)
				if (board[i][j] != 0) {
					const sf::Vector2i new_pos{ findFreeSpace(sf::Vector2i(i, j), direction), j };
					const int distance = i - new_pos.x;

					if (distance > 0) {
						for (size_t i = 0; i < HEIGHT; ++i)
							for (size_t j = 0; j < WIDTH; ++j) {
								board_moving[i][j] = false;
								board_merging[i][j] = false;
							}

						for (size_t i = 0; i < m_moveInstructions.size(); ++i)
							delete m_moveInstructions[i];
						m_moveInstructions.clear();

						return true;
					}
				}
	}
	else if (direction == Taction::RIGHT) {
		for (int j = 0; j < HEIGHT; ++j)
			for (int i = WIDTH - 1; i >= 0; --i)
				if (board[i][j] != 0) {
					const sf::Vector2i new_pos{ findFreeSpace(sf::Vector2i(i, j), direction), j };
					const int distance = new_pos.x - i;

					if (distance > 0) {
						for (size_t i = 0; i < HEIGHT; ++i)
							for (size_t j = 0; j < WIDTH; ++j) {
								board_moving[i][j] = false;
								board_merging[i][j] = false;
							}

						for (size_t i = 0; i < m_moveInstructions.size(); ++i)
							delete m_moveInstructions[i];
						m_moveInstructions.clear();

						return true;
					}

				}
	}

	return false;
}

const void Board::display(const std::string& text) const
{
	if (text != "")
		printf("%s\n", text.c_str());

	for (size_t i = 0; i < HEIGHT; ++i) {
		for (size_t j = 0; j < WIDTH; ++j) {
			printf("%d ", board[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

const bool Board::isTerminalState() const
{
	for (size_t i = 0; i < HEIGHT; ++i)
		for (size_t j = 0; j < WIDTH; ++j)
			if (board[j][i] == 0)
				return false;

	for (size_t i = 0; i < HEIGHT - 1; ++i)
		for (size_t j = 0; j < WIDTH - 1; ++j) {
			if (board[j][i] == board[j + 1][i])
				return false;
			if (board[j][i] == board[j][i + 1])
				return false;
		}

	for (size_t i = 0; i < HEIGHT - 1; ++i)
		if (board[WIDTH - 1][i] == board[WIDTH - 1][i + 1])
			return false;

	for (size_t i = 0; i < WIDTH - 1; ++i)
		if (board[i][HEIGHT - 1] == board[i + 1][HEIGHT - 1])
			return false;

	return true;
}

const int TileBoard::merge(const sf::Vector2i& pos, const Taction& direction)
{
	if (direction == Taction::UP) {
		Tile& tile = *m_matrix[pos.x][pos.y];
		if (tile.getType() == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			Tile& neighbourMovingTile = *m_matrix[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().x == pos.x &&
				neighbourMovingTile.getType() == tile.getType() &&
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
				if (m_matrix[pos.x][i]->getType() == tile.getType() && !m_matrix[pos.x][i]->getMerging() && !tile.getMerging()) {

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
		if (tile.getType() == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			Tile& neighbourMovingTile = *m_matrix[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().x == pos.x &&
				neighbourMovingTile.getType() == tile.getType() &&
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
				if (m_matrix[pos.x][i]->getType() == tile.getType() && !m_matrix[pos.x][i]->getMerging() && !tile.getMerging()) {

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
		if (tile.getType() == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			Tile& neighbourMovingTile = *m_matrix[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().y == pos.y &&
				neighbourMovingTile.getType() == tile.getType() &&
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
				if (m_matrix[i][pos.y]->getType() == tile.getType() && !m_matrix[i][pos.y]->getMerging() && !tile.getMerging()) {

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
		if (tile.getType() == m_maxType) return -1;

		// Merge if both are moving
		if (m_moveInstructions.size() != 0) {
			Tile& neighbourMovingTile = *m_matrix[m_moveInstructions.back()->getOldPos().x][m_moveInstructions.back()->getOldPos().y];

			if (m_moveInstructions.back()->getOldPos().y == pos.y &&
				neighbourMovingTile.getType() == tile.getType() &&
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
				if (m_matrix[i][pos.y]->getType() == tile.getType() && !m_matrix[i][pos.y]->getMerging() && !tile.getMerging()) {

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
}

TileBoard::TileBoard() : Board()
{
	tileBoard = new Tile** [HEIGHT];
	for (size_t i = 0; i < HEIGHT; ++i)
		tileBoard[i] = new Tile* [WIDTH];
}

TileBoard::TileBoard(const size_t& width, const size_t& height) : Board(width, height)
{
	tileBoard = new Tile** [HEIGHT];
	for (size_t i = 0; i < HEIGHT; ++i)
		tileBoard[i] = new Tile* [WIDTH];
}

TileBoard::TileBoard(const TileBoard& other) : Board(other)
{
	tileBoard = new Tile** [HEIGHT];
	for (size_t i = 0; i < HEIGHT; ++i)
		tileBoard[i] = new Tile* [WIDTH];

	for (size_t i = 0; i < HEIGHT; ++i)
		for (size_t j = 0; j < WIDTH; ++j)
			if (other.tileBoard[i][j])
				tileBoard[i][j] = new Tile(*other.tileBoard[i][j]);
}

TileBoard::~TileBoard(){
	for (size_t i = 0; i < HEIGHT; ++i)
		for (size_t j = 0; j < WIDTH; ++j)
			delete tileBoard[i][j];

	for (size_t i = 0; i < HEIGHT; ++i)
		delete[] tileBoard[i];

	delete[] tileBoard;
}
