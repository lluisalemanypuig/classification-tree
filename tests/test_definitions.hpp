/**
 * Tests for the Classification Tree
 * Copyright (C) 2025  Lluís Alemany Puig
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 * 		Lluís Alemany Puig
 * 		https://github.com/lluisalemanypuig
 */

#pragma once

// C++ includes
#include <sstream>
#include <ostream>

// ctree includes
#include <ctree/ctree.hpp>

struct data_eq {
	int i, j, k, z;
	[[nodiscard]] bool operator== (const data_eq& o) const noexcept
	{
		return i == o.i and j == o.j and k == o.k and z == o.z;
	}

	inline friend std::ostream&
	operator<< (std::ostream& os, const data_eq& c) noexcept
	{
		os << '(' << c.i << ' ' << c.j << ' ' << c.k << ' ' << c.z << ')';
		return os;
	}
};

static_assert(classtree::EqualityComparable<data_eq>);

struct data_lt {
	int i, j, k, z;

	[[nodiscard]] bool operator== (const data_lt& o) const noexcept
	{
		return i == o.i and j == o.j and k == o.k and z == o.z;
	}
	[[nodiscard]] std::strong_ordering operator<=> (const data_lt& o
	) const noexcept
	{
		if (i == o.i) {
			if (j == o.j) {
				if (k == o.k) {
					return z <=> o.z;
				}
				return k <=> o.k;
			}
			return j <=> o.j;
		}
		return i <=> o.i;
	}

	inline friend std::ostream&
	operator<< (std::ostream& os, const data_lt& c) noexcept
	{
		os << '(' << c.i << ' ' << c.j << ' ' << c.k << ' ' << c.z << ')';
		return os;
	}
};

static_assert(classtree::LessthanComparable<data_lt>);

struct meta_incr {
	int num_occs = 0;

	meta_incr& operator+= (const meta_incr& m) noexcept
	{
		num_occs += m.num_occs;
		return *this;
	}

	[[nodiscard]] bool operator== (const meta_incr& other) const noexcept
	{
		return num_occs == other.num_occs;
	}

	inline friend std::ostream&
	operator<< (std::ostream& os, const meta_incr& c) noexcept
	{
		os << '{' << c.num_occs << '}';
		return os;
	}
};

template <typename tree_t>
std::string print_string(const tree_t& kd) noexcept
{
	std::stringstream ss;
	kd.print(ss);
	return ss.str();
}

template <typename it_t>
std::string iterate_string(it_t& it) noexcept
{
	std::stringstream ss;
	ss << "Iterate:\n";
	while (not it.end()) {
		const auto& e = *it;
		ss << "    " << e.first << ' ' << e.second << '\n';
		++it;
	}
	return ss.str();
}

template <typename it_t>
std::string iterate_string_backward(it_t& it) noexcept
{
	it.at_end();

	std::stringstream ss;
	ss << "Iterate:\n";
	while (not it.past_begin()) {
		const auto& e = *it;
		ss << "    " << e.first << ' ' << e.second << '\n';
		--it;
	}
	return ss.str();
}

template <typename it_t>
std::string range_iterate_string_backward(it_t& it) noexcept
{
	[[maybe_unused]] const bool _ = it.at_end();

	std::stringstream ss;
	ss << "Iterate:\n";
	while (not it.past_begin()) {
		const auto& e = *it;
		ss << "    " << e.first << ' ' << e.second << '\n';
		--it;
	}
	return ss.str();
}
