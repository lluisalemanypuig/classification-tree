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
#include <functional>
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
class range_iterator_;

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
struct sub_range_iterator {

	/// Is @e tree_pointer_t constant?
	static constexpr bool is_constant = std::is_same_v<
		tree_pointer_t,
		const_pointer_t<value_t, metadata_t, key_t, keys_t...>>;

	/// Shorthand for a non-constant pointer type.
	using non_const_pointer_type = pointer_t<value_t, metadata_t, keys_t...>;
	/// Shorthand for a constant pointer type.
	using const_pointer_type = const_pointer_t<value_t, metadata_t, keys_t...>;

	/// Shorthand for a non-constant iterator type.
	using non_const_range_iterator_type = range_iterator_<
		non_const_pointer_type,
		typename ctree<value_t, metadata_t, keys_t...>::container_t::iterator,
		value_t,
		metadata_t,
		keys_t...>;

	/// Shorthand for a constant iterator type.
	using const_range_iterator_type = range_iterator_<
		const_pointer_type,
		typename ctree<value_t, metadata_t, keys_t...>::container_t::
			const_iterator,
		value_t,
		metadata_t,
		keys_t...>;

	/// Shorthand for the correct type of iterator over the current subtree.
	using type = typename std::conditional_t<
		is_constant,
		const_range_iterator_type,
		non_const_range_iterator_type>;
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
using sub_range_iterator_t =
	sub_range_iterator<tree_pointer_t, value_t, metadata_t, key_t, keys_t...>::
		type;

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
class range_iterator_<
	tree_pointer_t,
	container_iterator_t,
	value_t,
	metadata_t> {
public:

	/// Set the pointer of the tree to iterate on.
	void set_pointer(tree_pointer_t tree) noexcept
	{
		m_tree = tree;
	}

	void set_functions() noexcept { }
	void initialize_first_and_last() noexcept { }

	/**
	 * @brief Place the iterator at the beginning of the iteration.
	 * @returns True if an element that meets the criteria specified by @ref m_func
	 * was found in this node and down to the leaves of the tree. Returns false
	 * if no element was found.
	 */
	[[nodiscard]] bool to_begin() noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			return false;
		}

