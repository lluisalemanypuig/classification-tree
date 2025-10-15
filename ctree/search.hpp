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
#include <ctree/types.hpp>

namespace classtree {

template <LessthanComparable data_t, typename metadata_t>
	requires NotCompound<data_t, metadata_t>
[[nodiscard]] static constexpr inline std::pair<size_t, bool> search(
	const std::vector<element_t<data_t, metadata_t>>& v, const data_t& value
) noexcept
{
	if (v.size() == 0) [[unlikely]] {
		return {0, false};
	}
	if (v.size() == 1) [[unlikely]] {
		if (value < v[0]) {
			return {0, false};
		}
		else if (v[0] < value) {
			return {1, false};
		}
		return {0, true};
	}

	size_t i = 0;
	size_t j = v.size() - 1;
	while (i < j) {
		const size_t m = ((i + j) / 2);

		if (value < v[m]) {
			if (m == 0) [[unlikely]] {
				return {0, false};
			}
			j = m - 1;
		}
		else if (v[m] < value) {
			if (m == v.size() - 1) [[unlikely]] {
				return {v.size(), false};
			}
			i = m + 1;
		}
		else {
			return {m, true};
		}
	}

	if (value < v[i]) {
		return {i, false};
	}
	if (v[i] < value) {
		return {i + 1, false};
	}
	return {i, true};
}

template <LessthanComparable data_t, typename metadata_t>
	requires Compound<data_t, metadata_t>
[[nodiscard]] static constexpr inline std::pair<size_t, bool> search(
	const std::vector<element_t<data_t, metadata_t>>& v, const data_t& value
) noexcept
{
	if (v.size() == 0) [[unlikely]] {
		return {0, false};
	}
	if (v.size() == 1) [[unlikely]] {
		if (value < v[0].data) {
			return {0, false};
		}
		else if (v[0].data < value) {
			return {1, false};
		}
		return {0, true};
	}

	size_t i = 0;
	size_t j = v.size() - 1;
	while (i < j) {
		const size_t m = ((i + j) / 2);

		if (value < v[m].data) {
			if (m == 0) [[unlikely]] {
				return {0, false};
			}
			j = m - 1;
		}
		else if (v[m].data < value) {
			if (m == v.size() - 1) [[unlikely]] {
				return {v.size(), false};
			}
			i = m + 1;
		}
		else {
			return {m, true};
		}
	}

	if (value < v[i].data) {
		return {i, false};
	}
	if (v[i].data < value) {
		return {i + 1, false};
	}
	return {i, true};
}

template <LessthanComparable T, typename U>
[[nodiscard]] static constexpr inline std::pair<size_t, bool> search(
	const std::vector<std::pair<T, U>>& v, const T& value
) noexcept
{
	if (v.size() == 0) [[unlikely]] {
		return {0, false};
	}
	if (v.size() == 1) [[unlikely]] {
		if (value < v[0].first) {
			return {0, false};
		}
		else if (v[0].first < value) {
			return {1, false};
		}
		return {0, true};
	}

	size_t i = 0;
	size_t j = v.size() - 1;
	while (i < j) {
		const size_t m = ((i + j) / 2);

		if (value < v[m].first) {
			if (m == 0) [[unlikely]] {
				return {0, false};
			}
			j = m - 1;
		}
		else if (v[m].first < value) {
			if (m == v.size() - 1) [[unlikely]] {
				return {v.size(), false};
			}
			i = m + 1;
		}
		else {
			return {m, true};
		}
	}

	if (value < v[i].first) {
		return {i, false};
	}
	if (v[i].first < value) {
		return {i + 1, false};
	}
	return {i, true};
}

} // namespace classtree
