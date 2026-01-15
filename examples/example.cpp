/**
 * Usage examples of the Isomorphic Representatives Tree
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

// isorepr includes
#include <ctree/ctree.hpp>
#include <ctree/iterator.hpp>
#include <ctree/range_iterator.hpp>
#include <ctree/memory_profile.hpp>

struct A {
	int i, j;
	[[nodiscard]] bool operator== (const A& o) const noexcept
	{
		return i == o.i and j == o.j;
	}

	inline friend std::ostream&
	operator<< (std::ostream& os, const A& c) noexcept
	{
		os << '(' << c.i << ' ' << c.j << ')';
		return os;
	}
};

struct B {
	int num_occs = 0;

	B& operator+= (const B& m) noexcept
	{
		num_occs += m.num_occs;
		return *this;
	}

	inline friend std::ostream&
	operator<< (std::ostream& os, const B& c) noexcept
	{
		os << '{' << c.num_occs << '}';
		return os;
	}
};

int main()
{
	classtree::ctree<A, B, int, double, std::string> kd;

	using namespace std::literals::string_literals;
	kd.add({{.i = 1, .j = 1}, {.num_occs = 1}}, 1, 0.5, "a"s);
	kd.add({{.i = 1, .j = 2}, {.num_occs = 1}}, 1, 0.5, "a"s);
	kd.add({{.i = 1, .j = 3}, {.num_occs = 1}}, 1, 1.5, "b"s);
	kd.add({{.i = 1, .j = 4}, {.num_occs = 1}}, 2, 0.5, "a"s);
	kd.add({{.i = 1, .j = 5}, {.num_occs = 1}}, 2, 0.5, "b"s);
	kd.add({{.i = 1, .j = 6}, {.num_occs = 1}}, 2, 0.5, "b"s);
	kd.add({{.i = 1, .j = 7}, {.num_occs = 1}}, 3, 0.5, "a"s);
	kd.add({{.i = 1, .j = 8}, {.num_occs = 1}}, 3, 1.5, "c"s);

	kd.print(std::cout);

	{
		std::cout << "Full iterate (key-value):\n";
		auto it = kd.get_iterator_begin();
		while (not it.end()) {
			const auto& e = *it;
			std::cout << "    " << e.data << ' ' << e.metadata << '\n';
			++it;
		}

		it.to_begin();
		std::cout << "Full iterate (full branch):\n";
		while (not it.end()) {
			const auto& data = +it;
			std::cout << "    " << std::get<0>(data).data << ' '
					  << std::get<0>(data).metadata << " '" << std::get<1>(data)
					  << "' '" << std::get<2>(data) << "' '"
					  << std::get<3>(data) << "'\n";
			++it;
		}
	}

	{
		std::cout << "Range iterate (key-value):\n";

		auto it = kd.get_range_iterator_begin(
			[](const int v) -> bool
			{
				return 2 <= v and v <= 3;
			},
			[](const double v) -> bool
			{
				return v == 0.5;
			},
			[](const std::string& s) -> bool
			{
				return s == "b";
			}
		);

		while (not it.end()) {
			const auto& e = *it;
			std::cout << "    " << e.data << ' ' << e.metadata << '\n';
			++it;
		}

		[[maybe_unused]] const bool _ = it.to_begin();
		std::cout << "Range iterate (full branch):\n";
		while (not it.end()) {
			const auto& data = +it;
			std::cout << "    " << std::get<0>(data).data << ' '
					  << std::get<0>(data).metadata << " '" << std::get<1>(data)
					  << "' '" << std::get<2>(data) << "' '"
					  << std::get<3>(data) << "'\n";
			++it;
		}
	}

	{
		std::cout << "Internal nodes profile\n";
		std::cout << "    ";
		classtree::output_profile(kd, std::cout);
		std::cout << '\n';
	}
	{
		std::cout << "Initialize from profile\n";
		classtree::ctree<A, B, int, double, std::string> kd2;
		std::ifstream fin("../../examples/profile_1.txt");
		if (fin.is_open()) {
			size_t total_bytes;
			fin >> total_bytes;
			std::cout << "    total_bytes= " << total_bytes << '\n';
			classtree::initialize(kd2, fin);
		}
	}
}
