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
#include <algorithm>
#include <vector>
#include <ranges>

// custom includes
#include <ctree/search.hpp>
#include <ctree/type_traits.hpp>
#include <ctree/ctree.hpp>
#include <ctree/types.hpp>

namespace classtree {

/**
 * @brief Partial template specialization of the Classification Tree.
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
class ctree<data_t, metadata_t, key_t, keys_t...> {
public:

	/// Shorthand for a useful type.
	using leaf_element_t = element_t<data_t, metadata_t>;

	/// Type of the children nodes.
	using child_t = ctree<data_t, metadata_t, keys_t...>;

	/// Node type, a pair of key value and its associated subtree.
	using subtree_t = std::pair<key_t, child_t>;

	/// The container that stores the key values, and the associated subtree.
	using container_t = std::pmr::vector<subtree_t>;

public:

	/**
	 * @brief Resets the children empty and sets the memory resource
	 *
	 * Resets the @ref m_children vector and sets the memory resource
	 * @param mem_res Memory resource allocator.
	 */
	void set_allocator(std::pmr::memory_resource *mem_res)
	{
		m_children = container_t(mem_res);
	}

	/**
	 * @brief Resizes the allocator of children
	 * @param s Size.
	 */
	void resize(const size_t s)
	{
		m_children.resize(s);
	}

	/**
	 * @brief Clear the memory occupied by this internal node.
	 *
	 * And also clears the memory occupied by its subtrees.
	 */
	void clear() noexcept
	{
		for (auto& [_, c] : m_children) {
			c.clear();
		}
		m_children.clear();
		m_size = 0;
	}

	/// Iterator to the key-child pair container.
	[[nodiscard]] container_t::iterator begin() noexcept
	{
		return m_children.begin();
	}
	/// Iterator to the key-child pair container.
	[[nodiscard]] container_t::const_iterator begin() const noexcept
	{
		return m_children.begin();
	}
	/// Iterator to the key-child pair container.
	[[nodiscard]] container_t::iterator end() noexcept
	{
		return m_children.end();
	}
	/// Iterator to the key-child pair container.
	[[nodiscard]] container_t::const_iterator end() const noexcept
	{
		return m_children.end();
	}

	/**
	 * @brief Adds another element to this tree.
	 * @tparam unique Store the element when there are no repeats.
	 * @tparam _leaf_element_t Type of the element to be added.
	 * @tparam _key_t Type of the first key.
	 * @tparam _keys_t Type of the remaining keys.
	 * @param value Value to add.
	 * @param h The value of the first key.
	 * @param ks The values of the other keys.
	 * @returns True if the element was not found and added. False if otherwise.
	 */
	template <
		bool unique = true,
		typename _leaf_element_t = leaf_element_t,
		typename _key_t = key_t,
		typename... _keys_t>
	bool add(_leaf_element_t&& value, _key_t&& h, _keys_t&&...ks)
	{
		static_assert(check_types<_leaf_element_t, _key_t, _keys_t...>());

		const auto [i, exists] = search(m_children, h);
		if (not exists) {
			auto it = m_children.begin();
			std::advance(it, i);
			m_children.insert(it, subtree_t{std::move(h), child_t()});
			m_size += 1;
			// this always returns true
			return m_children[i].second.template add_empty<unique>(
				std::forward<leaf_element_t>(value),
				std::forward<_keys_t>(ks)...
			);
		}

		const bool added = m_children[i].second.template add<unique>(
			std::forward<leaf_element_t>(value), std::forward<_keys_t>(ks)...
		);
		m_size += added;
		return added;
	}

	/**
	 * @brief Adds another element to this tree.
	 *
	 * This method assumes that this node is empty.
	 * @tparam unique Store the element when there are no repeats.
	 * @tparam _leaf_element_t Type of the element to be added.
	 * @tparam _key_t Type of the first key.
	 * @tparam _keys_t Type of the remaining keys.
	 * @param v Value to add.
	 * @param h The value of the first key.
	 * @param ks The values of the other keys.
	 * @returns True if the element was not found and added. False if otherwise.
	 */
	template <
		bool unique = true,
		typename _leaf_element_t = leaf_element_t,
		typename _key_t = key_t,
		typename... _keys_t>
	bool add_empty(leaf_element_t&& value, _key_t&& h, _keys_t&&...ks)
	{
		static_assert(check_types<_leaf_element_t, _key_t, _keys_t...>());

		m_children.emplace_back(std::move(h), child_t());
		m_size += 1;
		// this always returns true
		return m_children[0].second.template add_empty<unique>(
			std::forward<leaf_element_t>(value), std::forward<_keys_t>(ks)...
		);
	}

