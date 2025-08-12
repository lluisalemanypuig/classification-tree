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
 * 		Lluís Alemany Puig
 * 		https://github.com/lluisalemanypuig
 */

// C++ includes
#include <iostream>
#include <cstring>
#include <chrono>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/utilities/tree_isomorphism.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/detail/linarr/D/Dmin/Planar_AEF.hpp>
#include <lal/detail/linarr/D/DMax/Planar_AEF.hpp>

// ctree includes
#include <ctree/ctree.hpp>

typedef std::chrono::high_resolution_clock::time_point time_point;

inline time_point now()
{
	return std::chrono::high_resolution_clock::now();
}

inline double elapsed_time(const time_point& begin, const time_point& end)
{
	return std::chrono::duration<double, std::chrono::milliseconds::period>(
			   end - begin
	)
		.count();
}

bool is_power_10(const std::size_t N) noexcept
{
	std::size_t p10 = 1;
	std::size_t n = N;
	while (n > 9) {
		p10 *= 10;
		n /= 10;
	}
	return p10 == N;
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

void profiling_0(const uint64_t n, const uint64_t _N)
{
	classtree::ctree<equal_comparable_tree, metadata> ir;

	double total_time = 0;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (std::size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		const auto begin = now();
		{
			ir.add({.tree = std::move(t)}, {.num_occs = 1});
		}
		const auto end = now();
		total_time += elapsed_time(begin, end);

		if (N % step == 0) [[unlikely]] {
			std::cout << n << '\t' << N << '\t' << "0" << '\t';
			std::cout << total_time << '\t' << ir.size() << std::endl;
			total_time = 0;

			if (is_power_10(N)) {
				step = N;
			}
		}

#if defined CTREE_DEBUG
		if (not ir.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

void profiling_1_Dminpl(const uint64_t n, const uint64_t _N)
{
	classtree::ctree<equal_comparable_tree, metadata, uint64_t> ir;

	double total_time = 0;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (std::size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		const auto begin = now();
		{
			const auto Dmin_planar = lal::detail::Dmin::planar::AEF<false>(t);
			ir.add({.tree = std::move(t)}, {.num_occs = 1}, Dmin_planar);
		}
		const auto end = now();
		total_time += elapsed_time(begin, end);

		if (N % step == 0) [[unlikely]] {
			std::cout << n << '\t' << N << '\t' << "1_Dminpl" << '\t';
			std::cout << total_time << '\t' << ir.size() << std::endl;
			total_time = 0;

			if (is_power_10(N)) {
				step = N;
			}
		}

#if defined CTREE_DEBUG
		if (not ir.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

void profiling_2_Dminpl_Cexp(const uint64_t n, const uint64_t _N)
{
	classtree::ctree<equal_comparable_tree, metadata, uint64_t, double> ir;

	double total_time = 0;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (std::size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		const auto begin = now();
		{
			const auto Dmin_planar = lal::detail::Dmin::planar::AEF<false>(t);
			const auto C_exp = lal::properties::exp_num_crossings(t);
			ir.add({.tree = std::move(t)}, {.num_occs = 1}, Dmin_planar, C_exp);
		}
		const auto end = now();
		total_time += elapsed_time(begin, end);

		if (N % step == 0) [[unlikely]] {
			std::cout << n << '\t' << N << '\t' << "2_Dminpl_Cexp" << '\t';
			std::cout << total_time << '\t' << ir.size() << std::endl;
			total_time = 0;

			if (is_power_10(N)) {
				step = N;
			}
		}

#if defined CTREE_DEBUG
		if (not ir.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

void profiling_2_Dminpl_Cvar(const uint64_t n, const uint64_t _N)
{
	classtree::ctree<equal_comparable_tree, metadata, uint64_t, double> ir;

	double total_time = 0;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (std::size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		const auto begin = now();
		{
			const auto Dmin_planar = lal::detail::Dmin::planar::AEF<false>(t);
			const auto C_var = lal::properties::var_num_crossings_tree(t);
			ir.add({.tree = std::move(t)}, {.num_occs = 1}, Dmin_planar, C_var);
		}
		const auto end = now();
		total_time += elapsed_time(begin, end);

		if (N % step == 0) [[unlikely]] {
			std::cout << n << '\t' << N << '\t' << "2_Dminpl_Cvar" << '\t';
			std::cout << total_time << '\t' << ir.size() << std::endl;
			total_time = 0;

			if (is_power_10(N)) {
				step = N;
			}
		}

#if defined CTREE_DEBUG
		if (not ir.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

void profiling_3_Dminpl_Cexp_Cvar(const uint64_t n, const uint64_t _N)
{
	classtree::ctree<equal_comparable_tree, metadata, uint64_t, double, double>
		ir;

	double total_time = 0;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (std::size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		const auto begin = now();
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
		const auto end = now();
		total_time += elapsed_time(begin, end);

		if (N % step == 0) [[unlikely]] {
			std::cout << n << '\t' << N << '\t' << "3_Dminpl_Cexp_Cvar" << '\t';
			std::cout << total_time << '\t' << ir.size() << std::endl;
			total_time = 0;

			if (is_power_10(N)) {
				step = N;
			}
		}

#if defined CTREE_DEBUG
		if (not ir.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

int main(int argc, char *argv[])
{
	if (argc != 4) {
		std::cerr << "Exactly 3 arguments are needed.\n";
		std::cerr << "    n: number of vertices\n";
		std::cerr << "    N: number of trees to generate\n";
		std::cerr << "    t: test to profile\n";
		std::cerr << "        0\n";
		std::cerr << "        1_Dminpl\n";
		std::cerr << "        2_Dminpl_Cexp\n";
		std::cerr << "        2_Dminpl_Cvar\n";
		std::cerr << "        3_Dminpl_Cexp_Cvar\n";
		std::cerr << "Example:\n";
		std::cerr << "    ./trees_profiling 100 10000 2_Dminpl_Cvar\n";
		return 1;
	}

	const uint64_t n = static_cast<uint64_t>(atoi(argv[1]));
	const std::size_t N = static_cast<std::size_t>(atoi(argv[2]));
	const std::string t(argv[3]);

	std::cout << "n\tN\tir_type\ttime\tunique\n";

	if (t == "0") {
		profiling_0(n, N);
	}
	else if (t == "1_Dminpl") {
		profiling_1_Dminpl(n, N);
	}
	else if (t == "2_Dminpl_Cexp") {
		profiling_2_Dminpl_Cexp(n, N);
	}
	else if (t == "2_Dminpl_Cvar") {
		profiling_2_Dminpl_Cvar(n, N);
	}
	else if (t == "3_Dminpl_Cexp_Cvar") {
		profiling_3_Dminpl_Cexp_Cvar(n, N);
	}
}
