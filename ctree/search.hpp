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
namespace detail {

template <LessthanComparable data_t, typename metadata_t>
[[nodiscard]] static constexpr inline std::pair<size_t, bool>
element_search_linear(
	const std::vector<element_t<data_t, metadata_t>>& v, const data_t& value
) noexcept
{
	const auto value_elem = [](const element_t<data_t, metadata_t>& elem)
	{
		if constexpr (Compound<data_t, metadata_t>) {
			return elem.data;
		}
		else {
			return elem;
		}
	};

	if (v.size() == 0) [[unlikely]] {
		return {0, false};
	}
	if (v.size() == 1) [[unlikely]] {
		if (value < value_elem(v[0])) {
			return {0, false};
		}
		if (value_elem(v[0]) < value) {
			return {1, false};
		}
		return {0, true};
	}

	for (size_t i = 0; i < v.size() - 1; ++i) {
		if (value < value_elem(v[i])) {
			return {i, false};
		}
		else if (value_elem(v[i]) < value) {
			if (value < value_elem(v[i + 1])) {
				return {i + 1, false};
			}
		}
		else {
			return {i, true};
		}
	}
	if (value_elem(v.back()) < value) {
		return {v.size(), false};
	}
	return {v.size() - 1, true};
}

template <LessthanComparable data_t, typename metadata_t>
[[nodiscard]] static constexpr inline std::pair<size_t, bool>
element_search_binary(
	const std::vector<element_t<data_t, metadata_t>>& v, const data_t& value
) noexcept
{
	const auto value_elem = [](const element_t<data_t, metadata_t>& elem)
	{
		if constexpr (Compound<data_t, metadata_t>) {
			return elem.data;
		}
		else {
			return elem;
		}
	};

	if (v.size() == 0) [[unlikely]] {
		return {0, false};
	}
	if (v.size() == 1) [[unlikely]] {
		if (value < value_elem(v[0])) {
			return {0, false};
		}
		if (value_elem(v[0]) < value) {
			return {1, false};
		}
		return {0, true};
	}

	size_t i = 0;
	size_t j = v.size() - 1;
	while (i < j) {
		const size_t m = ((i + j) / 2);

		if (value < value_elem(v[m])) {
			if (m == 0) [[unlikely]] {
				return {0, false};
			}
			j = m - 1;
		}
		else if (value_elem(v[m]) < value) {
			if (m == v.size() - 1) [[unlikely]] {
				return {v.size(), false};
			}
			i = m + 1;
		}
		else {
			return {m, true};
		}
	}

	if (value < value_elem(v[i])) {
		return {i, false};
	}
	if (value_elem(v[i]) < value) {
		return {i + 1, false};
	}
	return {i, true};
}

template <LessthanComparable T, typename U>
[[nodiscard]] static constexpr inline std::pair<size_t, bool>
pair_search_linear(const std::vector<std::pair<T, U>>& v, const T& value)
	noexcept
{
	if (v.size() == 0) [[unlikely]] {
		return {0, false};
	}
	if (v.size() == 1) [[unlikely]] {
		if (value < v[0].first) {
			return {0, false};
		}
		if (v[0].first < value) {
			return {1, false};
		}
		return {0, true};
	}

	for (size_t i = 0; i < v.size() - 1; ++i) {
		if (value < v[i].first) {
			return {i, false};
		}
		else if (v[i].first < value) {
			if (value < v[i + 1].first) {
				return {i + 1, false};
			}
		}
		else {
			return {i, true};
		}
	}
	if (v.back().first < value) {
		return {v.size(), false};
	}
	return {v.size() - 1, true};
}

template <LessthanComparable T, typename U>
[[nodiscard]] static constexpr inline std::pair<size_t, bool>
pair_search_binary(const std::vector<std::pair<T, U>>& v, const T& value)
	noexcept
{
	if (v.size() == 0) [[unlikely]] {
		return {0, false};
	}
	if (v.size() == 1) [[unlikely]] {
		if (value < v[0].first) {
			return {0, false};
		}
		if (v[0].first < value) {
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

} // namespace detail

template <LessthanComparable data_t, typename metadata_t>
[[nodiscard]] static constexpr inline std::pair<size_t, bool> search(
	const std::vector<element_t<data_t, metadata_t>>& v, const data_t& value
) noexcept
{
	if (v.size() <= 32) {
		return detail::element_search_linear<data_t, metadata_t>(v, value);
	}
	return detail::element_search_binary<data_t, metadata_t>(v, value);
}

template <LessthanComparable T, typename U>
[[nodiscard]] static constexpr inline std::pair<size_t, bool>
search(const std::vector<std::pair<T, U>>& v, const T& value) noexcept
{
	if (v.size() <= 32) {
		return detail::pair_search_linear<T, U>(v, value);
	}
	return detail::pair_search_binary<T, U>(v, value);
}

} // namespace classtree