	/**
	 * @brief Merges another tree into this tree.
	 * @tparam unique Store the elements of the new tree so that there are no repeats.
	 * @param t The tree to be merged into this tree.
	 * @returns The difference of the new size and the old size.
	 */
	template <bool unique = true>
	size_t merge(ctree<data_t, metadata_t, key_t, keys_t...>&& t)
	{
		size_t old_size = m_size;
		for (auto& [k, c] : t.m_children) {

			const auto [i, exists] = search(m_children, k);
			if (not exists) {
				auto it = m_children.begin();
				std::advance(it, i);
				m_size += c.size();
				m_children.insert(it, {std::move(k), std::move(c)});
			}
			else {
				m_size +=
					m_children[i].second.template merge<unique>(std::move(c));
			}
		}
		return m_size - old_size;
	}

	/**
	 * @brief Does this node have a specific key?
	 * @param key The key value to look for.
	 * @returns True if there is a child under the key value @e key.
	 */
	[[nodiscard]] bool has_key(const key_t& key) const noexcept
	{
		return std::find_if(
				   m_children.begin(),
				   m_children.end(),
				   [&key](const subtree_t& e) noexcept
				   {
					   return e.first == key;
				   }
			   ) != m_children.end();
	}

	/**
	 * @brief The number of unique elements over all leaves of this tree.
	 * @returns The number of unique elements over all leaves of this tree.
	 */
	[[nodiscard]] size_t size() const noexcept
	{
		return m_size;
	}
	/**
	 * @brief The number of keys in this node.
	 *
	 * This is equal to the number of children of this node.
	 * @returns The number of keys in this node.
	 */
	[[nodiscard]] size_t num_keys() const noexcept
	{
		return m_children.size();
	}
	/**
	 * @brief The number of bytes occupied by this leaf node.
	 *
	 * This value is calculated.
	 * @returns The number of bytes that this tree requires.
	 */
	[[nodiscard]] size_t num_bytes() const noexcept
	{
		size_t bytes = m_children.size() * sizeof(subtree_t);
		for (const auto& [_, child] : m_children) {
			bytes += child.num_bytes();
		}
		return bytes;
	}

	/**
	 * @brief Updates the size of the current node.
	 * @returns The resulting size.
	 */
	size_t update_size() noexcept
	{
		m_size = 0;
		for (auto& [_, child] : m_children) {
			m_size += child.update_size();
		}
		return m_size;
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
		const auto s = m_children.size();

		os << tab << "size: " << size() << '\n';
		os << tab << "keys: " << num_keys() << '\n';

		for (const auto& [i, value] : m_children | std::views::enumerate) {
			const auto& [v, child] = value;

			if (static_cast<size_t>(i) < s - 1) {
				os << tab << "├── " << v << '\n';
				child.print(os, print_leaves, tab + "│   ");
			}
			else {
				os << tab << "└── " << v << '\n';
				child.print(os, print_leaves, tab + "    ");
			}
		}
	}

	/**
	 * @brief Returns the sizes of all subtrees of this tree.
	 *
	 * The sizes are returned as proportion values (from 0 to 1).
	 * @returns A vector of sizes proportionally to the full size of this tree.
	 */
	[[nodiscard]] std::vector<size_t> sizes() const noexcept
	{
		std::vector<size_t> s(m_children.size(), 0);
		std::ranges::transform(
			m_children,
			s,
			[](const auto& key_child)
			{
				return key_child.second.size();
			}
		);
		return s;
	}

	/**
	 * @brief Returns the @e i-th key in this node.
	 * @param i A valid index. Must be less than @ref num_keys().
	 * @returns A constant reference to a key value.
	 */
	[[nodiscard]] const key_t& get_key(const size_t i) const noexcept
	{
#if defined DEBUG
		assert(i < m_children.size());
#endif
		auto it = m_children.begin();
		std::advance(it, i);
		return it->first;
	}

	/**
	 * @brief Returns the @e i-th child of this node.
	 * @param i A valid index. Must be less than @ref num_keys().
	 * @returns A non-constant reference to a key value.
	 */
	[[nodiscard]] child_t& get_child(const size_t i) noexcept
	{
#if defined DEBUG
		assert(i < m_children.size());
#endif
		auto it = m_children.begin();
		std::advance(it, i);
		return it->second;
	}

	/**
	 * @brief Returns the @e i-th child of this node.
	 * @param i A valid index. Must be less than @ref num_keys().
	 * @returns A constant reference to a key value.
	 */
	[[nodiscard]] const child_t& get_child(const size_t i) const noexcept
	{
#if defined DEBUG
		assert(i < m_children.size());
#endif
		auto it = m_children.begin();
		std::advance(it, i);
		return it->second;
	}

