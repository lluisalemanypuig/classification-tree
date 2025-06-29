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
#include <algorithm>
#include <vector>

// custom includes
#include <ctree/search.hpp>
#include <ctree/type_traits.hpp>
#include <ctree/ctree.hpp>

namespace classtree {

/**
 * @brief Partial template specialization of the Classification Tree.
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
class ctree<value_t, metadata_t, key_t, keys_t...> {
public:

	/// Type of the children nodes.
	using child_t = ctree<value_t, metadata_t, keys_t...>;

	/// Node type, a pair of key value and its associated subtree.
	typedef std::pair<key_t, child_t> element_t;

	/// The container that stores the key values, and the associated subtree.
	typedef std::vector<element_t> container_t;

public:

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
	 * @tparam _value_t Type of the value to add.
	 * @tparam _metadata_t Type of the metadata associated to this value.
	 * @tparam _key_t Type of the first key.
	 * @tparam _keys_t Type of the remaining keys.
	 * @param v Value to add.
	 * @param m Metadata associated to the value.
	 * @param h The value of the first key.
	 * @param ks The values of the other keys.
	 * @returns True if the element was not found and added. False if otherwise.
	 */
	template <
		bool unique = true,
		typename _value_t = value_t,
		typename _metadata_t = metadata_t,
		typename _key_t = key_t,
		typename... _keys_t>
	bool add(_value_t&& v, _metadata_t&& m, _key_t&& h, _keys_t&&...ks)
	{
		check_types<_value_t, _metadata_t, _key_t, _keys_t...>();

		const auto [i, exists] = search(m_children, h);
		if (not exists) {
			auto it = m_children.begin();
			std::advance(it, i);
			m_children.insert(it, {std::move(h), child_t()});
			m_size += 1;
			// this always returns true
			return m_children[i].second.template add<unique>(
				v, m, std::forward<_keys_t>(ks)...
			);
		}

		const bool added = m_children[i].second.template add<unique>(
			v, m, std::forward<_keys_t>(ks)...
		);
		m_size += added;
		return added;
	}

	/**
	 * @brief The number of unique elements over all leaves of this tree.
	 * @returns The number of unique elements over all leaves of this tree.
	 */
	[[nodiscard]] std::size_t size() const noexcept
	{
		return m_size;
	}
	/**
	 * @brief The number of keys in this node.
	 *
	 * This is equal to the number of children of this node.
	 * @returns The number of keys in this node.
	 */
	[[nodiscard]] std::size_t num_keys() const noexcept
	{
		return m_children.size();
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
		os << tab << "size: " << size() << '\n';
		os << tab << "keys: " << num_keys() << '\n';

		std::size_t i = 0;
		for (const auto& [v, child] : m_children) {

			if (i < m_children.size() - 1) {
				os << tab << "├── " << v << '\n';
				child.print(os, print_leaves, tab + "│   ");
			}
			else {
				os << tab << "└── " << v << '\n';
				child.print(os, print_leaves, tab + "    ");
			}

			++i;
		}
	}

	/**
	 * @brief Returns the sizes of all subtrees of this tree.
	 *
	 * The sizes are returned as proportion values (from 0 to 1).
	 * @returns A vector of sizes proportionally to the full size of this tree.
	 */
	[[nodiscard]] std::vector<double> sizes() const noexcept
	{
		std::vector<double> s(m_children.size(), 0);

		std::size_t i = 0;
		for (const auto& [_, child] : m_children) {
			const double S = static_cast<double>(child.size());
			s[i] = S / static_cast<double>(m_size);

			++i;
		}
		return s;
	}

	/**
	 * @brief Returns the @e i-th key in this node.
	 * @param i A valid index. Must be less than @ref num_keys().
	 * @returns A constant reference to a key value.
	 */
	const key_t& get_key(const std::size_t i) const noexcept
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
	child_t& get_child(const std::size_t i) noexcept
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
	const child_t& get_child(const std::size_t i) const noexcept
	{
#if defined DEBUG
		assert(i < m_children.size());
#endif
		auto it = m_children.begin();
		std::advance(it, i);
		return it->second;
	}

