/**
 * Classification Tree
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
#include <vector>

// ctree includes
#include <ctree/concepts.hpp>

namespace classtree {

template <LessthanComparable T, typename U>
[[nodiscard]] static constexpr inline std::pair<std::size_t, bool>
search(const std::vector<std::pair<T, U>>& v, const T& value) noexcept
{
	if (v.size() == 0) [[unlikely]] {
		return {0, false};
	}
	if (v.size() == 1) [[unlikely]] {
		const bool a_lt_b = value < v[0].first;
		if (a_lt_b) {
			return {0, false};
		}
		if (not(a_lt_b) and not(v[0].first < value)) {
			return {0, true};
		}
		return {1, false};
	}

	std::size_t i = 0;
	std::size_t j = v.size() - 1;
	while (i < j) {
		const std::size_t m = ((i + j) / 2);

		const bool a_lt_b = value < v[m].first;
		if (not(a_lt_b) and not(v[m].first < value)) {
			return {m, true};
		}

		if (a_lt_b) {
			if (m == 0) [[unlikely]] {
				return {0, false};
			}
			j = m - 1;
		}
		else {
			if (m == v.size() - 1) [[unlikely]] {
				return {v.size(), false};
			}
			i = m + 1;
		}
	}

	const bool a_lt_b = value < v[i].first;
	if (not(a_lt_b) and not(v[i].first < value)) {
		return {i, true};
	}
	if (a_lt_b) {
		return {i, false};
	}
	return {i + 1, false};
}

/*
template <LessthanComparable T, typename U>
[[nodiscard]] static constexpr std::pair<std::size_t, bool>
search(const std::vector<std::pair<T, U>>& v, const T& value) noexcept
{
	typedef std::pair<T, U> elem_t;

	const auto comp = [&](const elem_t& p1, const elem_t& p2) -> bool
	{
		return p1.first < p2.first;
	};

	const auto it =
		std::lower_bound(v.begin(), v.end(), elem_t{value, U{}}, comp);

	if (it == v.end()) [[unlikely]] {
		return {v.size(), false};
	}

	const int64_t d = std::distance(v.begin(), it);
#if defined DEBUG
	assert(d >= 0);
#endif
	const std::size_t dd = static_cast<std::size_t>(d);
	return std::pair<std::size_t, bool>{dd, are_equal(it->first, value)};
}
*/

} // namespace classtree
