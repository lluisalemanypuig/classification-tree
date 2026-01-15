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
#include <fstream>

// ctree includes
#include <ctree/type_traits.hpp>
#include <ctree/ctree.hpp>

namespace classtree {
namespace detail {

/**
 * @brief Writes to an output stream the internal memory profile of this tree.
 *
 * The internal memory profile is the memory information per node of the tree
 * in a pre-order iteration of the tree. For each internal node, this function
 * outputs: the number of keys and their value. For each leaf node, outputs the
 * number of elements in the leaf.
 * @tparam output_t Type of the output stream.
 * @tparam data_t Type of the data stored in the leaves of the tree.
 * @tparam metadata_t Type of the metadata stored in the leaves of the tree.
 * @tparam Ts Types of the keys of the tree.
 * @param t The tree whose memory profile is to be written to @e fout.
 * @param fout The output stream.
 */
template <
	typename output_t,
	typename data_t,
	typename metadata_t,
	Comparable... Ts>
void output_profile(const ctree<data_t, metadata_t, Ts...>& t, output_t& fout)
{
	if constexpr (sizeof...(Ts) == 0) {
		fout << t.size() << ' ';
	}
	else {
		const size_t k = t.num_keys();
		fout << k;
		for (size_t i = 0; i < k; ++i) {
			fout << ' ' << t.get_key(i);
		}
		if (k > 0) {
			fout << ' ';
			for (size_t i = 0; i < k; ++i) {
				detail::output_profile(t.get_child(i), fout);
			}
		}
	}
}

} // namespace detail

/**
 * @brief Writes to an output stream the memory profile of this tree.
 *
 * The memory profile is the total number of bytes of the tree plus the internal
 * memory profile of the tree (see @ref detail::output_profile).
 * @tparam output_t Type of the output stream.
 * @tparam data_t Type of the data stored in the leaves of the tree.
 * @tparam metadata_t Type of the metadata stored in the leaves of the tree.
 * @tparam Ts Types of the keys of the tree.
 * @param t The tree whose memory profile is to be written to @e fout.
 * @param fout The output stream.
 */
template <
	typename output_t,
	typename data_t,
	typename metadata_t,
	Comparable... Ts>
void output_profile(const ctree<data_t, metadata_t, Ts...>& t, output_t& fout)
{
	fout << t.num_bytes() << ' ';
	detail::output_profile(t, fout);
}

/**
 * @brief Writes to a file the memory profile of this tree.
 *
 * The memory profile is the total number of bytes of the tree plus the internal
 * memory profile of the tree (see @ref detail::output_profile).
 * @tparam data_t Type of the data stored in the leaves of the tree.
 * @tparam metadata_t Type of the metadata stored in the leaves of the tree.
 * @tparam Ts Types of the keys of the tree.
 * @param t The tree whose memory profile is to be written to @e fout.
 * @param filename The name of the file to write to.
 */
template <typename data_t, typename metadata_t, Comparable... Ts>
[[nodiscard]] bool output_profile(
	const ctree<data_t, metadata_t, Ts...>& t, const std::string& filename
)
{
	std::ofstream fout(filename);
	if (not fout.is_open()) {
		return false;
	}

	output_profile(t, fout);
	return true;
}

} // namespace classtree