	// Returns an iterator object over the leaves of this tree.
	[[nodiscard]] iterator<value_t, metadata_t, key_t, keys_t...>
	get_iterator() noexcept
	{
		iterator<value_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	[[nodiscard]] iterator<value_t, metadata_t, key_t, keys_t...>
	get_iterator_begin() noexcept
	{
		iterator<value_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		it.to_begin();
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the end of the iteration.
	 */
	[[nodiscard]] iterator<value_t, metadata_t, key_t, keys_t...>
	get_iterator_end() noexcept
	{
		iterator<value_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		it.to_end();
		return it;
	}
	// Returns an iterator object over the leaves of this tree.
	[[nodiscard]] const_iterator<value_t, metadata_t, key_t, keys_t...>
	get_const_iterator() const noexcept
	{
		const_iterator<value_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the beginning of the iteration.
	 */
	[[nodiscard]] const_iterator<value_t, metadata_t, key_t, keys_t...>
	get_const_iterator_begin() const noexcept
	{
		const_iterator<value_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		it.to_begin();
		return it;
	}
	/**
	 * @brief Returns an iterator object over the leaves of this tree.
	 *
	 * Starts at the end of the iteration.
	 */
	[[nodiscard]] const_iterator<value_t, metadata_t, key_t, keys_t...>
	get_const_iterator_end() const noexcept
	{
		const_iterator<value_t, metadata_t, key_t, keys_t...> it;
		it.set_pointer(this);
		it.to_end();
		return it;
	}

	// Returns a range iterator object over the leaves of this tree.
	template <typename... Callables>
	[[nodiscard]] range_iterator<value_t, metadata_t, key_t, keys_t...>
	get_range_iterator(Callables&&...fs) noexcept
	{
		range_iterator<value_t, metadata_t, key_t, keys_t...> it;
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
	[[nodiscard]] range_iterator<value_t, metadata_t, key_t, keys_t...>
	get_range_iterator_begin(Callables&&...fs) noexcept
	{
		range_iterator<value_t, metadata_t, key_t, keys_t...> it;
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
	[[nodiscard]] range_iterator<value_t, metadata_t, key_t, keys_t...>
	get_range_iterator_end(Callables&&...fs) noexcept
	{
		range_iterator<value_t, metadata_t, key_t, keys_t...> it;
		it.set_functions(std::forward<Callables>(fs)...);
		it.set_pointer(this);
		[[maybe_unused]] const bool _ = it.to_end();
		return it;
	}
	// Returns a const range iterator object over the leaves of this tree.
	template <typename... Callables>
	[[nodiscard]] const_range_iterator<value_t, metadata_t, key_t, keys_t...>
	get_const_range_iterator(Callables&&...fs) const noexcept
	{
		const_range_iterator<value_t, metadata_t, key_t, keys_t...> it;
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
	[[nodiscard]] const_range_iterator<value_t, metadata_t, key_t, keys_t...>
	get_const_range_iterator_begin(Callables&&...fs) const noexcept
	{
		const_range_iterator<value_t, metadata_t, key_t, keys_t...> it;
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
	[[nodiscard]] const_range_iterator<value_t, metadata_t, key_t, keys_t...>
	get_const_range_iterator_end(Callables&&...fs) const noexcept
	{
		const_range_iterator<value_t, metadata_t, key_t, keys_t...> it;
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
			[](const element_t& e1, const element_t& e2) -> bool
			{
				return e1.first < e2.first;
			}
		);
		if (not keys_sorted) {
			return false;
		}
		for (const element_t& e : m_children) {
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
	std::size_t m_size = 0;

private:

	/**
	 * @brief Ensure that the template parameters of this function are the
	 * same as those in the template parameters of this class.
	 *
	 * Constant and reference qualifiers are removed prior to comparing.
	 * @tparam _value_t Type of the keys.
	 * @tparam _metadata_t Type of the metadata.
	 * @tparam _keys_t Type of the key functions.
	 * @returns True if all the types are same. False if otherwise.
	 */
	template <typename _value_t, typename _metadata_t, typename... _keys_t>
	static constexpr void check_types() noexcept
	{
		static_assert(std::is_same_v<std::remove_cvref_t<_value_t>, value_t>);
		static_assert(std::is_same_v<
					  std::remove_cvref_t<_metadata_t>,
					  metadata_t>);
		check_packs(
			parameter_pack<key_t, keys_t...>{},
			parameter_pack<std::remove_cvref_t<_keys_t>...>{}
		);
	}
};

} // namespace classtree
