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

#define DOCTEST_CONFIG_IMPLEMENT

// C++ includes
#include <iostream>
#include <doctest/doctest.h>

// ctree includes
#include <ctree/ctree.hpp>
#include <ctree/iterator.hpp>
#include <ctree/range_iterator.hpp>

// custom includes
#include "test_definitions.hpp"

TEST_CASE("Unique elements -- depth 3")
{
	typedef classtree::ctree<data_eq, meta_incr, int, int, int> my_tree;
	classtree::ctree<data_eq, meta_incr, int, int, int> kd;
	static_assert(std::is_nothrow_move_constructible_v<my_tree>);
	static_assert(std::is_move_constructible_v<my_tree>);

	kd.add({.i = 1, .j = 1, .k = 1, .z = 1}, {.num_occs = 1}, 1, 1, 1);
	kd.add({.i = 1, .j = 1, .k = 1, .z = 2}, {.num_occs = 1}, 1, 1, 1);
	kd.add({.i = 1, .j = 1, .k = 1, .z = 3}, {.num_occs = 1}, 1, 1, 1);
	kd.add({.i = 1, .j = 1, .k = 1, .z = 4}, {.num_occs = 1}, 1, 1, 1);
	kd.add({.i = 1, .j = 2, .k = 1, .z = 1}, {.num_occs = 1}, 1, 2, 1);
	kd.add({.i = 1, .j = 2, .k = 2, .z = 1}, {.num_occs = 1}, 1, 2, 2);
	kd.add({.i = 1, .j = 3, .k = 5, .z = 1}, {.num_occs = 1}, 1, 3, 5);
	kd.add({.i = 1, .j = 1, .k = 1, .z = 2}, {.num_occs = 1}, 1, 1, 1); // *
	kd.add({.i = 2, .j = 2, .k = 2, .z = 1}, {.num_occs = 1}, 2, 2, 2);
	kd.add({.i = 2, .j = 2, .k = 2, .z = 2}, {.num_occs = 1}, 2, 2, 2);
	kd.add({.i = 2, .j = 2, .k = 3, .z = 2}, {.num_occs = 1}, 2, 2, 3);

	CHECK_EQ(kd.size(), 10);

	SUBCASE("Print entire tree")
	{
		static constexpr std::string_view kd_str =
			"size: 10\n"
			"keys: 2\n"
			"├── 1\n"
			"│   size: 7\n"
			"│   keys: 3\n"
			"│   ├── 1\n"
			"│   │   size: 4\n"
			"│   │   keys: 1\n"
			"│   │   └── 1\n"
			"│   │       ^ size: 4 4\n"
			"│   │       ├── (1 1 1 1) {1}\n"
			"│   │       ├── (1 1 1 2) {2}\n"
			"│   │       ├── (1 1 1 3) {1}\n"
			"│   │       └── (1 1 1 4) {1}\n"
			"│   ├── 2\n"
			"│   │   size: 2\n"
			"│   │   keys: 2\n"
			"│   │   ├── 1\n"
			"│   │   │   ^ size: 1 1\n"
			"│   │   │   └── (1 2 1 1) {1}\n"
			"│   │   └── 2\n"
			"│   │       ^ size: 1 1\n"
			"│   │       └── (1 2 2 1) {1}\n"
			"│   └── 3\n"
			"│       size: 1\n"
			"│       keys: 1\n"
			"│       └── 5\n"
			"│           ^ size: 1 1\n"
			"│           └── (1 3 5 1) {1}\n"
			"└── 2\n"
			"    size: 3\n"
			"    keys: 1\n"
			"    └── 2\n"
			"        size: 3\n"
			"        keys: 2\n"
			"        ├── 2\n"
			"        │   ^ size: 2 2\n"
			"        │   ├── (2 2 2 1) {1}\n"
			"        │   └── (2 2 2 2) {1}\n"
			"        └── 3\n"
			"            ^ size: 1 1\n"
			"            └── (2 2 3 2) {1}\n";

		const std::string print_str = print_string(kd);
		CHECK_EQ(print_str, kd_str);
	}

	SUBCASE("Iterate entire tree forward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 1 1 1) {1}\n"
														"    (1 1 1 2) {2}\n"
														"    (1 1 1 3) {1}\n"
														"    (1 1 1 4) {1}\n"
														"    (1 2 1 1) {1}\n"
														"    (1 2 2 1) {1}\n"
														"    (1 3 5 1) {1}\n"
														"    (2 2 2 1) {1}\n"
														"    (2 2 2 2) {1}\n"
														"    (2 2 3 2) {1}\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_iterator();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_iterator();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate entire tree backward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (2 2 3 2) {1}\n"
														"    (2 2 2 2) {1}\n"
														"    (2 2 2 1) {1}\n"
														"    (1 3 5 1) {1}\n"
														"    (1 2 2 1) {1}\n"
														"    (1 2 1 1) {1}\n"
														"    (1 1 1 4) {1}\n"
														"    (1 1 1 3) {1}\n"
														"    (1 1 1 2) {2}\n"
														"    (1 1 1 1) {1}\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_iterator();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_iterator();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Manual iteration")
	{
		auto it = kd.get_const_iterator();
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 2});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 3});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 2});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 2});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 2});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 3});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 4});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 2, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 2, .k = 2, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 2, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 4});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
	}

	SUBCASE("Iterate over a range forward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 1 1 1) {1}\n"
														"    (1 1 1 2) {2}\n"
														"    (1 1 1 3) {1}\n"
														"    (1 1 1 4) {1}\n"
														"    (1 2 1 1) {1}\n"
														"    (1 2 2 1) {1}\n";

		const auto f1 = [](const int v) -> bool
		{
			return v == 1;
		};
		const auto f2 = [](const int v) -> bool
		{
			return 1 <= v and v <= 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate over a range backward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 2 2 1) {1}\n"
														"    (1 2 1 1) {1}\n"
														"    (1 1 1 4) {1}\n"
														"    (1 1 1 3) {1}\n"
														"    (1 1 1 2) {2}\n"
														"    (1 1 1 1) {1}\n";

		const auto f1 = [](const int v) -> bool
		{
			return v == 1;
		};
		const auto f2 = [](const int v) -> bool
		{
			return 1 <= v and v <= 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}
}

