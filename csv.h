#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "algorithm_rda.h"
#include "table.h"

static table<std::string> csv_to_table(const std::string& text)
{
	size_t columns = 0;

	std::vector<std::string> lines = split_string_to_vector(text, "\n");
	size_t rows = lines.size();

	std::vector<std::vector<std::string> > cells;

	for (std::string line : lines)
	{
		cells.push_back(split_string_to_vector(line, ","));
		columns = std::max(columns, cells.back().size());
	}

	table<std::string> tab(columns, rows);

	for (size_t row = 0; row < rows; ++row)
		for (size_t column = 0; column < columns; ++column)
			tab[column][row] = cells[column][row];
	
	return tab;
}
