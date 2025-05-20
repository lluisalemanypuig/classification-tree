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
#include <concepts>

namespace classtree {

/**
 * @brief Equality comparable concept.
 *
 * Two objects of type @e T are comparable via the '==' operator.
 * @tparam T Type.
 */
template <typename T>
concept EqualityComparable = std::equality_comparable<T>;

/**
 * @brief Less-than comparable concept.
 *
 * Two objects of type @e T are comparable via the '<' operator.
 * @tparam T Type.
 */
template <typename T>
concept LessthanComparable = requires(const T& t1, const T& t2) {
	{ t1 < t2 } -> std::same_as<bool>;
};

/**
 * @brief Equality comparable concept.
 *
 * An object of type @e T can be incremented from another object of the
 * same type via the '+=' operator.
 * @tparam T Type.
 */
template <typename T>
concept Incrementable = requires(T& t1, const T& t2) { t1 += t2; };

/**
 * @brief Comparable concept.
 *
 * Any two objects of type @e T can be compared via the '<' and '=='
 * operators.
 * @tparam T Type.
 */
template <typename T>
concept Comparable = EqualityComparable<T> and LessthanComparable<T>;

/**
 * @brief Mergeable concept.
 *
 * Any two objects of type @e T can be merged via the '+=' operator.
 * @tparam T Type.
 */
template <typename T>
concept Mergeable = requires(T t1, T t2) { t1 += t2; };

} // namespace classtree
