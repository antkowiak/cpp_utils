#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "algorithm_rda.h"
#include "table.h"

static table<std::string> csv_to_table(const std::string& text)
{
	std::vector<std::string> lines = split_string_to_vector(text, "\n");

	std::vector<std::vector<std::string> > cells;

	for (std::string line : lines)
		cells.push_back(split_string_to_vector(line, ","));

	return table<std::string>(cells);
}
