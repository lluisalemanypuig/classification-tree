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
#include <ctree/concepts.hpp>

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
	std::stringstream ss;
	ss << "Iterate:\n";
	while (not it.past_begin()) {
		const auto& e = *it;
		ss << "    " << e.first << ' ' << e.second << '\n';
		--it;
	}
	return ss.str();
}
