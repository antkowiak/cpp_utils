#pragma once

#include <list>
#include <string>
#include <vector>

// Call the function with the minimum value of a collection
template<typename Iter, typename Function>
void apply_if_min(Iter iterStart, Iter iterEnd, Function func)
{
	Iter minIter = iterStart;

	for (; iterStart != iterEnd; ++iterStart)
		if (*iterStart < *minIter)
			minIter = iterStart;

	if (minIter != iterEnd)
		func(*minIter);
}

// call the function with the maximum value of a collection
template<typename Iter, typename Function>
void apply_if_max(Iter iterStart, Iter iterEnd, Function func)
{
	Iter maxIter = iterStart;

	for (; iterStart != iterEnd; ++iterStart)
		if (*maxIter < *iterStart)
			maxIter = iterStart;

	if (maxIter != iterEnd)
		func(*maxIter);
}

// Split a collection into a vector of sub-collections, based on a provided delimiter object
template<typename T, typename V>
std::vector<T> split_to_vector(const T& data, const V& delim)
{
	std::vector<T> vec;

	typename T::const_iterator startIter = data.begin();
	typename T::const_iterator endIter = std::find(startIter, data.end(), delim);

	while (startIter != data.end())
	{
		T t(startIter, endIter);
		if (!t.empty())
			vec.push_back(t);
		startIter = endIter;
		if (startIter == data.end())
			break;
		++startIter;
		endIter = std::find(startIter, data.end(), delim);
	}

	return vec;
}

// Split a collection into a list of sub-collections, based on a provided delimiter object
template<typename T, typename V>
std::list<T> split_to_list(const T& data, const V& delim)
{
	std::list<T> lst;

	typename T::const_iterator startIter = data.begin();
	typename T::const_iterator endIter = std::find(startIter, data.end(), delim);

	while (startIter != data.end())
	{
		T t(startIter, endIter);
		if (!t.empty())
			lst.push_back(t);
		startIter = endIter;
		if (startIter == data.end())
			break;
		++startIter;
		endIter = std::find(startIter, data.end(), delim);
	}

	return lst;
}

// Split a string into a vector of sub-strings, based on a provided delimiter string
std::vector<std::string> split_to_vector(const std::string& data, const std::string& delim)
{
	std::vector<std::string> vec;

	if (data.empty())
		return vec;

	if (delim.empty())
	{
		vec.push_back(data);
		return vec;
	}

	size_t startPos = 0;
	while (true)
	{
		if (startPos >= data.size())
			return vec;
		size_t nextPos = data.find(delim, startPos);
		if (nextPos == std::string::npos)
		{
			std::string s = data.substr(startPos);
			if (!s.empty())
				vec.push_back(s);
			return vec;
		}
		std::string s = data.substr(startPos, nextPos - startPos);

		if (!s.empty())
			vec.push_back(s);

		startPos = nextPos + delim.size();
	}
	return vec;
}

// Split a string into a list of sub-strings, based on a provided delimiter string
std::list<std::string> split_to_list(const std::string& data, const std::string& delim)
{
	std::list<std::string> lst;

	if (data.empty())
		return lst;

	if (delim.empty())
	{
		lst.push_back(data);
		return lst;
	}

	size_t startPos = 0;
	while (true)
	{
		if (startPos >= data.size())
			return lst;
		size_t nextPos = data.find(delim, startPos);
		if (nextPos == std::string::npos)
		{
			std::string s = data.substr(startPos);
			if (!s.empty())
				lst.push_back(s);
			return lst;
		}
		std::string s = data.substr(startPos, nextPos - startPos);

		if (!s.empty())
			lst.push_back(s);

		startPos = nextPos + delim.size();
	}
	return lst;
}
