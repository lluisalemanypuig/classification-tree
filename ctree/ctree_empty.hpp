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
#include <ostream>
#include <vector>

// custom includes
#include <ctree/ctree.hpp>

namespace classtree {

/**
 * @brief Partial template specialization of the Classification Tree.
 * 
 * This class has no subtrees and is implemented simply as an array of
 * pairs of value and its metadata (see @ref element_t).
 * @tparam value_t Type of the values to add.
 * @tparam metadata_t Type of the metadata associated to the values.
 */
template <EqualityComparable value_t, Incrementable metadata_t>
class ctree<value_t, metadata_t> {
public:

	/// A pair of representative element and its metadata.
	typedef std::pair<value_t, metadata_t> element_t;

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
	 * @param v Value to add.
	 * @param m Metadata associated to the value.
	 * @returns True if the element was not found and added. False if otherwise.
	 */
	template <
		bool unique = true,
		typename _value_t = value_t,
		typename _metadata_t = metadata_t>
	bool add(_value_t&& val, _metadata_t&& meta)
	{
		static_assert(check_types<_value_t, _metadata_t>());

		if constexpr (unique) {
			for (auto& [v, m] : m_children) {
				if (val == v) {
					m += meta;
					return false;
				}
			}
		}

		m_children.emplace_back(std::move(val), std::move(meta));
		return true;
	}

	/**
	 * @brief The number of unique elements over all leaves of this tree.
	 * @returns The number of unique elements over all leaves of this tree.
	 */
	[[nodiscard]] std::size_t size() const noexcept
	{
		return m_children.size();
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
	 * @brief Returns the @e i-th child of this node.
	 * @param i A valid index. Must be less than @ref num_keys().
	 * @returns A non-constant reference to a key value.
	 */
	std::pair<value_t, metadata_t>& get_child(const std::size_t i) noexcept
	{
#if defined DEBUG
		assert(i < m_children.size());
#endif
		return m_children[i];
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
		os << tab << "^ size: " << size() << ' ' << num_keys() << '\n';
		if (print_leaves) {
			for (std::size_t i = 0; i < m_children.size(); ++i) {
				const auto& [v, meta] = m_children[i];

				if (i < m_children.size() - 1) {
					os << tab << "├── ";
				}
				else {
					os << tab << "└── ";
				}

				os << v << ' ' << meta << '\n';
			}
		}
	}

	/// Returns an iterator object over the leaves of this tree.
	[[nodiscard]] iterator<value_t, metadata_t> get_iterator() noexcept
	{
		iterator<value_t, metadata_t> it;
		it.set_pointer(this);
		it.at_begin();
		return it;
	}
	/// Returns a constant iterator object over the leaves of this tree.
	[[nodiscard]] const_iterator<value_t, metadata_t>
	get_const_iterator() const noexcept
	{
		const_iterator<value_t, metadata_t> it;
		it.set_pointer(this);
		it.at_begin();
		return it;
	}

	/// Returns a range iterator object over the leaves of this tree.
	[[nodiscard]] range_iterator<value_t, metadata_t>
	get_range_iterator() noexcept
	{
		range_iterator<value_t, metadata_t> it;
		it.set_pointer(this);
		it.at_begin();
		return it;
	}

	/// Returns a range iterator object over the leaves of this tree.
	[[nodiscard]] const_range_iterator<value_t, metadata_t>
	get_const_range_iterator() const noexcept
	{
		const_range_iterator<value_t, metadata_t> it;
		it.set_pointer(this);
		it.at_begin();
		return it;
	}

#if defined IR_DEBUG
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
	 * @tparam _value_t Type of the keys.
	 * @tparam _metadata_t Type of the metadata.
	 * @returns True if all the types are same. False if otherwise.
	 */
	template <typename _value_t, typename _metadata_t>
	[[nodiscard]] static constexpr bool check_types() noexcept
	{
		return std::is_same_v<std::remove_cvref_t<_value_t>, value_t> and
			   std::is_same_v<std::remove_cvref_t<_metadata_t>, metadata_t>;
	}

private:

	/// The unique elements in this tree.
	container_t m_children;
};

} // namespace ctree
