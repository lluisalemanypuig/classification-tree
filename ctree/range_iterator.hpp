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
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename tree_pointer_t,
	typename container_iterator_t,
	typename data_t,
	typename metadata_t,
	Comparable... keys_t>
class range_iterator_;

/**
 * @brief Shorthand for the type of iterator over the children of the current subtree.
 * @tparam tree_pointer_t Type of the pointer to the tree iterated on.
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename tree_pointer_t,
	typename data_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
struct sub_range_iterator {

	/// Is @e tree_pointer_t const-qualified?
	static constexpr bool is_constant = std::is_same_v<
		tree_pointer_t,
		const_pointer_t<data_t, metadata_t, key_t, keys_t...>>;

	/// Shorthand for a non-constant pointer type.
	using non_const_pointer_type = pointer_t<data_t, metadata_t, keys_t...>;
	/// Shorthand for a constant pointer type.
	using const_pointer_type = const_pointer_t<data_t, metadata_t, keys_t...>;

	/// Shorthand for a non-constant iterator type.
	using non_const_range_iterator_type = range_iterator_<
		non_const_pointer_type,
		typename ctree<data_t, metadata_t, keys_t...>::container_t::iterator,
		data_t,
		metadata_t,
		keys_t...>;

	/// Shorthand for a constant iterator type.
	using const_range_iterator_type = range_iterator_<
		const_pointer_type,
		typename ctree<data_t, metadata_t, keys_t...>::container_t::
			const_iterator,
		data_t,
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
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename tree_pointer_t,
	typename data_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
using sub_range_iterator_t =
	sub_range_iterator<tree_pointer_t, data_t, metadata_t, key_t, keys_t...>::
		type;

/**
 * @brief Partial template specialization of the @ref iterator_ class.
 *
 * This class iterates over leaf nodes.
 * @tparam tree_pointer_t Type of the pointer to the tree iterated on.
 * @tparam container_iterator_t Type of the iterator over the keys of the tree iterated on.
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <
	typename tree_pointer_t,
	typename container_iterator_t,
	typename data_t,
	typename metadata_t>
class range_iterator_<
	tree_pointer_t,
	container_iterator_t,
	data_t,
	metadata_t> {
public:

	/// Shorthand for a useful type.
	using leaf_element_t = element_t<data_t, metadata_t>;

public:

	/// Set the pointer of the tree to iterate on.
	void set_pointer(tree_pointer_t tree) noexcept
	{
		m_tree = tree;
	}

	/**
	 * @brief Set the search functions that describe the search criteria.
	 *
	 * No functions are needed for a leaf iterator.
	 */
	void set_functions() noexcept { }

	/**
	 * @brief Place the iterator at the beginning of the iteration.
	 * @returns True if an element that meets the criteria specified by @ref m_func
	 * was found in this node and down to the leaves of the tree. Returns false
	 * if no element was found to match the search criteria in this leaf node.
	 */
	[[nodiscard]] bool to_begin() noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		if (m_tree->num_keys() == 0) [[unlikely]] {
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
	 * if no element was found to match the search criteria in this leaf node.
	 */
	[[nodiscard]] bool to_end() noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		if (m_tree->num_keys() == 0) [[unlikely]] {
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
	 * @ref begin) a flag (see @ref past_begin) is activated to detect a
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

	/// Count the number of elements that match the search criteria.
	[[nodiscard]] size_t count() const noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		return m_tree->size();
	}

	/**
	 * @brief Is the iteration at the beginning?
	 *
	 * If so, the iterator can be incremented (++) and decremented (--).
	 */
	[[nodiscard]] bool begin() const noexcept
	{
		return m_tree != nullptr and not m_past_begin and
			   m_it == m_tree->begin();
	}
	/**
	 * @brief Is the iteration past the beginning?
	 *
	 * If so, the iterator cannot be decremented (--).
	 */
	[[nodiscard]] bool past_begin() const noexcept
	{
		return m_tree == nullptr or m_past_begin;
	}
	/**
	 * @brief Is the iteration past the end?
	 *
	 * If so, the iterator cannot be incremented (++).
	 */
	[[nodiscard]] bool end() const noexcept
	{
		return m_tree == nullptr or m_it == m_tree->end();
	}

	/// Returns the current value of the iteration.
	template <
		typename _inner_pointer_t = tree_pointer_t,
		std::enable_if_t<
			std::is_same_v<pointer_t<data_t, metadata_t>, _inner_pointer_t>,
			bool> = true>
	leaf_element_t& operator* () noexcept
	{
		return *m_it;
	}
	/// Returns the current value of the iteration.
	const leaf_element_t& operator* () const noexcept
	{
		return *m_it;
	}

	/// Returns the current value of the iteration.
	std::tuple<leaf_element_t> operator+ () const noexcept
	{
		if constexpr (Compound<data_t, metadata_t>) {
			return std::make_tuple(leaf_element_t{m_it->data, m_it->metadata});
		}
		else {
			return std::make_tuple(*m_it);
		}
	}

