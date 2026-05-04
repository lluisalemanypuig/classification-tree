/**
 * Classification Tree
 * Copyright (C) 2025 - 2026  Lluís Alemany Puig
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

// This could have been implemented using a std::tuple.
template <typename... args>
struct parameter_pack { };

template <typename pack1, typename pack2>
constexpr bool are_packs_equal_v = false;

template <
	typename head1_t,
	typename... pack1,
	typename head2_t,
	typename... pack2>
constexpr bool are_packs_equal_v<
	parameter_pack<head1_t, pack1...>,
	parameter_pack<head2_t, pack2...>> =
	std::is_same_v<head1_t, head2_t> and
	are_packs_equal_v<parameter_pack<pack1...>, parameter_pack<pack2...>>;

template <typename head1_t, typename head2_t>
constexpr bool
	are_packs_equal_v<parameter_pack<head1_t>, parameter_pack<head2_t>> =
		std::is_same_v<head1_t, head2_t>;

} // namespace classtree