TEST_CASE("All elements -- depth 3")
{
	typedef classtree::ctree<data_eq, meta_incr, int, int, int> my_tree;
	classtree::ctree<data_eq, meta_incr, int, int, int> kd;
	static_assert(std::is_nothrow_move_constructible_v<my_tree>);
	static_assert(std::is_move_constructible_v<my_tree>);

	kd.add<false>({.i = 1, .j = 1, .k = 1, .z = 1}, {.num_occs = 1}, 1, 1, 1);
	kd.add<false>({.i = 1, .j = 1, .k = 1, .z = 2}, {.num_occs = 1}, 1, 1, 1);
	kd.add<false>({.i = 1, .j = 1, .k = 1, .z = 3}, {.num_occs = 1}, 1, 1, 1);
	kd.add<false>({.i = 1, .j = 1, .k = 1, .z = 4}, {.num_occs = 1}, 1, 1, 1);
	kd.add<false>({.i = 1, .j = 2, .k = 1, .z = 1}, {.num_occs = 1}, 1, 2, 1);
	kd.add<false>({.i = 1, .j = 2, .k = 2, .z = 1}, {.num_occs = 1}, 1, 2, 2);
	kd.add<false>({.i = 1, .j = 3, .k = 5, .z = 1}, {.num_occs = 1}, 1, 3, 5);
	kd.add<false>(
		{.i = 1, .j = 1, .k = 1, .z = 2}, {.num_occs = 1}, 1, 1, 1
	); // *
	kd.add<false>({.i = 2, .j = 2, .k = 2, .z = 1}, {.num_occs = 1}, 2, 2, 2);
	kd.add<false>({.i = 2, .j = 2, .k = 2, .z = 2}, {.num_occs = 1}, 2, 2, 2);
	kd.add<false>({.i = 2, .j = 2, .k = 3, .z = 2}, {.num_occs = 1}, 2, 2, 3);

	CHECK_EQ(kd.size(), 11);

	SUBCASE("Print entire tree")
	{
		static constexpr std::string_view kd_str =
			"size: 11\n"
			"keys: 2\n"
			"├── 1\n"
			"│   size: 8\n"
			"│   keys: 3\n"
			"│   ├── 1\n"
			"│   │   size: 5\n"
			"│   │   keys: 1\n"
			"│   │   └── 1\n"
			"│   │       ^ size: 5 5\n"
			"│   │       ├── (1 1 1 1) {1}\n"
			"│   │       ├── (1 1 1 2) {1}\n"
			"│   │       ├── (1 1 1 3) {1}\n"
			"│   │       ├── (1 1 1 4) {1}\n"
			"│   │       └── (1 1 1 2) {1}\n"
			"│   ├── 2\n"
			"│   │   size: 2\n"
			"│   │   keys: 2\n"
			"│   │   ├── 1\n"
			"│   │   │   ^ size: 1 1\n"
			"│   │   │   └── (1 2 1 1) {1}\n"
			"│   │   └── 2\n"
			"│   │       ^ size: 1 1\n"
			"│   │       └── (1 2 2 1) {1}\n"
			"│   └── 3\n"
			"│       size: 1\n"
			"│       keys: 1\n"
			"│       └── 5\n"
			"│           ^ size: 1 1\n"
			"│           └── (1 3 5 1) {1}\n"
			"└── 2\n"
			"    size: 3\n"
			"    keys: 1\n"
			"    └── 2\n"
			"        size: 3\n"
			"        keys: 2\n"
			"        ├── 2\n"
			"        │   ^ size: 2 2\n"
			"        │   ├── (2 2 2 1) {1}\n"
			"        │   └── (2 2 2 2) {1}\n"
			"        └── 3\n"
			"            ^ size: 1 1\n"
			"            └── (2 2 3 2) {1}\n";

		const std::string print_str = print_string(kd);
		CHECK_EQ(print_str, kd_str);
	}

	SUBCASE("Iterate entire tree forward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 1 1 1) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 1 1 3) {1}\n"
														"    (1 1 1 4) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 2 1 1) {1}\n"
														"    (1 2 2 1) {1}\n"
														"    (1 3 5 1) {1}\n"
														"    (2 2 2 1) {1}\n"
														"    (2 2 2 2) {1}\n"
														"    (2 2 3 2) {1}\n";

		std::cout << "Iterate over 'All elements -- depth 3'\n";
		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_iterator();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_iterator();
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate entire tree backward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (2 2 3 2) {1}\n"
														"    (2 2 2 2) {1}\n"
														"    (2 2 2 1) {1}\n"
														"    (1 3 5 1) {1}\n"
														"    (1 2 2 1) {1}\n"
														"    (1 2 1 1) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 1 1 4) {1}\n"
														"    (1 1 1 3) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 1 1 1) {1}\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_iterator();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_iterator();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Manual iteration")
	{
		auto it = kd.get_const_iterator();
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 3});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 3});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 4});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 2, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		++it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 2, .k = 2, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 2, .k = 1, .z = 1});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
		--it;
		CHECK_EQ((*it).first, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		CHECK_EQ((*it).second, meta_incr{.num_occs = 1});
	}

	SUBCASE("Iterate over a range forward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 1 1 1) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 1 1 3) {1}\n"
														"    (1 1 1 4) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 2 1 1) {1}\n"
														"    (1 2 2 1) {1}\n";

		const auto f1 = [](const int v) -> bool
		{
			return v == 1;
		};
		const auto f2 = [](const int v) -> bool
		{
			return 1 <= v and v <= 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate over a range backward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 2 2 1) {1}\n"
														"    (1 2 1 1) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 1 1 4) {1}\n"
														"    (1 1 1 3) {1}\n"
														"    (1 1 1 2) {1}\n"
														"    (1 1 1 1) {1}\n";

		const auto f1 = [](const int v) -> bool
		{
			return v == 1;
		};
		const auto f2 = [](const int v) -> bool
		{
			return 1 <= v and v <= 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}
}

int main(int argc, char **argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	int res = context.run(); // run doctest

	// important - query flags (and --exit) rely on the user doing this
	if (context.shouldExit()) {
		// propagate the result of the tests
		return res;
	}
}
