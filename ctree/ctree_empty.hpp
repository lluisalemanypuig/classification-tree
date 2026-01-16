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
#if defined DEBUG
#include <cassert>
#endif
#include <memory_resource>
#include <ostream>
#include <vector>
#include <ranges>

// custom includes
#include <ctree/ctree.hpp>
#include <ctree/search.hpp>
#include <ctree/types.hpp>

namespace classtree {

/**
 * @brief Partial template specialization of the Classification Tree.
 *
 * This class has no subtrees and is implemented simply as an array of
 * pairs of value and its metadata (see @ref element_t).
 * @tparam data_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <typename data_t, typename metadata_t>
class ctree<data_t, metadata_t> {
public:

	/// Shorthand for a useful type.
	using leaf_element_t = element_t<data_t, metadata_t>;

	/// The container that stores the key values, and the associated subtree.
	using container_t = std::pmr::vector<leaf_element_t>;

	/// Direct access to a nice property of @ref element_t.
	static constexpr bool is_compound = Compound<data_t, metadata_t>;

public:

	/**
	 * @brief Resets the children empty and sets the memory resource
	 *
	 * Resets the @ref m_data vector and sets the memory resource
	 * @param mem_res Memory resource allocator.
	 */
	void set_allocator(std::pmr::memory_resource *mem_res)
	{
		m_data = container_t(mem_res);
	}

	/**
	 * @brief Resizes the allocator of children
	 * @param s Size.
	 */
	void reserve(const size_t s)
	{
		m_data.reserve(s);
	}

	/// Clear the memory occupied by this leaf node.
	void clear() noexcept
	{
		m_data.clear();
	}

	/// Iterator to the key-child pair container.
	[[nodiscard]] container_t::iterator begin() noexcept
	{
		return m_data.begin();
	}
	/// Iterator to the key-child pair container.
	[[nodiscard]] container_t::const_iterator begin() const noexcept
	{
		return m_data.begin();
	}
	/// Iterator to the key-child pair container.
	[[nodiscard]] container_t::iterator end() noexcept
	{
		return m_data.end();
	}
	/// Iterator to the key-child pair container.
	[[nodiscard]] container_t::const_iterator end() const noexcept
	{
		return m_data.end();
	}

	/**
	 * @brief Adds another element to this tree.
	 * @tparam _leaf_element_t Type of the value to add.
	 * @tparam unique Store the element when there are no repeats.
	 * @param value Value to add.
	 * @returns True if the element was not found and added. False if otherwise.
	 */
	template <bool unique = true, typename _leaf_element_t = leaf_element_t>
	bool add(leaf_element_t&& value)
	{
		static_assert(check_types<_leaf_element_t>());

		if constexpr (not unique) {
			// store all objects, regardless of repeats

			if constexpr (LessthanComparable<data_t>) {

				// do binary search and then add
				const auto [i, _] = [&]()
				{
					if constexpr (is_compound) {
						return search<data_t, metadata_t>(m_data, value.data);
					}
					else {
						return search<data_t, metadata_t>(m_data, value);
					}
				}();

				auto it = m_data.begin();
				std::advance(it, i);
				m_data.insert(it, {std::move(value)});
			}
			else {
				// simply add the object and its metadata -- no need
				// to check the types.
				m_data.emplace_back(std::move(value));
			}
			return true;
		}

		/* store unique objects */

		if constexpr (not EqualityComparable<data_t> and
					  not LessthanComparable<data_t>) {
			// this cannot happen if we want to keep unique instances.
			static_assert(false);
			return false;
		}

		if constexpr (LessthanComparable<data_t>) {
			static_assert(LessthanComparable<data_t>);

			// do binary search and then add if needed.
			const auto [i, exists] = [&]()
			{
				if constexpr (is_compound) {
					return search<data_t, metadata_t>(m_data, value.data);
				}
				else {
					return search<data_t, metadata_t>(m_data, value);
				}
			}();

			if (exists) {
				if constexpr (Mergeable<metadata_t>) {
					static_assert(is_compound);
					m_data[i].metadata += std::move(value.metadata);
				}
				return false;
			}
			auto it = m_data.begin();
			std::advance(it, i);
			m_data.insert(it, std::move(value));
			return true;
		}

		if constexpr (EqualityComparable<data_t>) {
			const auto it = std::find_if(
				m_data.begin(),
				m_data.end(),
				[&](const leaf_element_t& e) -> bool
				{
					if constexpr (is_compound) {
						return e.data == value.data;
					}
					else {
						return e == value;
					}
				}
			);

			if (it == m_data.end()) {
				m_data.emplace_back(std::move(value));
				return true;
			}
			if constexpr (Mergeable<metadata_t>) {
				static_assert(is_compound);
				it->metadata += std::move(value.metadata);
			}
			return false;
		}
	}