	// Returns an iterator object over the leaves of this tree.
	[[nodiscard]] iterator<data_t, metadata_t, key_t, keys_t...>
	get_iterator() noexcept
	{
		iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	[[nodiscard]] iterator<data_t, metadata_t, key_t, keys_t...>
	get_iterator_begin() noexcept
	{
		iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		it.to_begin();
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the end of the iteration.
	 */
	[[nodiscard]] iterator<data_t, metadata_t, key_t, keys_t...>
	get_iterator_end() noexcept
	{
		iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		it.to_end();
		return it;
	}
	// Returns an iterator object over the leaves of this tree.
	[[nodiscard]] const_iterator<data_t, metadata_t, key_t, keys_t...>
	get_const_iterator() const noexcept
	{
		const_iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	[[nodiscard]] const_iterator<data_t, metadata_t, key_t, keys_t...>
	get_const_iterator_begin() const noexcept
	{
		const_iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		it.to_begin();
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the end of the iteration.
	 */
	[[nodiscard]] const_iterator<data_t, metadata_t, key_t, keys_t...>
	get_const_iterator_end() const noexcept
	{
		const_iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		it.to_end();
		return it;
	}

	// Returns a range iterator object over the leaves of this tree.
	template <typename... Callables>
	[[nodiscard]] range_iterator<data_t, metadata_t, key_t, keys_t...>
	get_range_iterator(Callables&&...fs) noexcept
	{
		range_iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_functions(std::forward<Callables>(fs)...);
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns a range iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	template <typename... Callables>
	[[nodiscard]] range_iterator<data_t, metadata_t, key_t, keys_t...>
	get_range_iterator_begin(Callables&&...fs) noexcept
	{
		range_iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_functions(std::forward<Callables>(fs)...);
		it.set_pointer(this);
		[[maybe_unused]] const bool _ = it.to_begin();
		return it;
	}
	/**
	 * @brief Returns a range iterator object over the leaves of this tree.
	 *
	 * Starts at the end of the iteration.
	 */
	template <typename... Callables>
	[[nodiscard]] range_iterator<data_t, metadata_t, key_t, keys_t...>
	get_range_iterator_end(Callables&&...fs) noexcept
	{
		range_iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_functions(std::forward<Callables>(fs)...);
		it.set_pointer(this);
		[[maybe_unused]] const bool _ = it.to_end();
		return it;
	}
	// Returns a const range iterator object over the leaves of this tree.
	template <typename... Callables>
	[[nodiscard]] const_range_iterator<data_t, metadata_t, key_t, keys_t...>
	get_const_range_iterator(Callables&&...fs) const noexcept
	{
		const_range_iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_functions(std::forward<Callables>(fs)...);
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns a constant range iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	template <typename... Callables>
	[[nodiscard]] const_range_iterator<data_t, metadata_t, key_t, keys_t...>
	get_const_range_iterator_begin(Callables&&...fs) const noexcept
	{
		const_range_iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_functions(std::forward<Callables>(fs)...);
		it.set_pointer(this);
		[[maybe_unused]] const bool _ = it.to_begin();
		return it;
	}
	/**
	 * @brief Returns a constant range iterator object over the leaves of this tree.
	 *
	 * Starts at the end of the iteration.
	 */
	template <typename... Callables>
	[[nodiscard]] const_range_iterator<data_t, metadata_t, key_t, keys_t...>
	get_const_range_iterator_end(Callables&&...fs) const noexcept
	{
		const_range_iterator<data_t, metadata_t, key_t, keys_t...> it;
		it.set_functions(std::forward<Callables>(fs)...);
		it.set_pointer(this);
		[[maybe_unused]] const bool _ = it.to_end();
		return it;
	}

#if defined CTREE_DEBUG
	/// Ensure that the all keys in every node of this tree are sorted.
	[[nodiscard]] bool check_sorted_keys() const noexcept
	{
		const bool keys_sorted = std::is_sorted(
			m_children.begin(),
			m_children.end(),
			[](const subtree_t& e1, const subtree_t& e2) -> bool
			{
				return e1.first < e2.first;
			}
		);
		if (not keys_sorted) {
			return false;
		}
		for (const subtree_t& e : m_children) {
			if (not e.second.check_sorted_keys()) {
				return false;
			}
		}
		return true;
	}
#endif

private:

	/// The set of keys and their associated subtree of this tree's node.
	container_t m_children;
	/// The number of unique elements over all leaves of this tree.
	size_t m_size = 0;

private:

	/**
	 * @brief Ensure that the template parameters of this function are the
	 * same as those in the template parameters of this class.
	 *
	 * Constant and reference qualifiers are removed prior to comparing.
	 * @tparam _leaf_element_t Type of the keys.
	 * @tparam _keys_t Type of the key functions.
	 * @returns True if all the types are same. False if otherwise.
	 */
	template <typename _leaf_element_t, typename... _keys_t>
	[[nodiscard]] static consteval bool check_types() noexcept
	{
		return std::is_same_v<
				   std::remove_cvref_t<_leaf_element_t>,
				   leaf_element_t> and
			   check_packs(
				   parameter_pack<key_t, keys_t...>{},
				   parameter_pack<std::remove_cvref_t<_keys_t>...>{}
			   );
	}
};

} // namespace classtree