private:

	/**
	 * @brief Initialize the beginning and ending limits of this iterator.
	 *
	 * Since this iterator iterates over leaf nodes, there are no limits
	 * to specify.
	 */
	void initialize_limits_begin() noexcept { }
	void initialize_limits_end() noexcept { }

private:

	/// Pointer over the tree iterated on.
	tree_pointer_t m_tree = nullptr;

	/// Iterator over the child of @ref m_tree currently being iterated.
	container_iterator_t m_it;

	/// Has the iterator reached the beginning and tried to move back?
	bool m_past_begin = false;
};

/**
 * @brief Partial template specialization of the @ref range_iterator class.
 *
 * This iterator iterates over internal nodes. This class contains a subtree
 * iterator, which is just another iterator over the nodes of the next level
 * of the tree.
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename tree_pointer_t,
	typename container_iterator_t,
	typename data_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
class range_iterator_<
	tree_pointer_t,
	container_iterator_t,
	data_t,
	metadata_t,
	key_t,
	keys_t...> {
public:

	/// Shorthand for a useful type.
	using leaf_element_t = element_t<data_t, metadata_t>;

public:

	/// Set the pointer of the tree to iterate on.
	void set_pointer(tree_pointer_t tree) noexcept
	{
		m_tree = tree;
	}

	/**
	 * @brief Set the search functions that describe the search criteria.
	 *
	 * The first function is assigned to this node. The second and the remaining
	 * functions are assigned to the remaining iterators.
	 */
	template <typename Callable, typename... Callables>
	void set_functions(Callable&& f, Callables&&...fs) noexcept
	{
		m_func = std::move(f);
		m_subtree_iterator.set_functions(std::forward<Callables>(fs)...);
	}

	/**
	 * @brief Place the iterator at the beginning of the iteration.
	 * @returns True if an element that meets the criteria specified by @ref m_func
	 * was found in this node and down to the leaves of the tree. Returns false
	 * if no element was found to match the search criteria in this internal node.
	 */
	[[nodiscard]] bool to_begin() noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		if (m_tree->num_keys() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			m_begin_idx = 1;
			m_it_idx = 1;
			m_end_idx = 1;
			return false;
		}

		return initialize_limits_begin();
	}

	/**
	 * @brief Place the iterator at the end of the iteration.
	 * @returns True if an element that meets the criteria specified by @ref m_func
	 * was found in this node and down to the leaves of the tree. Returns false
	 * if no element was found to match the search criteria in this internal node.
	 */
	[[nodiscard]] bool to_end() noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		if (m_tree->num_keys() == 0) [[unlikely]] {
			m_past_begin = true;
			m_it = m_tree->end();
			m_begin_idx = 1;
			m_it_idx = 1;
			m_end_idx = 1;
			return false;
		}

		return initialize_limits_end();
	}

	/// Advance one step in the iteration.
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
	 * @brief Move back one step in the iteration.
	 *
	 * In case the iterator is at the beginning of the iteration (see
	 * @ref begin) a flag (see @ref m_past_begin) is activated to detect a
	 * 'past begin' situation.
	 */
	void operator-- () noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

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

	/// Count the number of elements that match the search criteria.
	[[nodiscard]] size_t count() noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		m_it = m_tree->begin();
		m_it_idx = 0;
		m_begin_idx = 0;
		m_end_idx = m_tree->num_keys();

		size_t c = 0;
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

	/**
	 * @brief Is the iteration at the beginning?
	 *
	 * If so, the iterator can be incremented (++) and decremented (--).
	 */
	[[nodiscard]] bool begin() const noexcept
	{
		if (m_tree == nullptr or m_tree->num_keys() == 0) [[unlikely]] {
			return false;
		}
		return shallow_begin() and m_subtree_iterator.begin();
	}
	/**
	 * @brief Is the iteration past the beginning?
	 *
	 * If so, the iterator cannot be decremented (--).
	 */
	[[nodiscard]] bool past_begin() const noexcept
	{
		if (m_tree == nullptr or m_tree->num_keys() == 0) [[unlikely]] {
			return true;
		}
		return shallow_past_begin() and m_subtree_iterator.past_begin();
	}
	/**
	 * @brief Is the iteration past the end?
	 *
	 * If so, the iterator cannot be incremented (++).
	 */
	[[nodiscard]] bool end() const noexcept
	{
		if (m_tree == nullptr or m_tree->num_keys() == 0) [[unlikely]] {
			return true;
		}
		return shallow_end() and m_subtree_iterator.end();
	}

	/// Returns the current value of the iteration.
	template <
		typename _inner_pointer_t = tree_pointer_t,
		std::enable_if_t<
			std::is_same_v<
				pointer_t<data_t, metadata_t, key_t, keys_t...>,
				_inner_pointer_t>,
			bool> = true>
	leaf_element_t& operator* () noexcept
	{
		return *m_subtree_iterator;
	}
	/// Returns the current value of the iteration.
	const leaf_element_t& operator* () const noexcept
	{
		return *m_subtree_iterator;
	}

	/// Returns the current value of the iteration.
	std::tuple<leaf_element_t, key_t, keys_t...> operator+ () const noexcept
	{
		std::tuple<leaf_element_t, keys_t...> subtree = +m_subtree_iterator;

		std::tuple<leaf_element_t, key_t, keys_t...> current;
		std::get<0>(current) = std::get<0>(std::move(subtree));
		std::get<1>(current) = m_it->first;
		move_tuple_into<2, 2 + sizeof...(keys_t)>(current, std::move(subtree));
		return current;
	}

