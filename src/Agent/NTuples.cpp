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