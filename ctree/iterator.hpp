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
#include <tuple>

// ctree includes
#include <ctree/ctree.hpp>

namespace classtree {
namespace detail {

/**
 * @brief Partial template specialization of the @ref iterator_ class.
 * @tparam tree_pointer_t Type of the pointer to the tree iterated on.
 * @tparam container_iterator_t Type of the iterator over the keys of the tree iterated on.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename tree_pointer_t,
	typename container_iterator_t,
	typename value_t,
	typename metadata_t,
	Comparable... keys_t>
class iterator_;

/**
 * @brief Shorthand for the type of iterator over the children of the current subtree.
 * @tparam tree_pointer_t Type of the pointer to the tree iterated on.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename tree_pointer_t,
	typename value_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
struct sub_iterator {

	/// Is @e tree_pointer_t constant?
	static constexpr bool is_constant = std::is_same_v<
		tree_pointer_t,
		const_pointer_t<value_t, metadata_t, key_t, keys_t...>>;

	/// Shorthand for a non-constant pointer type.
	using non_const_pointer_type = pointer_t<value_t, metadata_t, keys_t...>;
	/// Shorthand for a constant pointer type.
	using const_pointer_type = const_pointer_t<value_t, metadata_t, keys_t...>;

	/// Shorthand for a non-constant iterator type.
	using non_const_iterator_type = iterator_<
		non_const_pointer_type,
		typename ctree<value_t, metadata_t, keys_t...>::container_t::iterator,
		value_t,
		metadata_t,
		keys_t...>;

	/// Shorthand for a constant iterator type.
	using const_iterator_type = iterator_<
		const_pointer_type,
		typename ctree<value_t, metadata_t, keys_t...>::container_t::
			const_iterator,
		value_t,
		metadata_t,
		keys_t...>;

	/// Shorthand for the correct type of iterator over the current subtree.
	using type = typename std::conditional_t<
		is_constant,
		const_iterator_type,
		non_const_iterator_type>;
};

/**
 * @brief Shorthand for the type of iterator over the children of the current subtree.
 * @tparam tree_pointer_t Type of the pointer to the tree iterated on.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename tree_pointer_t,
	typename value_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
using sub_iterator_t =
	sub_iterator<tree_pointer_t, value_t, metadata_t, key_t, keys_t...>::type;

/**
 * @brief Partial template specialization of the @ref iterator_ class.
 * @tparam tree_pointer_t Type of the pointer to the tree iterated on.
 * @tparam container_iterator_t Type of the iterator over the keys of the tree iterated on.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <
	typename tree_pointer_t,
	typename container_iterator_t,
	typename value_t,
	typename metadata_t>
class iterator_<tree_pointer_t, container_iterator_t, value_t, metadata_t> {
public:

	/// Set the pointer of the tree to iterate on.
	void set_pointer(tree_pointer_t tree) noexcept
	{
		m_tree = tree;
	}

	/// Place the iterator at the beginning.
	void to_begin() noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			return;
		}

		m_past_begin = false;
		m_it = m_tree->begin();
	}
	/// Place the iterator at the end.
	void to_end() noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			return;
		}

		m_past_begin = false;
		m_it = m_tree->end();
		--m_it;
	}

	/// Move forward one value in the iteration.
	void operator++ () noexcept
	{
		m_past_begin = false;
		++m_it;
	}
	/**
	 * @brief Move back one value in the iteration.
	 *
	 * In case the iterator is at the beginning of the iteration (see
	 * @ref begin) a flag (see @ref m_past_begin) is activated to detect a
	 * 'past begin' situation.
	 */
	void operator-- () noexcept
	{
		if (begin()) [[unlikely]] {
			m_past_begin = true;
		}
		else [[likely]] {
			--m_it;
		}
	}

	/// Is the iteration at the beginning?
	[[nodiscard]] bool begin() const noexcept
	{
		return m_it == m_tree->begin();
	}
	/// Is the iteration past the beginning?
	[[nodiscard]] bool past_begin() const noexcept
	{
		return m_past_begin;
	}
	/// Is the iteration at the end?
	[[nodiscard]] bool end() const noexcept
	{
		return m_it == m_tree->end();
	}

	/// Returns the current value of the iteration.
	template <
		typename _inner_pointer_t = tree_pointer_t,
		std::enable_if_t<
			std::is_same_v<pointer_t<value_t, metadata_t>, _inner_pointer_t>,
			bool> = true>
	std::pair<value_t, metadata_t>& operator* () noexcept
	{
		return *m_it;
	}
	/// Returns the current value of the iteration.
	const std::pair<value_t, metadata_t>& operator* () const noexcept
	{
		return *m_it;
	}

	/// Returns the current value of the iteration.
	std::tuple<value_t, metadata_t> operator+ () const noexcept
	{
		return std::make_tuple(m_it->first, m_it->second);
	}

protected:

	/// Pointer to the tree iterated on.
	tree_pointer_t m_tree;

	/// Index to the child of @ref m_tree currently being iterated.
	container_iterator_t m_it;

