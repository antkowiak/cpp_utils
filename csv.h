#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "algorithm_rda.h"
#include "table.h"

static table<std::string> string_to_table(const std::string& text, const std::string& row_delim, const std::string& col_delim)
{
	std::vector<std::string> lines = split_string_to_vector(text, row_delim);

	std::vector<std::vector<std::string> > cells;

	for (std::string line : lines)
		cells.push_back(split_string_to_vector(line, col_delim));

	return table<std::string>(cells);
}

static table<std::string> csv_to_table(const std::string& text)
{
	return string_to_table(text, "\n", ",");
}
