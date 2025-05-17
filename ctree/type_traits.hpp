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

// This could have been implemented using a std::tuple.
template <typename... args>
struct parameter_pack { };

template <
	typename head1_t,
	typename... args1,
	typename head2_t,
	typename... args2>
static constexpr inline void
check_packs(const parameter_pack<head1_t, args1...>&, const parameter_pack<head2_t, args2...>&)
	noexcept
{
	static_assert(std::is_same_v<head1_t, head2_t>);
	if constexpr (sizeof...(args1) > 0) {
		check_packs(parameter_pack<args1...>{}, parameter_pack<args2...>{});
	}
}

} // namespace isorepr