	/// Has the iterator reached the beginning and tried to move back?
	bool m_past_begin = false;
};

/**
 * @brief Partial template specialization of the @ref iterator_ class.
 * @tparam tree_pointer_t Type of the pointer to the tree iterated on.
 * @tparam container_iterator_t Type of the iterator over the keys of the tree iterated on.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename tree_pointer_t,
	typename container_iterator_t,
	typename value_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
class iterator_<
	tree_pointer_t,
	container_iterator_t,
	value_t,
	metadata_t,
	key_t,
	keys_t...> {
public:

	/// Set the pointer of the tree to iterate on.
	void set_pointer(tree_pointer_t tree) noexcept
	{
		m_tree = tree;
	}

	/// Initialize the iteration at the beginning.
	void to_begin() noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			return;
		}

		m_past_begin = false;
		m_it = m_tree->begin();
		m_subtree_iterator.set_pointer(&m_it->second);
		m_subtree_iterator.to_begin();
	}
	/// Initialize the iteration at the end.
	void to_end() noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			return;
		}

		m_past_begin = false;
		m_it = m_tree->end();
		--m_it;
		m_subtree_iterator.set_pointer(&m_it->second);
		m_subtree_iterator.to_end();
	}

	/// Advance one value in the iteration.
	void operator++ () noexcept
	{
		m_past_begin = false;
		++m_subtree_iterator;
		if (m_subtree_iterator.end()) {
			++m_it;
			if (not end()) {
				m_subtree_iterator.set_pointer(&m_it->second);
				m_subtree_iterator.to_begin();
			}
		}
	}
	/**
	 * @brief Move back one value in the iteration.
	 *
	 * In case the iterator is at the beginning of the iteration (see
	 * @ref begin) a flag (see @ref m_past_begin) is activated to detect a
	 * 'past begin' situation.
	 */
	void operator-- () noexcept
	{
		--m_subtree_iterator;
		if (m_subtree_iterator.past_begin()) [[unlikely]] {

			if (begin()) [[unlikely]] {
				m_past_begin = true;
			}
			else [[likely]] {
				--m_it;
				m_subtree_iterator.set_pointer(&m_it->second);
				m_subtree_iterator.to_end();
			}
		}
	}

	/// Is the iteration at the beginning?
	[[nodiscard]] bool begin() const noexcept
	{
		return m_it == m_tree->begin();
	}
	/// Is the iteration past the beginning?
	[[nodiscard]] bool past_begin() const noexcept
	{
		return m_past_begin;
	}
	/// Is the iteration at the end?
	[[nodiscard]] bool end() const noexcept
	{
		return m_it == m_tree->end();
	}

	/// Returns the current value of the iteration.
	template <
		typename _inner_pointer_t = tree_pointer_t,
		std::enable_if_t<
			std::is_same_v<
				pointer_t<value_t, metadata_t, key_t, keys_t...>,
				_inner_pointer_t>,
			bool> = true>
	std::pair<value_t, metadata_t>& operator* () noexcept
	{
		return *m_subtree_iterator;
	}
	/// Returns the current value of the iteration.
	const std::pair<value_t, metadata_t>& operator* () const noexcept
	{
		return *m_subtree_iterator;
	}

	/// Returns the current value of the iteration.
	std::tuple<value_t, metadata_t, key_t, keys_t...>
	operator+ () const noexcept
	{
		std::tuple<value_t, metadata_t, keys_t...> subtree =
			+m_subtree_iterator;

		std::tuple<value_t, metadata_t, key_t, keys_t...> current;
		std::get<0>(current) = std::get<0>(std::move(subtree));
		std::get<1>(current) = std::get<1>(std::move(subtree));
		std::get<2>(current) = m_it->first;
		move_tuple_into<3, 3 + sizeof...(keys_t)>(current, std::move(subtree));
		return current;
	}

protected:

	template <std::size_t i, std::size_t total_size>
	static constexpr void move_tuple_into(
		std::tuple<value_t, metadata_t, key_t, keys_t...>& current,
		std::tuple<value_t, metadata_t, keys_t...>&& subtree
	) noexcept
	{
		if constexpr (i < total_size) {
			std::get<i>(current) = std::get<i - 1>(std::move(subtree));
			move_tuple_into<i + 1, total_size>(current, std::move(subtree));
		}
	}

protected:

	/// Pointer to the tree iterated on.
	tree_pointer_t m_tree;

	/// Iterator to the key of @ref m_tree currently being iterated.
	container_iterator_t m_it;

	/// Has the iterator reached the beginning and tried to move back?
	bool m_past_begin = false;

	/// Iterator over the children of @ref m_tree.
	sub_iterator_t<tree_pointer_t, value_t, metadata_t, key_t, keys_t...>
		m_subtree_iterator;
};

} // namespace detail

// --------------------------------------------------------------------------

/**
 * @brief Partial template specialization of the @ref iterator class.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <typename value_t, typename metadata_t>
class iterator<value_t, metadata_t>
	: public detail::iterator_<
		  detail::pointer_t<value_t, metadata_t>,
		  typename ctree<value_t, metadata_t>::container_t::iterator,
		  value_t,
		  metadata_t> {
public:
};

/**
 * @brief Partial template specialization of the @ref iterator class.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename value_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
class iterator<value_t, metadata_t, key_t, keys_t...>
	: public detail::iterator_<
		  detail::pointer_t<value_t, metadata_t, key_t, keys_t...>,
		  typename ctree<value_t, metadata_t, key_t, keys_t...>::container_t::
			  iterator,
		  value_t,
		  metadata_t,
		  key_t,
		  keys_t...> {
public:
};

/**
 * @brief Partial template specialization of the @ref const_iterator class.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <typename value_t, typename metadata_t>
class const_iterator<value_t, metadata_t>
	: public detail::iterator_<
		  detail::const_pointer_t<value_t, metadata_t>,
		  typename ctree<value_t, metadata_t>::container_t::const_iterator,
		  value_t,
		  metadata_t> {
public:
};

/**
 * @brief Partial template specialization of the @ref const_iterator class.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename value_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
class const_iterator<value_t, metadata_t, key_t, keys_t...>
	: public detail::iterator_<
		  detail::const_pointer_t<value_t, metadata_t, key_t, keys_t...>,
		  typename ctree<value_t, metadata_t, key_t, keys_t...>::container_t::
			  const_iterator,
		  value_t,
		  metadata_t,
		  key_t,
		  keys_t...> {
public:
};

} // namespace classtree
