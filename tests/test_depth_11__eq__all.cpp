/**
 * Tests for the Classification Tree
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
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

// ctree includes
#include <ctree/ctree.hpp>
#include <ctree/iterator.hpp>
#include <ctree/range_iterator.hpp>

// custom includes
#include "definitions_11.hpp"
#include "functions.hpp"
#define level_types int, int, int, int, int, int, int, int, int, int, int

TEST_CASE("Deep tree")
{
	typedef classtree::ctree<d11, m11, level_types> my_tree;
	classtree::ctree<d11, m11, level_types> kd;
	static_assert(std::is_nothrow_move_constructible_v<my_tree>);
	static_assert(std::is_move_constructible_v<my_tree>);

	kd.add<false>(
		{6, 6, 2, 2, 1, 0, 1, 2, 1, 1, 0},
		{.num_occs = 1},
		6, // pawns
		6,
		2, // rooks
		2,
		1, // knights
		0,
		1, // bishops
		2,
		1, // queens
		1,
		0 // turn
	);
	kd.add<false>(
		{6, 6, 2, 2, 1, 1, 1, 0, 1, 1, 1},
		{.num_occs = 1},
		6, // pawns
		6,
		2, // rooks
		2,
		1, // knights
		1,
		1, // bishops
		0,
		1, // queens
		1,
		1 // turn
	);
	kd.add<false>(
		{6, 6, 2, 2, 2, 1, 0, 0, 1, 1, 1},
		{.num_occs = 1},
		6, // pawns
		6,
		2, // rooks
		2,
		2, // knights
		1,
		0, // bishops
		0,
		1, // queens
		1,
		1 // turn
	);
	kd.add<false>(
		{7, 7, 2, 2, 1, 0, 1, 2, 1, 1, 0},
		{.num_occs = 1},
		7, // pawns
		7,
		2, // rooks
		2,
		1, // knights
		0,
		1, // bishops
		2,
		1, // queens
		1,
		0 // turn
	);
	kd.add<false>(
		{7, 7, 2, 2, 1, 1, 2, 2, 1, 1, 1},
		{.num_occs = 1},
		7, // pawns
		7,
		2, // rooks
		2,
		1, // knights
		1,
		2, // bishops
		2,
		1, // queens
		1,
		1 // turn
	);
	kd.add<false>(
		{7, 7, 2, 2, 2, 2, 2, 2, 1, 1, 0},
		{.num_occs = 1},
		7, // pawns
		7,
		2, // rooks
		2,
		2, // knights
		2,
		2, // bishops
		2,
		1, // queens
		1,
		0 // turn
	);

	SUBCASE("Print entire tree")
	{
		// clang-format off
		static constexpr std::string_view kd_str =
			"size: 6\n"
			"keys: 2\n"
			"├── 6\n"
			"│   size: 3\n"
			"│   keys: 1\n"
			"│   └── 6\n"
			"│       size: 3\n"
			"│       keys: 1\n"
			"│       └── 2\n"
			"│           size: 3\n"
			"│           keys: 1\n"
			"│           └── 2\n"
			"│               size: 3\n"
			"│               keys: 2\n"
			"│               ├── 1\n"
			"│               │   size: 2\n"
			"│               │   keys: 2\n"
			"│               │   ├── 0\n"
			"│               │   │   size: 1\n"
			"│               │   │   keys: 1\n"
			"│               │   │   └── 1\n"
			"│               │   │       size: 1\n"
			"│               │   │       keys: 1\n"
			"│               │   │       └── 2\n"
			"│               │   │           size: 1\n"
			"│               │   │           keys: 1\n"
			"│               │   │           └── 1\n"
			"│               │   │               size: 1\n"
			"│               │   │               keys: 1\n"
			"│               │   │               └── 1\n"
			"│               │   │                   size: 1\n"
			"│               │   │                   keys: 1\n"
			"│               │   │                   └── 0\n"
			"│               │   │                       ^ size: 1 1\n"
			"│               │   │                       └── 6 6 2 2 1 0 1 2 1 1 0 {1}\n"
			"│               │   └── 1\n"
			"│               │       size: 1\n"
			"│               │       keys: 1\n"
			"│               │       └── 1\n"
			"│               │           size: 1\n"
			"│               │           keys: 1\n"
			"│               │           └── 0\n"
			"│               │               size: 1\n"
			"│               │               keys: 1\n"
			"│               │               └── 1\n"
			"│               │                   size: 1\n"
			"│               │                   keys: 1\n"
			"│               │                   └── 1\n"
			"│               │                       size: 1\n"
			"│               │                       keys: 1\n"
			"│               │                       └── 1\n"
			"│               │                           ^ size: 1 1\n"
			"│               │                           └── 6 6 2 2 1 1 1 0 1 1 1 {1}\n"
			"│               └── 2\n"
			"│                   size: 1\n"
			"│                   keys: 1\n"
			"│                   └── 1\n"
			"│                       size: 1\n"
			"│                       keys: 1\n"
			"│                       └── 0\n"
			"│                           size: 1\n"
			"│                           keys: 1\n"
			"│                           └── 0\n"
			"│                               size: 1\n"
			"│                               keys: 1\n"
			"│                               └── 1\n"
			"│                                   size: 1\n"
			"│                                   keys: 1\n"
			"│                                   └── 1\n"
			"│                                       size: 1\n"
			"│                                       keys: 1\n"
			"│                                       └── 1\n"
			"│                                           ^ size: 1 1\n"
			"│                                           └── 6 6 2 2 2 1 0 0 1 1 1 {1}\n"
			"└── 7\n"
			"    size: 3\n"
			"    keys: 1\n"
			"    └── 7\n"
			"        size: 3\n"
			"        keys: 1\n"
			"        └── 2\n"
			"            size: 3\n"
			"            keys: 1\n"
			"            └── 2\n"
			"                size: 3\n"
			"                keys: 2\n"
			"                ├── 1\n"
			"                │   size: 2\n"
			"                │   keys: 2\n"
			"                │   ├── 0\n"
			"                │   │   size: 1\n"
			"                │   │   keys: 1\n"
			"                │   │   └── 1\n"
			"                │   │       size: 1\n"
			"                │   │       keys: 1\n"
			"                │   │       └── 2\n"
			"                │   │           size: 1\n"
			"                │   │           keys: 1\n"
			"                │   │           └── 1\n"
			"                │   │               size: 1\n"
			"                │   │               keys: 1\n"
			"                │   │               └── 1\n"
			"                │   │                   size: 1\n"
			"                │   │                   keys: 1\n"
			"                │   │                   └── 0\n"
			"                │   │                       ^ size: 1 1\n"
			"                │   │                       └── 7 7 2 2 1 0 1 2 1 1 0 {1}\n"
			"                │   └── 1\n"
			"                │       size: 1\n"
			"                │       keys: 1\n"
			"                │       └── 2\n"
			"                │           size: 1\n"
			"                │           keys: 1\n"
			"                │           └── 2\n"
			"                │               size: 1\n"
			"                │               keys: 1\n"
			"                │               └── 1\n"
			"                │                   size: 1\n"
			"                │                   keys: 1\n"
			"                │                   └── 1\n"
			"                │                       size: 1\n"
			"                │                       keys: 1\n"
			"                │                       └── 1\n"
			"                │                           ^ size: 1 1\n"
			"                │                           └── 7 7 2 2 1 1 2 2 1 1 1 {1}\n"
			"                └── 2\n"
			"                    size: 1\n"
			"                    keys: 1\n"
			"                    └── 2\n"
			"                        size: 1\n"
			"                        keys: 1\n"
			"                        └── 2\n"
			"                            size: 1\n"
			"                            keys: 1\n"
			"                            └── 2\n"
			"                                size: 1\n"
			"                                keys: 1\n"
			"                                └── 1\n"
			"                                    size: 1\n"
			"                                    keys: 1\n"
			"                                    └── 1\n"
			"                                        size: 1\n"
			"                                        keys: 1\n"
			"                                        └── 0\n"
			"                                            ^ size: 1 1\n"
			"                                            └── 7 7 2 2 2 2 2 2 1 1 0 {1}\n";
		// clang-format on

		const std::string print_str_const = print_string(kd);
		CHECK_EQ(print_str_const, kd_str);
	}

	SUBCASE("Iterate entire tree forward")
	{
		static constexpr std::string_view kd_iter_str =
			"Iterate:\n"
			"    6 6 2 2 1 0 1 2 1 1 0 {1}\n"
			"    6 6 2 2 1 1 1 0 1 1 1 {1}\n"
			"    6 6 2 2 2 1 0 0 1 1 1 {1}\n"
			"    7 7 2 2 1 0 1 2 1 1 0 {1}\n"
			"    7 7 2 2 1 1 2 2 1 1 1 {1}\n"
			"    7 7 2 2 2 2 2 2 1 1 0 {1}\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_iterator_begin();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_iterator_begin();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate entire tree backward")
	{
		static constexpr std::string_view kd_iter_str =
			"Iterate:\n"
			"    7 7 2 2 2 2 2 2 1 1 0 {1}\n"
			"    7 7 2 2 1 1 2 2 1 1 1 {1}\n"
			"    7 7 2 2 1 0 1 2 1 1 0 {1}\n"
			"    6 6 2 2 2 1 0 0 1 1 1 {1}\n"
			"    6 6 2 2 1 1 1 0 1 1 1 {1}\n"
			"    6 6 2 2 1 0 1 2 1 1 0 {1}\n";

		const std::string iter_str_fb_const = [&]()
		{
			auto it = kd.get_const_iterator_end();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_fb_const);

		const std::string iter_str_fb = [&]()
		{
			auto it = kd.get_iterator_end();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_fb);
	}

	int white_knights = 0;
	// clang-format off
	const auto f1 = [](const int) -> bool { return true; };
	const auto f2 = [](const int) -> bool { return true; };
	const auto f3 = [](const int) -> bool { return true; };
	const auto f4 = [](const int) -> bool { return true; };
	const auto f5 = [&](const int v) -> bool {
		white_knights = v;
		return true;
	};
	const auto f6 = [&](const int v) -> bool {
		const int black_knights = v;
		const int total = white_knights + black_knights;
		return 2 <= total and total <= 4;
	};
	const auto f7 = [](const int) -> bool { return true; };
	const auto f8 = [](const int) -> bool { return true; };
	const auto f9 = [](const int) -> bool { return true; };
	const auto f10 = [](const int) -> bool { return true; };
	const auto f11 = [](const int) -> bool { return true; };
	// clang-format on

	SUBCASE("Check range iterator bounds (2)")
	{
		auto it = kd.get_const_range_iterator(
			f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11
		);

		[[maybe_unused]] const bool _ = it.to_begin();
		CHECK_EQ((*it).first.n_white_pawns, 6);
		CHECK_EQ((*it).first.n_white_knights, 1);
		CHECK_EQ((*it).first.n_black_knights, 1);

		++it;
		CHECK_EQ((*it).first.n_white_pawns, 6);
		CHECK_EQ((*it).first.n_white_knights, 2);
		CHECK_EQ((*it).first.n_black_knights, 1);

		++it;
		CHECK_EQ((*it).first.n_white_pawns, 7);
		CHECK_EQ((*it).first.n_white_knights, 1);
		CHECK_EQ((*it).first.n_black_knights, 1);

		++it;
		CHECK_EQ((*it).first.n_white_pawns, 7);
		CHECK_EQ((*it).first.n_white_knights, 2);
		CHECK_EQ((*it).first.n_black_knights, 2);

		++it;
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.end(), true);
	}

	SUBCASE("Iterate over a range forward")
	{
		static constexpr std::string_view kd_iter_str =
			"Iterate:\n"
			"    6 6 2 2 1 1 1 0 1 1 1 {1}\n"
			"    6 6 2 2 2 1 0 0 1 1 1 {1}\n"
			"    7 7 2 2 1 1 2 2 1 1 1 {1}\n"
			"    7 7 2 2 2 2 2 2 1 1 0 {1}\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator_begin(
				f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11
			);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator_begin(
				f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11
			);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate over a range backward")
	{
		static constexpr std::string_view kd_iter_str =
			"Iterate:\n"
			"    7 7 2 2 2 2 2 2 1 1 0 {1}\n"
			"    7 7 2 2 1 1 2 2 1 1 1 {1}\n"
			"    6 6 2 2 2 1 0 0 1 1 1 {1}\n"
			"    6 6 2 2 1 1 1 0 1 1 1 {1}\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator_end(
				f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11
			);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator_end(
				f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11
			);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Count elements")
	{
		auto it =
			kd.get_range_iterator(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11);
		const std::size_t c = it.count();
		CHECK_EQ(c, 4);
	}
}

int main(int argc, char **argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	return context.run(); // run doctest
}
