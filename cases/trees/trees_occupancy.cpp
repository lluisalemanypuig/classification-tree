/**
 * Case study of the Isomorphic Representatives Tree
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
 *     Lluís Alemany Puig
 *     https://github.com/lluisalemanypuig
 */

// C++ includes
#include <iostream>
#include <cstring>
#include <iomanip>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/utilities/tree_isomorphism.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/detail/linarr/D/Dmin/Planar_AEF.hpp>
#include <lal/detail/linarr/D/DMax/Planar_AEF.hpp>

// isorepr includes
#include <isorepr/ir_tree.hpp>

inline std::ostream& operator<< (std::ostream& os, const std::vector<double>& v)
{
	for (const auto s : v) {
		os << s << ' ';
	}
	return os;
}

struct equal_comparable_tree {
	lal::graphs::free_tree tree;
	[[nodiscard]] bool operator== (const equal_comparable_tree& t
	) const noexcept
	{
		return lal::utilities::are_trees_isomorphic(tree, t.tree);
	}

	friend std::ostream&
	operator<< (std::ostream& os, const equal_comparable_tree& t) noexcept
	{
		const auto hv = t.tree.get_head_vector();
		os << hv[0];
		for (std::size_t i = 1; i < hv.size(); ++i) {
			os << ' ' << hv[i];
		}
		return os;
	}
};

struct metadata {
	uint64_t num_occs = 0;
	metadata& operator+= (const metadata& m) noexcept
	{
		num_occs = m.num_occs;
		return *this;
	}

	friend std::ostream&
	operator<< (std::ostream& os, const metadata& m) noexcept
	{
		os << '{' << m.num_occs << '}';
		return os;
	}
};

void occupancy_0(const uint64_t n, const uint64_t _N)
{
	isorepr::ir_tree<equal_comparable_tree, metadata> ir;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (std::size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		{
			ir.add({.tree = std::move(t)}, {.num_occs = 1});
		}

		if (N % step == 0) {
			if (N == 1000) {
				step = 1000;
			}
		}

#if defined IR_DEBUG
		if (not ir.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

void occupancy_1(const uint64_t n, const uint64_t _N)
{
	isorepr::ir_tree<equal_comparable_tree, metadata, uint64_t> ir;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (std::size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		{
			const auto Dmin_planar = lal::detail::Dmin::planar::AEF<false>(t);
			ir.add({.tree = std::move(t)}, {.num_occs = 1}, Dmin_planar);
		}

		if (N % step == 0) {
			std::cout << "--------------------\n";
			std::cout << ir.sizes() << '\n';
			if (N == 1000) {
				step = 1000;
			}
		}

#if defined IR_DEBUG
		if (not ir.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

void occupancy_2(const uint64_t n, const uint64_t _N)
{
	isorepr::ir_tree<equal_comparable_tree, metadata, uint64_t, double> ir;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (std::size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		{
			const auto Dmin_planar = lal::detail::Dmin::planar::AEF<false>(t);
			const auto C_exp = lal::properties::exp_num_crossings(t);
			ir.add({.tree = std::move(t)}, {.num_occs = 1}, Dmin_planar, C_exp);
		}

		if (N % step == 0) {
			std::cout << "--------------------\n";
			const auto sizes = ir.sizes();
			for (std::size_t i = 0; i < ir.num_keys(); ++i) {
				const auto& ir_i = ir.get_child(i);
				std::cout << std::setw(2) << i << ": (" << sizes[i] << ") -> "
						  << ir_i.sizes() << '\n';
			}
			if (N == 1000) {
				step = 1000;
			}
		}

#if defined IR_DEBUG
		if (not ir.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

void occupancy_3(const uint64_t n, const uint64_t _N)
{
	isorepr::ir_tree<equal_comparable_tree, metadata, uint64_t, double, double>
		ir;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (std::size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		{
			const auto Dmin_planar = lal::detail::Dmin::planar::AEF<false>(t);
			const auto C_exp = lal::properties::exp_num_crossings(t);
			const auto C_var = lal::properties::var_num_crossings_tree(t);
			ir.add(
				{.tree = std::move(t)},
				{.num_occs = 1},
				Dmin_planar,
				C_exp,
				C_var
			);
		}

		if (N % step == 0) {
			if (N == 1000) {
				step = 1000;
			}
		}

#if defined IR_DEBUG
		if (not ir.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}

	std::cout << "--------------------\n";
	const auto sizes = ir.sizes();
	for (std::size_t i = 0; i < ir.num_keys(); ++i) {
		const auto& ir_i = ir.get_child(i);
		std::cout << std::setw(2) << i << ": (" << sizes[i] << ") ->\n";

		const auto sizes_i = ir_i.sizes();
		for (std::size_t j = 0; j < ir_i.num_keys(); ++j) {
			const auto& ir_j = ir_i.get_child(j);
			std::cout << "    " << std::setw(2) << j << ": (" << sizes_i[j]
					  << ") -> " << ir_j.sizes() << '\n';
		}
	}
}

int main(int argc, char *argv[])
{
	std::cout.setf(std::ios::fixed);
	std::cout.precision(3);

	if (argc != 4) {
		std::cerr << "Exactly 3 arguments are needed.\n";
		std::cerr << "    d: depth of the tree (e.g.: 0, 1, 2, ...)\n";
		std::cerr << "    n: number of vertices\n";
		std::cerr << "    N: number of trees to generate\n";
		std::cerr << "Example:\n";
		std::cerr << "    ./trees_occupancy 100 10000\n";
		return 1;
	}

	const uint64_t d = static_cast<uint64_t>(atoi(argv[1]));
	const uint64_t n = static_cast<uint64_t>(atoi(argv[2]));
	const uint64_t N = static_cast<uint64_t>(atoi(argv[3]));

	std::cout << "n\tN\tir_type\ttime\tunique\n";
	if (d == 0) {
		std::cout << "====================\n";
		std::cout << "OCCUPANCY 0\n";
		occupancy_0(n, N);
	}
	else if (d == 1) {
		std::cout << "====================\n";
		std::cout << "OCCUPANCY 1\n";
		occupancy_1(n, N);
	}
	else if (d == 2) {
		std::cout << "====================\n";
		std::cout << "OCCUPANCY 2\n";
		occupancy_2(n, N);
	}
	else if (d == 3) {
		std::cout << "====================\n";
		std::cout << "OCCUPANCY 3\n";
		occupancy_3(n, N);
	}
}
