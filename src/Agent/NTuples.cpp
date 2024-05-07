#include "NTuples.h"

void NTuples::save(const std::string& filename) const
{
	std::ofstream file;
	file.open(filename);

	if (file.good()) {
		std::stringstream buffer;

		for (size_t i = 0; i < NUM_VERTICAL; ++i)
			for (size_t j = 0; j < MAX_TYPE + 1; ++j)
				for (size_t k = 0; k < MAX_TYPE + 1; ++k)
					for (size_t l = 0; l < MAX_TYPE + 1; ++l)
						for (size_t m = 0; m < MAX_TYPE + 1; ++m)
							buffer << V_LUT[i][j][k][l][m] << ' ';

		for (size_t i = 0; i < NUM_HORIZONTAL; ++i)
			for (size_t j = 0; j < MAX_TYPE + 1; ++j)
				for (size_t k = 0; k < MAX_TYPE + 1; ++k)
					for (size_t l = 0; l < MAX_TYPE + 1; ++l)
						for (size_t m = 0; m < MAX_TYPE + 1; ++m)
							buffer << H_LUT[i][j][k][l][m] << ' ';

		for (size_t i = 0; i < NUM_SQUARES; ++i)
			for (size_t j = 0; j < MAX_TYPE + 1; ++j)
				for (size_t k = 0; k < MAX_TYPE + 1; ++k)
					for (size_t l = 0; l < MAX_TYPE + 1; ++l)
						for (size_t m = 0; m < MAX_TYPE + 1; ++m)
							buffer << SQUARE_LUT[i][j][k][l][m] << ' ';

		file << buffer.str();

		file.close();
	}
}

void NTuples::load(const std::string& filename)
{
	std::ifstream file;
	file.open(filename);

	if (file.good()) {
		for (size_t i = 0; i < NUM_VERTICAL; ++i)
			for (size_t j = 0; j < MAX_TYPE + 1; ++j)
				for (size_t k = 0; k < MAX_TYPE + 1; ++k)
					for (size_t l = 0; l < MAX_TYPE + 1; ++l)
						for (size_t m = 0; m < MAX_TYPE + 1; ++m)
							file >> V_LUT[i][j][k][l][m];

		for (size_t i = 0; i < NUM_HORIZONTAL; ++i)
			for (size_t j = 0; j < MAX_TYPE + 1; ++j)
				for (size_t k = 0; k < MAX_TYPE + 1; ++k)
					for (size_t l = 0; l < MAX_TYPE + 1; ++l)
						for (size_t m = 0; m < MAX_TYPE + 1; ++m)
							file >> H_LUT[i][j][k][l][m];

		for (size_t i = 0; i < NUM_SQUARES; ++i)
			for (size_t j = 0; j < MAX_TYPE + 1; ++j)
				for (size_t k = 0; k < MAX_TYPE + 1; ++k)
					for (size_t l = 0; l < MAX_TYPE + 1; ++l)
						for (size_t m = 0; m < MAX_TYPE + 1; ++m)
							file >> SQUARE_LUT[i][j][k][l][m];

		file.close();
	}
}

const void NTuples::updateVFunction(const State* const state, const double& delta)
{
	for (size_t i = 0; i < NTuples::NUM_VERTICAL; ++i)
		for (size_t j = 0; j < state->WIDTH; ++j)
			V_LUT[i][state->board[0][j]][state->board[1][j]][state->board[2][j]][state->board[3][j]] += delta;

	for (size_t i = 0; i < NTuples::NUM_HORIZONTAL; ++i)
		for (size_t j = 0; j < state->HEIGHT; ++j)
			H_LUT[i][state->board[j][0]][state->board[j][1]][state->board[j][2]][state->board[j][3]] += delta;

	size_t square_index = 0;
	for (size_t i = 0; i < state->HEIGHT - 1; ++i)
		for (size_t j = 0; j < state->WIDTH - 1; ++j) {
			SQUARE_LUT[square_index][state->board[i][j]][state->board[i][j + 1]][state->board[i + 1][j]][state->board[i + 1][j + 1]] += delta;
			square_index++;
		}
}