		m_past_begin = false;
		m_it = m_tree->begin();
		return true;
	}
	/**
	 * @brief Place the iterator at the end of the iteration.
	 * @returns True if an element that meets the criteria specified by @ref m_func
	 * was found in this node and down to the leaves of the tree. Returns false
	 * if no element was found.
	 */
	[[nodiscard]] bool to_end() noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			return false;
		}

		m_past_begin = false;
		m_it = m_tree->end();
		--m_it;
		return true;
	}

	/// Advance one value in the iteration.
	void operator++ () noexcept
	{
		if (m_past_begin) [[unlikely]] {
			m_past_begin = false;
			return;
		}
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

	[[nodiscard]] std::size_t count() const noexcept
	{
		return m_tree->size();
	}

	/// Is the iteration at the beginning?
	[[nodiscard]] bool begin() const noexcept
	{
		return not m_past_begin and (m_it == m_tree->begin());
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

private:

	/// Pointer over the tree iterated on.
	tree_pointer_t m_tree;

	/// Iterator over the child of @ref m_tree currently being iterated.
	container_iterator_t m_it;

	/// Has the iterator reached the beginning and tried to move back?
	bool m_past_begin = false;
};

/**
 * @brief Partial template specialization of the @ref range_iterator class.
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
class range_iterator_<
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

	template <typename F, typename... Callables>
	void set_functions(F&& f, Callables&&...fs) noexcept
	{
		m_func = std::move(f);
		m_subtree_iterator.set_functions(std::forward<Callables>(fs)...);
	}

	/**
	 * @brief Place the iterator at the beginning of the iteration.
	 * @returns True if an element that meets the criteria specified by @ref m_func
	 * was found in this node and down to the leaves of the tree. Returns false
	 * if no element was found.
	 */
	[[nodiscard]] bool to_begin() noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			m_it_idx = 1;
			return false;
		}

		initialize_first_and_last();

		m_past_begin = false;
		m_it = m_tree->begin();
		m_it_idx = 0;
		return next();
	}
	/**
	 * @brief Place the iterator at the end of the iteration.
	 * @returns True if an element that meets the criteria specified by @ref m_func
	 * was found in this node and down to the leaves of the tree. Returns false
	 * if no element was found.
	 */
	[[nodiscard]] bool to_end() noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			m_it_idx = 1;
			return false;
		}

		initialize_first_and_last();

		m_past_begin = false;
		m_it = m_tree->end();
		--m_it;
		m_it_idx = m_tree->size() - 1;
		return previous();
	}

	/// Advance one value in the iteration.
	void operator++ () noexcept
	{
		m_past_begin = false;
		++m_subtree_iterator;
		if (m_subtree_iterator.end()) {
			++m_it;
			++m_it_idx;
			if (not shallow_end()) [[likely]] {
				[[maybe_unused]] const bool _ = next();
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

			if (shallow_begin()) [[unlikely]] {
				m_past_begin = true;
			}
			else [[likely]] {
				--m_it;
				--m_it_idx;
				[[maybe_unused]] const bool _ = previous();
			}
		}
		if (shallow_end()) {
			--m_it;
			--m_it_idx;
			while (not m_func(m_it->first) and m_it != m_tree->begin()) {
				--m_it;
				--m_it_idx;
			}
		}
	}

	[[nodiscard]] std::size_t count() noexcept
	{
		m_it = m_tree->begin();
		m_it_idx = 0;
		m_begin_idx = 0;
		m_end_idx = m_tree->size();

		std::size_t c = 0;
		while (not shallow_end()) {
			while (not shallow_end() and not m_func(m_it->first)) {
				++m_it;
				++m_it_idx;
			}

			if (not shallow_end()) {
				m_subtree_iterator.set_pointer(&m_it->second);
				c += m_subtree_iterator.count();
				++m_it;
				++m_it_idx;
			}
		}
		return c;
	}

	/// Is the iteration at the beginning?
	[[nodiscard]] bool begin() const noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			return true;
		}
		return shallow_begin() and m_subtree_iterator.begin();
	}
	/// Is the iteration past the beginning?
	[[nodiscard]] bool past_begin() const noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			return true;
		}
		return shallow_past_begin() and m_subtree_iterator.past_begin();
	}
	/// Is the iteration at the end?
	[[nodiscard]] bool end() const noexcept
	{
		if (m_tree->size() == 0) [[unlikely]] {
			return true;
		}
		return shallow_end() and m_subtree_iterator.end();
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

	/**
	 * @brief Finds the first and last+1 valid positions in this node.
	 *
	 * Sets the pointers @ref m_begin_ptr and @ref m_last_ptr.
	 */
	void initialize_first_and_last() noexcept
	{
		m_it = m_tree->begin();
		m_it_idx = 0;
		m_end_idx = m_tree->size() + 3;
		const bool found_begin = next();
		if (not found_begin) {
			m_begin_idx = m_tree->size();
			m_end_idx = m_tree->size();
			return;
		}

		m_begin_idx = m_it_idx;

		m_end_idx = m_tree->size();

		m_past_begin = false;
		m_it = m_tree->end();
		--m_it;
		m_it_idx = m_tree->size() - 1;

		[[maybe_unused]] const bool found_end = previous();
#if defined DEBUG
		assert(found_end);
#endif

		m_end_idx = m_it_idx;
		m_end_idx += (m_it_idx != m_tree->size());
	}

private:

	/**
	 * @brief Move a tuple into another.
	 * @tparam i Index of the element to move.
	 * @tparam total_size The total amount of elements.
	 * @param to The tuple into which the elements will be moved.
	 * @param from The tuple from which the elements will be moved.
	 */
	template <std::size_t i, std::size_t total_size>
	static constexpr void move_tuple_into(
		std::tuple<value_t, metadata_t, key_t, keys_t...>& to,
		std::tuple<value_t, metadata_t, keys_t...>&& from
	) noexcept
	{
		if constexpr (i < total_size) {
			std::get<i>(to) = std::get<i - 1>(std::move(from));
			move_tuple_into<i + 1, total_size>(to, std::move(from));
		}
	}

	/// Is the iteration of this node at the beginning?
	[[nodiscard]] bool shallow_begin() const noexcept
	{
		return not shallow_past_begin() and
			   (m_it == m_tree->begin() or m_it_idx == m_begin_idx);
	}
	/// Is the iteration of this node past the beginning?
	[[nodiscard]] bool shallow_past_begin() const noexcept
	{
		return m_past_begin;
	}
	/// Is the iteration of this node at the end?
	[[nodiscard]] bool shallow_end() const noexcept
	{
		return m_it == m_tree->end() or m_it_idx == m_end_idx;
	}

	/**
	 * @brief Find the next element according to function @ref m_func.
	 * @returns True if a valid element (according to the functions) was
	 * found, or false if otherwise.
	 */
	[[nodiscard]] bool next() noexcept
	{
		bool stop = false;
		while (not stop) {
			while (not shallow_end() and not m_func(m_it->first)) {
				++m_it;
				++m_it_idx;
			}
			if (shallow_end()) {
				return false;
			}

			m_subtree_iterator.set_pointer(&m_it->second);
			stop = m_subtree_iterator.to_begin();

			if (not stop) {
				++m_it;
				++m_it_idx;
			}
		}
		return true;
	}

	/// Move one step backwards. Update @ref m_past_begin appropriately.
	void simple_move_back() noexcept
	{
		if (shallow_begin()) [[unlikely]] {
			m_past_begin = true;
			return;
		}
		--m_it;
		--m_it_idx;
	}

	/// Find the previous element according to function @ref m_func.
	[[nodiscard]] bool previous() noexcept
	{
		bool stop = false;
		while (not stop) {
			while (not shallow_past_begin() and not m_func(m_it->first)) {
				simple_move_back();
			}
			if (shallow_past_begin()) {
				return false;
			}

			m_subtree_iterator.set_pointer(&m_it->second);
			stop = m_subtree_iterator.to_end();

			if (not stop) {
				simple_move_back();
				stop = shallow_past_begin();
			}
		}
		return true;
	}

private:

	/// Filtering function to determine the range of iteration over the current tree.
	std::function<bool(const key_t&)> m_func;

	/// Pointer to the tree iterated on.
	tree_pointer_t m_tree;

	/// Iterator to the key of @ref m_tree currently being iterated.
	container_iterator_t m_it;
	/// The index of @ref m_it within the container.
	std::size_t m_it_idx;

	/// Pointer to the first valid position of this node.
	std::size_t m_begin_idx;
	/// Pointer to the last + 1 valid position of this node.
	std::size_t m_end_idx;

	/// Has the iterator reached the beginning and tried to move back?
	bool m_past_begin = false;

	/// Iterator over the children of @ref m_tree.
	sub_range_iterator_t<tree_pointer_t, value_t, metadata_t, key_t, keys_t...>
		m_subtree_iterator;
};

} // namespace detail

/**
 * @brief Partial template specialization of the @ref range_iterator class.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <typename value_t, typename metadata_t>
class range_iterator<value_t, metadata_t>
	: public detail::range_iterator_<
		  detail::pointer_t<value_t, metadata_t>,
		  typename ctree<value_t, metadata_t>::container_t::iterator,
		  value_t,
		  metadata_t> {
public:
};

/**
 * @brief Partial template specialization of the @ref range_iterator class.
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
class range_iterator<value_t, metadata_t, key_t, keys_t...>
	: public detail::range_iterator_<
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
 * @brief Partial template specialization of the @ref const_range_iterator class.
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <typename value_t, typename metadata_t>
class const_range_iterator<value_t, metadata_t>
	: public detail::range_iterator_<
		  detail::const_pointer_t<value_t, metadata_t>,
		  typename ctree<value_t, metadata_t>::container_t::const_iterator,
		  value_t,
		  metadata_t> {
public:
};

/**
 * @brief Partial template specialization of the @ref const_range_iterator class.
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
class const_range_iterator<value_t, metadata_t, key_t, keys_t...>
	: public detail::range_iterator_<
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
