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
#include <lal/generate/all_ulab_free_trees.hpp>
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

template <typename T, typename Fn>
void distribution_1(const uint64_t n, const Fn& f)
{
	classtree::ctree<equal_comparable_tree, metadata, T> ctree;

	lal::generate::all_ulab_free_trees gen(n);
	while (not gen.end()) {
		lal::graphs::free_tree t = gen.yield_tree();

		ctree.template add<false>(
			{.tree = std::move(t)}, {.num_occs = 1}, f(t)
		);

#if defined CTREE_DEBUG
		if (not ctree.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}

	std::cout << ctree.sizes() << '\n';
}

template <typename T1, typename T2, typename Fn1, typename Fn2>
void distribution_2(const uint64_t n, const Fn1& f1, const Fn2& f2)
{
	classtree::ctree<equal_comparable_tree, metadata, T1, T2> ctree;

	lal::generate::all_ulab_free_trees gen(n);
	while (not gen.end()) {
		lal::graphs::free_tree t = gen.yield_tree();

		ctree.template add<false>(
			{.tree = std::move(t)}, {.num_occs = 1}, f1(t), f2(t)
		);

#if defined CTREE_DEBUG
		if (not ctree.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}

	for (size_t i = 0; i < ctree.num_keys(); ++i) {
		const auto& ctree_i = ctree.get_child(i);
		std::cout << ctree_i.sizes() << '\n';
	}
}

template <
	typename T1,
	typename T2,
	typename T3,
	typename Fn1,
	typename Fn2,
	typename Fn3>
void distribution_3(
	const uint64_t n, const Fn1& f1, const Fn2& f2, const Fn3& f3
)
{
	classtree::ctree<equal_comparable_tree, metadata, T1, T2, T3> ctree;

	lal::generate::all_ulab_free_trees gen(n);
	while (not gen.end()) {
		lal::graphs::free_tree t = gen.yield_tree();

		ctree.template add<false>(
			{.tree = std::move(t)}, {.num_occs = 1}, f1(t), f2(t), f3(t)
		);

#if defined CTREE_DEBUG
		if (not ctree.check_sorted_keys()) {
			std::cout << "ERROR!\n";
		}
#endif
	}

	const auto sizes = ctree.sizes();
	for (size_t i = 0; i < ctree.num_keys(); ++i) {
		const auto& ctree_i = ctree.get_child(i);
		std::cout << ctree_i.sizes() << ":\n";

		const auto sizes_i = ctree_i.sizes();
		for (size_t j = 0; j < ctree_i.num_keys(); ++j) {
			const auto& ctree_j = ctree_i.get_child(j);
			std::cout << "    " << ctree_j.sizes() << '\n';
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		std::cerr << "Exactly 2 arguments are needed.\n";
		std::cerr << "    n: number of vertices\n";
		std::cerr << "    t: test to profile\n";
		std::cerr << "        1_Dminpl\n";
		std::cerr << "        1_Cexp\n";
		std::cerr << "        1_Cvar\n";
		std::cerr << "        2_Dminpl_Cexp\n";
		std::cerr << "        2_Dminpl_Cvar\n";
		std::cerr << "        2_Cexp_Dminpl\n";
		std::cerr << "        2_Cvar_Dminpl\n";
		std::cerr << "        3_Dminpl_Cexp_Cvar\n";
		std::cerr << "Example:\n";
		std::cerr << "    ./trees_profiling 100 10000 2_Dminpl_Cvar\n";
		return 1;
	}

	const uint64_t n = static_cast<uint64_t>(atoi(argv[1]));
	const std::string_view t(argv[2]);

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

	if (t == "1_Dminpl") {
		distribution_1<uint64_t>(n, Dminpl);
	}
	else if (t == "1_Cexp") {
		distribution_1<double>(n, Cexp);
	}
	else if (t == "1_Cvar") {
		distribution_1<double>(n, Cvar);
	}
	else if (t == "2_Dminpl_Cexp") {
		distribution_2<uint64_t, double>(n, Dminpl, Cexp);
	}
	else if (t == "2_Dminpl_Cvar") {
		distribution_2<uint64_t, double>(n, Dminpl, Cvar);
	}
	else if (t == "2_Cexp_Dminpl") {
		distribution_2<double, uint64_t>(n, Cexp, Dminpl);
	}
	else if (t == "2_Cvar_Dminpl") {
		distribution_2<double, uint64_t>(n, Cvar, Dminpl);
	}
	else if (t == "3_Dminpl_Cexp_Cvar") {
		distribution_3<uint64_t, double, double>(n, Dminpl, Cexp, Cvar);
	}
}
