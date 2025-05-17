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
concept Comparable = EqualityComparable<T> or LessthanComparable<T>;

/**
 * @brief The Classification Tree class.
 *
 * This class helps manage the classification of objects via hand-crafted
 * key values \f$k_1, \dots, k_n\f$. At every node of the tree is stored
 * an array of pairs of (key, subtree). At depth level \f$i\f$ of the
 * tree, the key values in the pairs correspond to \f$k_i\f$.
 *
 * Now, for any two objects \f$p, q\f$,
 * - if \f$p \cong q\f$ then we must have \f$k_1(p) = k_1(q), \dots, k_n(p) = k_n(q)\f$.
 * The key values should be the same (first with first, second with second, ...)
 * for every element in a group of equal objects.
 * - if \f$k_i(p) \neq k_i(q)\f$ for any key function \f$k_i\f$ then we
 * must have \f$p \ncong q\f$.
 * - the fact that \f$k_1(p) = k_1(q), \dots, k_n(p) = k_n(q)\f$ does
 * not necessarily imply that \f$p \cong q\f$.
 * 
 * At every call to the @ref add function with a new value, users can
 * provide some metadata associated to the new value. The object can be
 * added in two ways:
 * - Keep only unique instances. If the new value already exists in the
 * structure, the metadata provided is merged with the metadata of its
 * corresponding representative via the '+=' operator. In this case,
 * @e value_t is a type that must be Comparable via either the '==' operator
 * (see concept @ref EqualityComparable) or the '<' operator (see concept
 * @ref LessthanComparable). Lastly, and @e metadata_t is a type that
 * implements the += operator (see concept @ref Incrementable).
 * - Keep all objects. Store the new object at the corresponding leaf of
 * the tree ignoring possible repeats.
 * 
 * @tparam value_t Type of the values to store uniquely.
 * @tparam metadata_ Type of the metadata object associated to every unique
 * value.
 * @tparam keys_t The types of the values returned by the key functions.
 */
template <typename value_t, typename metadata_t, Comparable... keys_t>
class ctree;

/// Iterator class over the leaves of a tree @ref ir_tree.
template <typename value_t, typename metadata_t, Comparable... keys_t>
class iterator;

/// Constant iterator class over the leaves of a tree @ref ir_tree.
template <typename value_t, typename metadata_t, Comparable... keys_t>
class const_iterator;

/**
 * @brief Iterator class over the leaves of a tree @ref ir_tree.
 *
 * This class iterates over ranges of values of the keys determined by a
 * series of functions passed as parameter.
 */
template <typename value_t, typename metadata_t, Comparable... keys_t>
class range_iterator;

/**
 * @brief Constant iterator class over the leaves of a tree @ref ir_tree.
 *
 * This class iterates over ranges of values of the keys determined by a
 * series of functions passed as parameter.
 */
template <typename value_t, typename metadata_t, Comparable... keys_t>
class const_range_iterator;

/// Implementation details.
namespace detail {

/// Pointer type to an instance of @ref ir_tree.
template <typename value_t, typename metadata_t, Comparable... keys_t>
struct pointer {
	using type = ctree<value_t, metadata_t, keys_t...> *;
};

/// Pointer type to an instance of @ref ir_tree.
template <typename value_t, typename metadata_t, Comparable... keys_t>
using pointer_t = pointer<value_t, metadata_t, keys_t...>::type;

/// Constant pointer type to an instance of @ref ir_tree.
template <typename value_t, typename metadata_t, Comparable... keys_t>
struct const_pointer {
	using type = const ctree<value_t, metadata_t, keys_t...> *;
};

/// Constant pointer type to an instance of @ref ir_tree.
template <typename value_t, typename metadata_t, Comparable... keys_t>
using const_pointer_t = const_pointer<value_t, metadata_t, keys_t...>::type;

} // namespace detail
} // namespace classtree

#include <ctree/ctree_empty.hpp>
#include <ctree/ctree_full.hpp>