	/**
	 * @brief Adds another element to this tree.
	 *
	 * This method assumes that this leaf is empty.
	 * @tparam _leaf_element_t Type of the value to add.
	 * @tparam unique Store the element when there are no repeats.
	 * @param value Value to add.
	 * @returns True if the element was not found and added. False if otherwise.
	 */
	template <bool unique = true, typename _leaf_element_t = leaf_element_t>
	bool add_empty(_leaf_element_t&& value)
	{
		static_assert(check_types<_leaf_element_t>());
		m_data.emplace_back(std::move(value));
		return true;
	}

	/**
	 * @brief Merges another tree into this tree.
	 * @tparam unique Store the elements of the new tree so that there are no repeats.
	 * @param t The tree to be merged into this tree.
	 * @returns The difference of the new size and the old size.
	 */
	template <bool unique = true>
	size_t merge(ctree<data_t, metadata_t>&& t)
	{
		size_t added_elems = 0;
		for (auto& v : t.m_data) {
			added_elems += add<unique>(std::move(v));
		}
		return added_elems;
	}

	/**
	 * @brief The number of unique elements over all leaves of this tree.
	 * @returns The number of unique elements over all leaves of this tree.
	 */
	[[nodiscard]] size_t size() const noexcept
	{
		return m_data.size();
	}
	/**
	 * @brief The number of keys in this node.
	 *
	 * This is equal to the number of children of this node.
	 * @returns 0 since there are no children in this leaf node.
	 */
	[[nodiscard]] size_t num_keys() const noexcept
	{
		return 0;
	}
	/**
	 * @brief The number of bytes occupied by this leaf node.
	 *
	 * This value is calculated.
	 * @returns The number of bytes that this tree requires.
	 */
	[[nodiscard]] size_t num_bytes() const noexcept
	{
		return m_data.size() * sizeof(leaf_element_t);
	}

	/**
	 * @brief Returns the @e i-th child of this node.
	 * @param i A valid index. Must be less than @ref num_keys().
	 * @returns A non-constant reference to a key value.
	 */
	[[nodiscard]] leaf_element_t& get_child(const size_t i) noexcept
	{
#if defined DEBUG
		assert(i < m_data.size());
#endif
		return m_data[i];
	}
	/**
	 * @brief Returns the @e i-th child of this node.
	 * @param i A valid index. Must be less than @ref num_keys().
	 * @returns A non-constant reference to a key value.
	 */
	[[nodiscard]] const leaf_element_t& get_child(const size_t i) const noexcept
	{
#if defined DEBUG
		assert(i < m_data.size());
#endif
		return m_data[i];
	}

	/**
	 * @brief Prints this tree to the output specified by @e os.
	 * @param os Output stream.
	 * @param print_leaves Whether or not to print the values in the leaves of the tree.
	 * @param tab The tabulator string.
	 */
	void print(
		std::ostream& os,
		const bool print_leaves = true,
		const std::string& tab = ""
	) const
	{
		os << tab << "^ size: " << size() << '\n';
		if (print_leaves) {
			const auto s = m_data.size();
			for (const auto& [i, value] : m_data | std::views::enumerate) {

				if (static_cast<size_t>(i) < s - 1) {
					os << tab << "├── ";
				}
				else {
					os << tab << "└── ";
				}

				if constexpr (is_compound) {
					const auto& [v, meta] = value;
					os << v << ' ' << meta << '\n';
				}
				else {
					const auto& v = value;
					os << v << '\n';
				}
			}
		}
	}

