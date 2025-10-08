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

// ctree includes
#include <ctree/ctree.hpp>

template <typename T>
inline std::ostream& operator<< (std::ostream& os, const std::vector<T>& v)
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
		for (size_t i = 1; i < hv.size(); ++i) {
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
	classtree::ctree<equal_comparable_tree, metadata> ctree;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		{
			ctree.add({.tree = std::move(t)}, {.num_occs = 1});
		}

		if (N % step == 0) {
			if (N == 1000) {
				step = 1000;
			}
		}

#if defined CTREE_DEBUG
		if (not ctree.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

template <typename T, typename Fn>
void occupancy_1(const uint64_t n, const uint64_t _N, const Fn& f)
{
	classtree::ctree<equal_comparable_tree, metadata, T> ctree;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		ctree.add({.tree = std::move(t)}, {.num_occs = 1}, f(t));

		if (N % step == 0) {
			std::cout << "--------------------\n";
			std::cout << ctree.sizes() << '\n';
			if (N == 1000) {
				step = 1000;
			}
		}

#if defined CTREE_DEBUG
		if (not ctree.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

template <typename T1, typename T2, typename Fn1, typename Fn2>
void occupancy_2(
	const uint64_t n, const uint64_t _N, const Fn1& f1, const Fn2& f2
)
{
	classtree::ctree<equal_comparable_tree, metadata, T1, T2> ctree;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		ctree.add({.tree = std::move(t)}, {.num_occs = 1}, f1(t), f2(t));

		if (N % step == 0) {
			std::cout << "--------------------\n";
			const auto sizes = ctree.sizes();
			for (size_t i = 0; i < ctree.num_keys(); ++i) {
				const auto& ctree_i = ctree.get_child(i);
				std::cout << std::setw(2) << i << ": (" << sizes[i] << ") -> "
						  << ctree_i.sizes() << '\n';
			}
			if (N == 1000) {
				step = 1000;
			}
		}

#if defined CTREE_DEBUG
		if (not ctree.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}
}

template <
	typename T1,
	typename T2,
	typename T3,
	typename Fn1,
	typename Fn2,
	typename Fn3>
void occupancy_3(
	const uint64_t n,
	const uint64_t _N,
	const Fn1& f1,
	const Fn2& f2,
	const Fn3& f3
)
{
	classtree::ctree<equal_comparable_tree, metadata, T1, T2, T3> ctree;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		ctree.add({.tree = std::move(t)}, {.num_occs = 1}, f1(t), f2(t), f3(t));

		if (N % step == 0) {
			if (N == 1000) {
				step = 1000;
			}
		}

#if defined CTREE_DEBUG
		if (not ctree.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}

	std::cout << "--------------------\n";
	const auto sizes = ctree.sizes();
	for (size_t i = 0; i < ctree.num_keys(); ++i) {
		const auto& ctree_i = ctree.get_child(i);
		std::cout << std::setw(2) << i << ": (" << sizes[i] << ") ->\n";

		const auto sizes_i = ctree_i.sizes();
		for (size_t j = 0; j < ctree_i.num_keys(); ++j) {
			const auto& ctree_j = ctree_i.get_child(j);
			std::cout << "    " << std::setw(2) << j << ": (" << sizes_i[j]
					  << ") -> " << ctree_j.sizes() << '\n';
		}
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
		std::cerr << "        1_Cexp\n";
		std::cerr << "        1_Cvar\n";
		std::cerr << "        2_Dminpl_Cexp\n";
		std::cerr << "        2_Dminpl_Cvar\n";
		std::cerr << "        3_Dminpl_Cexp_Cvar\n";
		std::cerr << "Example:\n";
		std::cerr << "    ./trees_profiling 100 10000 2_Dminpl_Cvar\n";
		return 1;
	}

	const uint64_t n = static_cast<uint64_t>(atoi(argv[1]));
	const size_t N = static_cast<size_t>(atoi(argv[2]));
	const std::string_view t(argv[3]);

	const auto Dminpl = [](const lal::graphs::free_tree& T) -> uint64_t
	{
		return lal::detail::Dmin::planar::AEF<false>(T);
	};
	const auto Cexp = [](const lal::graphs::free_tree& T) -> double
	{
		return lal::properties::exp_num_crossings(T);
	};
	const auto Cvar = [](const lal::graphs::free_tree& T) -> double
	{
		return lal::properties::var_num_crossings(T);
	};

	if (t == "0") {
		occupancy_0(n, N);
	}
	else if (t == "1_Dminpl") {
		occupancy_1<uint64_t>(n, N, Dminpl);
	}
	else if (t == "1_Cexp") {
		occupancy_1<double>(n, N, Cexp);
	}
	else if (t == "1_Cvar") {
		occupancy_1<double>(n, N, Cvar);
	}
	else if (t == "2_Dminpl_Cexp") {
		occupancy_2<uint64_t, double>(n, N, Dminpl, Cexp);
	}
	else if (t == "2_Dminpl_Cvar") {
		occupancy_2<uint64_t, double>(n, N, Dminpl, Cvar);
	}
	else if (t == "3_Dminpl_Cexp_Cvar") {
		occupancy_3<uint64_t, double, double>(n, N, Dminpl, Cexp, Cvar);
	}
}
