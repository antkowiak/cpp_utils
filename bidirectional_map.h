#pragma once

#include <algorithm>
#include <map>
#include <vector>

template<typename T, typename U>
class bidirectional_map
{
protected:
	std::map<T, U> m_map;
	std::map<U, T> m_map_r;

protected:
	template<typename V, typename W>
	static size_t erase_values(std::map<V, W>& m, const W& val)
	{
		std::vector<V> keys;
		for (auto iter = m.cbegin(); iter != m.cend(); ++iter)
			if (iter->second == val)
				keys.push_back(iter->first);
		for (auto key : keys)
			m.erase(key);
		return keys.size();
	}

	template<typename V, typename W>
	static bool contains_key(const std::map<V, W>& m, const V& key)
	{
		for (auto iter = m.cbegin(); iter != m.cend(); ++iter)
			if (iter->first == key)
				return true;
		return false;
	}

	template<typename V, typename W>
	static bool contains_value(const std::map<V, W>& m, const U& val)
	{
		for (auto iter = m.cbegin(); iter != m.cend(); ++iter)
			if (iter->second == val)
				return true;
		return false;
	}

public:
	// Constructors, destructor, and assignment operator
	bidirectional_map<T, U>() {}
	bidirectional_map<T, U>(const bidirectional_map<T, U>& rhs) : m_map(rhs.m_map), m_map_r(rhs.m_map_r) {}
	virtual ~bidirectional_map<T, U>() {}
	
	bidirectional_map<T, U>& operator = (const bidirectional_map<T, U>& rhs) { this->m_map = rhs.m_map; this->m_map_r = rhs.m_map_r; return *this; }

	// Element access
	virtual const U& at(const T& t) const { return m_map.at(t); }
	virtual const T& at(const U& u) const { return m_map_r.at(u); }
	
	virtual U& get(const T& t) { return m_map[t]; }
	virtual T& get(const U& u) { return m_map_r[u]; }

	virtual const U& operator [] (const T& t) { return m_map[t]; }
	virtual const T& operator [] (const U& u) { return m_map_r[u]; }

	// Iterators
	virtual typename std::map<T, U>::const_iterator begin() const { return m_map.begin(); }
	virtual typename std::map<U, T>::const_iterator begin_r() const { return m_map_r.begin(); }

	virtual typename std::map<T, U>::const_iterator cbegin() const { return m_map.cbegin(); }
	virtual typename std::map<U, T>::const_iterator cbegin_r() const { return m_map_r.cbegin(); }

	virtual typename std::map<T, U>::const_reverse_iterator rbegin() const { return m_map.rbegin(); }
	virtual typename std::map<U, T>::const_reverse_iterator rbegin_r() const { return m_map_r.rbegin(); }

	virtual typename std::map<T, U>::const_reverse_iterator crbegin() const { return m_map.crbegin(); }
	virtual typename std::map<U, T>::const_reverse_iterator crbegin_r() const { return m_map_r.crbegin(); }

	virtual typename std::map<T, U>::const_iterator end() const { return m_map.end(); }
	virtual typename std::map<U, T>::const_iterator end_r() const { return m_map_r.end(); }

	virtual typename std::map<T, U>::const_iterator cend() const { return m_map.cend(); }
	virtual typename std::map<U, T>::const_iterator cend_r() const { return m_map_r.cend(); }

	virtual typename std::map<T, U>::const_reverse_iterator rend() const { return m_map.rend(); }
	virtual typename std::map<U, T>::const_reverse_iterator rend_r() const { return m_map_r.rend(); }

	virtual typename std::map<T, U>::const_reverse_iterator crend() const { return m_map.crend(); }
	virtual typename std::map<U, T>::const_reverse_iterator crend_r() const { return m_map_r.crend(); }

	// Capacity
	virtual bool empty() const { return m_map.empty() && m_map_r.empty(); }
	virtual size_t size() const { return m_map.size(); }
	virtual size_t max_size() const { return std::min(m_map.max_size(), m_map_r.max_size()); }

	// Modifiers
	virtual void clear() { m_map.clear(); m_map_r.clear(); }

	virtual void put(const T& t, const U& u) { erase_values(m_map, u); erase_values(m_map_r, t); m_map[t] = u; m_map_r[u] = t; }
	virtual void put(const U& u, const T& t) { erase_values(m_map, u); erase_values(m_map_r, t); m_map[t] = u; m_map_r[u] = t; }

	virtual size_t erase(const T& t)
	{
		if (!contains_key(m_map, t))
			return 0;

		const U value = m_map[t];

		return std::max({
			m_map.erase(t),
			erase_values(m_map, value),
			m_map_r.erase(value),
			erase_values(m_map_r, t) });
	}
	virtual size_t erase(const U& u)
	{
		if (!contains_key(m_map_r, u))
			return 0;

		const T value = m_map_r[u];

		return std::max({
			m_map_r.erase(u),
			erase_values(m_map_r, value),
			m_map.erase(value),
			erase_values(m_map, u) });
	}

	virtual void swap(bidirectional_map<T, U>& rhs) { std::swap(m_map, rhs.m_map); std::swap(m_map_r, rhs.m_map_r); }

	// Lookup
	virtual size_t count(const T& t) const { return m_map.count(t); }
	virtual size_t count(const U& u) const { return m_map_r.count(u); }
	
	virtual typename std::map<T, U>::const_iterator find(const T& t) const { return m_map.find(t); }
	virtual typename std::map<U, T>::const_iterator find(const U& u) const { return m_map_r.find(u); }

	virtual bool contains(const T& t) const
	{
		for (auto iter = m_map.cbegin(); iter != m_map.cend(); ++iter)
			if (iter->first == t)
				return true;
		return false;
	}
	virtual bool contains(const U& u) const
	{
		for (auto iter = m_map_r.cbegin(); iter != m_map_r.cend(); ++iter)
			if (iter->first == u)
				return true;
		return false;
	}

	// Comparison operators
	virtual bool operator == (const bidirectional_map<T, U>& rhs) const { return m_map == rhs.m_map && m_map_r == rhs.m_map_r; }
	virtual bool operator == (const std::map<T, U>& rhs) const { return m_map == rhs; }
	virtual bool operator == (const std::map<U, T>& rhs) const { return m_map_r == rhs; }

	virtual bool operator != (const bidirectional_map<T, U>& rhs) const { return !(*this == rhs); }
	virtual bool operator != (const std::map<T, U>& rhs) const { return !(*this == rhs); }
	virtual bool operator != (const std::map<U, T>& rhs) const { return !(*this == rhs); }
};