private:

	/**
	 * @brief Initialize the pointers of the iteration.
	 *
	 * This routine is optimized for iterations that start at the beginning.
	 */
	[[nodiscard]] bool initialize_limits_begin() noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		// prepare the pointers
		m_past_begin = false;
		m_begin_idx = 0;
		m_it = m_tree->end();
		--m_it;
		m_it_idx = m_tree->num_keys() - 1;
		m_end_idx = m_tree->num_keys();

		// find the last element
		const bool found_last = previous();
		if (not found_last) {
			m_it = m_tree->begin();
			m_it_idx = m_tree->num_keys();
			m_begin_idx = m_tree->num_keys();
			m_end_idx = m_tree->num_keys();
			m_past_begin = true;
			return false;
		}

#if defined DEBUG
		assert(not m_past_begin);
#endif

		// capture the right limit (end)
		m_end_idx = m_it_idx + 1;

		m_it = m_tree->begin();
		m_it_idx = 0;

		// find the first element
		[[maybe_unused]] const bool found_first = next();
#if defined DEBUG
		// The first element must have been found due to the simple fact that
		// if there was a 'last' then there must be a first. Notice that the
		// first and last elements may coincide.
		assert(found_first);
#endif

		// capture the left limit (begin)
		m_begin_idx = m_it_idx;

#if defined DEBUG
		assert(m_begin_idx < m_end_idx);
#endif
		return true;
	}

	/**
	 * @brief Initialize the pointers of the iteration.
	 *
	 * This routine is optimized for iterations that start at the end.
	 */
	[[nodiscard]] bool initialize_limits_end() noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		// prepare the pointers
		m_begin_idx = 0;
		m_it = m_tree->begin();
		m_it_idx = 0;
		m_end_idx = m_tree->num_keys();
		m_past_begin = false;

		// find the first element
		const bool found_first = next();
		if (not found_first) {
			m_it = m_tree->begin();
			m_it_idx = m_tree->num_keys();
			m_begin_idx = m_tree->num_keys();
			m_end_idx = m_tree->num_keys();
			m_past_begin = true;
			return false;
		}
#if defined DEBUG
		assert(not m_past_begin);
#endif

		// capture the left limit (begin)
		m_begin_idx = m_it_idx;

		m_it = m_tree->end();
		--m_it;
		m_it_idx = m_tree->num_keys() - 1;

		// find the last element
		[[maybe_unused]] const bool found_last = previous();
#if defined DEBUG
		// The last element must have been found due to the simple fact that
		// if there was a 'first' then there must be a last. Notice that the
		// first and last elements may coincide.
		assert(found_last);
#endif

		// capture the right limit (end)
		m_end_idx = m_it_idx + 1;

#if defined DEBUG
		assert(m_begin_idx < m_end_idx);
