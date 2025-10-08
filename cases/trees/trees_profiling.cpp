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

bool is_power_10(const size_t N) noexcept
{
	size_t p10 = 1;
	size_t n = N;
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

template <typename... TreeTypes, typename... Funcs>
void profiling(
	const uint64_t n,
	const uint64_t _N,
	const std::string_view& name,
	Funcs&&...fs
)
{
	classtree::ctree<equal_comparable_tree, metadata, TreeTypes...> ctree;

	double total_time = 0;

	uint64_t step = 100;
	lal::generate::rand_ulab_free_trees gen(n, 1234);
	for (size_t N = 1; N <= _N; ++N) {
		lal::graphs::free_tree t = gen.yield_tree();

		const auto begin = now();
		{
			ctree.add({.tree = std::move(t)}, {.num_occs = 1}, fs(t)...);
		}
		const auto end = now();
		total_time += elapsed_time(begin, end);

		if (N % step == 0) [[unlikely]] {
			std::cout << n << '\t' << N << '\t' << name << '\t';
			std::cout << total_time << '\t' << ctree.size() << '\n';
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
	const size_t N = static_cast<size_t>(atoi(argv[2]));
	const std::string_view t(argv[3]);

	std::cout << "n\tN\tfeature_type\ttime\tunique\n";

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
		profiling<>(n, N, t);
	}
	else if (t == "1_Dminpl") {
		profiling<uint64_t>(n, N, t, Dminpl);
	}
	else if (t == "1_Cexp") {
		profiling<double>(n, N, t, Cexp);
	}
	else if (t == "1_Cvar") {
		profiling<double>(n, N, t, Cvar);
	}
	else if (t == "2_Dminpl_Cexp") {
		profiling<uint64_t, double>(n, N, t, Dminpl, Cexp);
	}
	else if (t == "2_Dminpl_Cvar") {
		profiling<uint64_t, double>(n, N, t, Dminpl, Cvar);
	}
	else if (t == "2_Cexp_Dminpl") {
		profiling<double, uint64_t>(n, N, t, Cexp, Dminpl);
	}
	else if (t == "2_Cvar_Dminpl") {
		profiling<double, uint64_t>(n, N, t, Cvar, Dminpl);
	}
	else if (t == "3_Dminpl_Cexp_Cvar") {
		profiling<uint64_t, double, double>(n, N, t, Dminpl, Cexp, Cvar);
	}
}
