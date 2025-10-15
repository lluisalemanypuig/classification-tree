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
#include <type_traits>

namespace classtree {

/**
 * @brief Custom pair struct.
 * @tparam data_t Type of the data contained.
 * @tparam metadata_t Type of the metadata associated to the data.
 */
template <typename data_t, typename metadata_t>
struct pair {
	/// Data contained in the struct.
	data_t data;
	/// Metadata contained in the struct.
	metadata_t metadata;
};

/**
 * @brief The type of the value to be passed and used to actually store data.
 *
 * If the @ref metadata_t is void then the element is simply the @ref data_t type.
 * @tparam data_t Type of the data contained.
 * @tparam metadata_t Type of the metadata associated to the data.
 */
template <typename data_t, typename metadata_t>
using element_t = std::
	conditional_t<std::is_void_v<metadata_t>, data_t, pair<data_t, metadata_t>>;

/**
 * @brief Is @ref element_t a compound type of data and metadata?
 * @tparam data_t Type of the data contained.
 * @tparam metadata_t Type of the metadata associated to the data.
 */
template <typename data_t, typename metadata_t>
concept Compound = not std::is_void_v<metadata_t>;

/**
 * @brief Is @ref element_t a compound type of data and metadata?
 * @tparam data_t Type of the data contained.
 * @tparam metadata_t Type of the metadata associated to the data.
 */
template <typename data_t, typename metadata_t>
concept NotCompound = not Compound<data_t, metadata_t>;

} // namespace classtree