#endif
		return true;
	}

	/**
	 * @brief Move a tuple into another.
	 * @tparam i Index of the element to move.
	 * @tparam total_size The total amount of elements.
	 * @param to The tuple into which the elements will be moved.
	 * @param from The tuple from which the elements will be moved.
	 */
	template <size_t i, size_t total_size>
	static constexpr void move_tuple_into(
		std::tuple<leaf_element_t, key_t, keys_t...>& to,
		std::tuple<leaf_element_t, keys_t...>&& from
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
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		return not shallow_past_begin() and
			   (m_it == m_tree->begin() or m_it_idx == m_begin_idx);
	}
	/// Is the iteration of this node past the beginning?
	[[nodiscard]] bool shallow_past_begin() const noexcept
	{
		return m_tree == nullptr or m_past_begin;
	}
	/// Is the iteration of this node at the end?
	[[nodiscard]] bool shallow_end() const noexcept
	{
#if defined DEBUG
		assert(m_tree != nullptr);
#endif

		return m_it_idx == m_end_idx or m_it == m_tree->end();
	}

	/**
	 * @brief Find the next element according to function @ref m_func.
	 * @returns True if a valid element (according to the functions) was
	 * found, or false if otherwise.
	 * @post If the function returned true, the iterator over the subtree is
	 * initialized to the beginning of its iteration.
	 */
	[[nodiscard]] bool next() noexcept
	{
		bool stop = false;
		while (not stop) {
			bool s_end;
			while (not(s_end = shallow_end()) and not m_func(m_it->first)) {
				++m_it;
				++m_it_idx;
			}
			if (s_end) {
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

	/**
	 * @brief Find the previous element according to function @ref m_func.
	 * @returns True if a valid element (according to the functions) was
	 * found, or false if otherwise.
	 * @post If the function returned true, the iterator over the subtree is
	 * initialized to the end of its iteration.
	 */
	[[nodiscard]] bool previous() noexcept
	{
		bool stop = false;
		while (not stop) {
			bool s_begin;
			while (not(s_begin = shallow_past_begin()) and
				   not m_func(m_it->first)) {
				simple_move_back();
			}
			if (s_begin) {
				return false;
			}

			m_subtree_iterator.set_pointer(&m_it->second);
			stop = m_subtree_iterator.to_end();

			if (not stop) {
				simple_move_back();
			}
		}
		return true;
	}

private:

	/// Filtering function to determine the range of iteration over the current tree.
	std::function<bool(const key_t&)> m_func;

	/// Pointer to the tree iterated on.
	tree_pointer_t m_tree = nullptr;

	/// Iterator to the key of @ref m_tree currently being iterated.
	container_iterator_t m_it;
	/// The index of @ref m_it within the container.
	size_t m_it_idx;

	/// Pointer to the first valid position of this node.
	size_t m_begin_idx;
	/// Pointer to the last + 1 valid position of this node.
	size_t m_end_idx;

	/// Has the iterator reached the beginning and tried to move back?
	bool m_past_begin = false;

	/// Iterator over the children of @ref m_tree.
	sub_range_iterator_t<tree_pointer_t, data_t, metadata_t, key_t, keys_t...>
		m_subtree_iterator;
};

} // namespace detail

/**
 * @brief Partial template specialization of the @ref range_iterator class.
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <typename data_t, typename metadata_t>
class range_iterator<data_t, metadata_t>
	: public detail::range_iterator_<
		  detail::pointer_t<data_t, metadata_t>,
		  typename ctree<data_t, metadata_t>::container_t::iterator,
		  data_t,
		  metadata_t> {
public:
};

/**
 * @brief Partial template specialization of the @ref range_iterator class.
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename data_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
class range_iterator<data_t, metadata_t, key_t, keys_t...>
	: public detail::range_iterator_<
		  detail::pointer_t<data_t, metadata_t, key_t, keys_t...>,
		  typename ctree<data_t, metadata_t, key_t, keys_t...>::container_t::
			  iterator,
		  data_t,
		  metadata_t,
		  key_t,
		  keys_t...> {
public:
};

/**
 * @brief Partial template specialization of the @ref const_range_iterator class.
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <typename data_t, typename metadata_t>
class const_range_iterator<data_t, metadata_t>
	: public detail::range_iterator_<
		  detail::const_pointer_t<data_t, metadata_t>,
		  typename ctree<data_t, metadata_t>::container_t::const_iterator,
		  data_t,
		  metadata_t> {
public:
};

/**
 * @brief Partial template specialization of the @ref const_range_iterator class.
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 * @tparam key_t Type of the first key.
 * @tparam keys_t Type of the remaining keys.
 */
template <
	typename data_t,
	typename metadata_t,
	Comparable key_t,
	Comparable... keys_t>
class const_range_iterator<data_t, metadata_t, key_t, keys_t...>
	: public detail::range_iterator_<
		  detail::const_pointer_t<data_t, metadata_t, key_t, keys_t...>,
		  typename ctree<data_t, metadata_t, key_t, keys_t...>::container_t::
			  const_iterator,
		  data_t,
		  metadata_t,
		  key_t,
		  keys_t...> {
public:
};

} // namespace classtree