	/// Returns an iterator object over the leaves of this tree.
	[[nodiscard]] iterator<data_t, metadata_t> get_iterator() noexcept
	{
		iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	[[nodiscard]] iterator<data_t, metadata_t> get_iterator_begin() noexcept
	{
		iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		it.to_begin();
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the end of the iteration.
	 */
	[[nodiscard]] iterator<data_t, metadata_t> get_iterator_end() noexcept
	{
		iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		it.to_end();
		return it;
	}
	/// Returns an iterator object over the leaves of this tree.
	[[nodiscard]] const_iterator<data_t, metadata_t>
	get_const_iterator() const noexcept
	{
		const_iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns a constant iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	[[nodiscard]] const_iterator<data_t, metadata_t>
	get_const_iterator_begin() const noexcept
	{
		const_iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		it.to_begin();
		return it;
	}
	/**
	 * @brief Returns a constant iterator object over the leaves of this tree.
	 *
	 * Starts at the end of the iteration.
	 */
	[[nodiscard]] const_iterator<data_t, metadata_t>
	get_const_iterator_end() const noexcept
	{
		const_iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		it.to_end();
		return it;
	}

	/// Returns an iterator object over the leaves of this tree.
	[[nodiscard]] range_iterator<data_t, metadata_t>
	get_range_iterator() noexcept
	{
		range_iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns a range iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	[[nodiscard]] range_iterator<data_t, metadata_t>
	get_range_iterator_begin() noexcept
	{
		range_iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		[[maybe_unused]] const auto _ = it.to_begin();
		return it;
	}
	/**
	 * @brief Returns a range iterator object over the leaves of this tree.
	 *
	 * Starts at the end of the iteration.
	 */
	[[nodiscard]] range_iterator<data_t, metadata_t>
	get_range_iterator_end() noexcept
	{
		range_iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		[[maybe_unused]] const auto _ = it.to_end();
		return it;
	}
	/// Returns an iterator object over the leaves of this tree.
	[[nodiscard]] const_range_iterator<data_t, metadata_t>
	get_const_range_iterator() const noexcept
	{
		const_range_iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns a range iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	[[nodiscard]] const_range_iterator<data_t, metadata_t>
	get_const_range_iterator_begin() const noexcept
	{
		const_range_iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		[[maybe_unused]] const auto _ = it.to_begin();
		return it;
	}
	/**
	 * @brief Returns a range iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	[[nodiscard]] const_range_iterator<data_t, metadata_t>
	get_const_range_iterator_end() const noexcept
	{
		const_range_iterator<data_t, metadata_t> it;
		it.set_pointer(this);
		[[maybe_unused]] const auto _ = it.to_end();
		return it;
	}

#if defined CTREE_DEBUG
	/// Ensure that the all keys in every node of this tree are sorted.
	[[nodiscard]] bool check_sorted_keys() const noexcept
	{
		// there are no keys in this tree
		return true;
	}
#endif

private:

	/**
	 * @brief Ensure that the template parameters of this function are the
	 * same as those in the template parameters of this class.
	 *
	 * Constant and reference qualifiers are removed prior to comparing.
	 * @tparam _leaf_element_t Type of the value to add.
	 * @returns True if all the types are same. False if otherwise.
	 */
	template <typename _leaf_element_t>
	[[nodiscard]] static consteval bool check_types() noexcept
	{
		return std::
			is_same_v<std::remove_cvref_t<_leaf_element_t>, leaf_element_t>;
	}

private:

	/// The unique elements in this tree.
	container_t m_data;
};

} // namespace classtree
