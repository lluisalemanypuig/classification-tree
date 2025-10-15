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
#include <doctest/doctest.h>

// ctree includes
#include <ctree/ctree.hpp>
#include <ctree/iterator.hpp>
#include <ctree/range_iterator.hpp>

// custom includes
#include "definitions.hpp"
#include "functions.hpp"

TEST_CASE("All elements (1)")
{
	typedef classtree::ctree<data_eq, void, int, int, int> my_tree;
	classtree::ctree<data_eq, void, int, int, int> kd;
	static_assert(std::is_nothrow_move_constructible_v<my_tree>);
	static_assert(std::is_move_constructible_v<my_tree>);

	kd.add<false>({.i = 1, .j = 1, .k = 1, .z = 2}, 1, 1, 1);
	kd.add<false>({.i = 1, .j = 1, .k = 1, .z = 1}, 1, 1, 1);
	kd.add<false>({.i = 1, .j = 1, .k = 1, .z = 3}, 1, 1, 1);
	kd.add<false>({.i = 1, .j = 1, .k = 1, .z = 4}, 1, 1, 1);
	kd.add<false>({.i = 1, .j = 2, .k = 1, .z = 1}, 1, 2, 1);
	kd.add<false>({.i = 1, .j = 2, .k = 2, .z = 1}, 1, 2, 2);
	kd.add<false>({.i = 1, .j = 3, .k = 5, .z = 1}, 1, 3, 5);
	kd.add<false>({.i = 1, .j = 1, .k = 1, .z = 2}, 1, 1, 1);
	kd.add<false>({.i = 2, .j = 2, .k = 2, .z = 1}, 2, 2, 2);
	kd.add<false>({.i = 2, .j = 2, .k = 2, .z = 2}, 2, 2, 2);
	kd.add<false>({.i = 2, .j = 2, .k = 3, .z = 2}, 2, 2, 3);

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
			"│   │       ├── (1 1 1 2)\n"
			"│   │       ├── (1 1 1 1)\n"
			"│   │       ├── (1 1 1 3)\n"
			"│   │       ├── (1 1 1 4)\n"
			"│   │       └── (1 1 1 2)\n"
			"│   ├── 2\n"
			"│   │   size: 2\n"
			"│   │   keys: 2\n"
			"│   │   ├── 1\n"
			"│   │   │   ^ size: 1 1\n"
			"│   │   │   └── (1 2 1 1)\n"
			"│   │   └── 2\n"
			"│   │       ^ size: 1 1\n"
			"│   │       └── (1 2 2 1)\n"
			"│   └── 3\n"
			"│       size: 1\n"
			"│       keys: 1\n"
			"│       └── 5\n"
			"│           ^ size: 1 1\n"
			"│           └── (1 3 5 1)\n"
			"└── 2\n"
			"    size: 3\n"
			"    keys: 1\n"
			"    └── 2\n"
			"        size: 3\n"
			"        keys: 2\n"
			"        ├── 2\n"
			"        │   ^ size: 2 2\n"
			"        │   ├── (2 2 2 1)\n"
			"        │   └── (2 2 2 2)\n"
			"        └── 3\n"
			"            ^ size: 1 1\n"
			"            └── (2 2 3 2)\n";

		const std::string print_str = print_string(kd);
		CHECK_EQ(print_str, kd_str);
	}

	SUBCASE("Iterate entire tree forward")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 1 1 2)\n"
														"    (1 1 1 1)\n"
														"    (1 1 1 3)\n"
														"    (1 1 1 4)\n"
														"    (1 1 1 2)\n"
														"    (1 2 1 1)\n"
														"    (1 2 2 1)\n"
														"    (1 3 5 1)\n"
														"    (2 2 2 1)\n"
														"    (2 2 2 2)\n"
														"    (2 2 3 2)\n";

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
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (2 2 3 2)\n"
														"    (2 2 2 2)\n"
														"    (2 2 2 1)\n"
														"    (1 3 5 1)\n"
														"    (1 2 2 1)\n"
														"    (1 2 1 1)\n"
														"    (1 1 1 2)\n"
														"    (1 1 1 4)\n"
														"    (1 1 1 3)\n"
														"    (1 1 1 1)\n"
														"    (1 1 1 2)\n";

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_iterator_end();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_iterator_end();
			return iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Manual iteration")
	{
		auto it = kd.get_const_iterator_begin();
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		
		++it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		
		++it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 3});
		
		--it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		
		--it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		
		++it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		
		--it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		
		++it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 1});
		
		++it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 3});
		
		++it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 4});
		
		++it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
		
		++it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 2, .k = 1, .z = 1});
		
		++it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 2, .k = 2, .z = 1});
		
		--it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 2, .k = 1, .z = 1});
		
		--it;
		CHECK_EQ(*it, data_eq{.i = 1, .j = 1, .k = 1, .z = 2});
	}

	SUBCASE("Iterate over a range forward (1)")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 1 1 2)\n"
														"    (1 1 1 1)\n"
														"    (1 1 1 3)\n"
														"    (1 1 1 4)\n"
														"    (1 1 1 2)\n"
														"    (1 2 1 1)\n"
														"    (1 2 2 1)\n";

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
			auto it = kd.get_const_range_iterator_begin(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator_begin(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate over a range forward (2)")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 2 1 1)\n"
														"    (1 2 2 1)\n"
														"    (2 2 2 1)\n"
														"    (2 2 2 2)\n"
														"    (2 2 3 2)\n";

		const auto f1 = [](const int) -> bool
		{
			return true;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator_begin(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator_begin(f1, f2, f3);
			return iterate_string(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate over a range backward (1)")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (1 2 2 1)\n"
														"    (1 2 1 1)\n"
														"    (1 1 1 2)\n"
														"    (1 1 1 4)\n"
														"    (1 1 1 3)\n"
														"    (1 1 1 1)\n"
														"    (1 1 1 2)\n";

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
			auto it = kd.get_const_range_iterator_end(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator_end(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Iterate over a range backward (2)")
	{
		static constexpr std::string_view kd_iter_str = "Iterate:\n"
														"    (2 2 3 2)\n"
														"    (2 2 2 2)\n"
														"    (2 2 2 1)\n"
														"    (1 2 2 1)\n"
														"    (1 2 1 1)\n";

		const auto f1 = [](const int) -> bool
		{
			return true;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		const std::string iter_str_const = [&]()
		{
			auto it = kd.get_const_range_iterator_end(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str_const);

		const std::string iter_str = [&]()
		{
			auto it = kd.get_range_iterator_end(f1, f2, f3);
			return range_iterate_string_backward(it);
		}();
		CHECK_EQ(kd_iter_str, iter_str);
	}

	SUBCASE("Count elements (1)")
	{
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

		auto it = kd.get_range_iterator(f1, f2, f3);
		const size_t c = it.count();
		CHECK_EQ(c, 7);
	}

	SUBCASE("Count elements (2)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 10;
		};
		const auto f2 = [](const int v) -> bool
		{
			return 1 <= v and v <= 2;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		auto it = kd.get_range_iterator(f1, f2, f3);
		const size_t c = it.count();
		CHECK_EQ(c, 0);
	}

	SUBCASE("Count elements (3)")
	{
		const auto f1 = [](const int) -> bool
		{
			return true;
		};
		const auto f2 = [](const int) -> bool
		{
			return true;
		};
		const auto f3 = [](const int) -> bool
		{
			return true;
		};

		auto it = kd.get_range_iterator(f1, f2, f3);
		const size_t c = it.count();
		CHECK_EQ(c, kd.size());
	}

	SUBCASE("Count elements (4)")
	{
		const auto f1 = [](const int) -> bool
		{
			return false;
		};
		const auto f2 = [](const int) -> bool
		{
			return false;
		};
		const auto f3 = [](const int) -> bool
		{
			return false;
		};

		auto it = kd.get_range_iterator(f1, f2, f3);
		const size_t c = it.count();
		CHECK_EQ(c, 0);
	}

	SUBCASE("Check iterator bounds (1)")
	{
		auto it = kd.get_const_iterator_begin();

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		for (int i = 0; i < 3; ++i) {
			--it;
			CHECK_EQ(it.past_begin(), true);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), false);

			++it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), true);
			CHECK_EQ(it.end(), false);
		}

		for (size_t i = 0; i < kd.size(); ++i) {
			CHECK_EQ(it.past_begin(), false);
			if (i > 0) {
				CHECK_EQ(it.begin(), false);
			}
			CHECK_EQ(it.end(), false);

			++it;
		}

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);

		for (int i = 0; i < 3; ++i) {
			--it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), false);

			++it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), true);
		}
	}

	SUBCASE("Check iterator bounds (2)")
	{
		auto it = kd.get_const_iterator_begin();

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		++it;

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), false);

		++it;
		++it;
		++it;
		++it;
		++it;
		++it;
		++it;
		++it;
		++it;

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), false);

		{
			const auto& d = *it;
			CHECK_EQ(d.i, 2);
			CHECK_EQ(d.j, 2);
			CHECK_EQ(d.k, 3);
			CHECK_EQ(d.z, 2);
		}

		++it;

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);

		--it;
		--it;
		--it;
		--it;
		--it;
		--it;
		--it;
		--it;
		--it;
		--it;
		--it;

		{
			const auto& d = *it;
			CHECK_EQ(d.i, 1);
			CHECK_EQ(d.j, 1);
			CHECK_EQ(d.k, 1);
			CHECK_EQ(d.z, 2);
		}

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		--it;

		CHECK_EQ(it.past_begin(), true);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), false);
	}

	SUBCASE("Check range iterator bounds (1)")
	{
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

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		for (int i = 0; i < 3; ++i) {
			--it;
			CHECK_EQ(it.past_begin(), true);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), false);

			++it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), true);
			CHECK_EQ(it.end(), false);
		}

		for (size_t i = 0; i < 7; ++i) {
			CHECK_EQ(it.past_begin(), false);
			if (i > 0) {
				CHECK_EQ(it.begin(), false);
			}
			CHECK_EQ(it.end(), false);

			++it;
		}

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);

		for (int i = 0; i < 3; ++i) {
			--it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), false);

			++it;
			CHECK_EQ(it.past_begin(), false);
			CHECK_EQ(it.begin(), false);
			CHECK_EQ(it.end(), true);
		}
	}

	SUBCASE("Check range iterator bounds (2)")
	{
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

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		++it;

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), false);

		++it;
		++it;
		++it;
		++it;
		++it;

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), false);

		{
			const auto& d = *it;
			CHECK_EQ(d.i, 1);
			CHECK_EQ(d.j, 2);
			CHECK_EQ(d.k, 2);
			CHECK_EQ(d.z, 1);
		}

		++it;

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);

		--it;
		--it;
		--it;
		--it;
		--it;
		--it;
		--it;

		{
			const auto& d = *it;
			CHECK_EQ(d.i, 1);
			CHECK_EQ(d.j, 1);
			CHECK_EQ(d.k, 1);
			CHECK_EQ(d.z, 2);
		}

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		--it;

		CHECK_EQ(it.past_begin(), true);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), false);
	}

	SUBCASE("Check range iterator bounds (3)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 4;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f3 = [](const int v) -> bool
		{
			return v == 2;
		};

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);

		CHECK_EQ(it.past_begin(), true);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);
	}

	SUBCASE("Check range iterator bounds (4)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v == 4;
		};
		const auto f3 = [](const int v) -> bool
		{
			return v == 2;
		};

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);

		CHECK_EQ(it.past_begin(), true);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);
	}

	SUBCASE("Check range iterator bounds (5)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f3 = [](const int v) -> bool
		{
			return v == 4;
		};

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);

		CHECK_EQ(it.past_begin(), true);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);
	}
}

TEST_CASE("All elements (2)")
{
	typedef classtree::ctree<data_eq, void, int, int, int> my_tree;
	classtree::ctree<data_eq, void, int, int, int> kd;
	static_assert(std::is_nothrow_move_constructible_v<my_tree>);
	static_assert(std::is_move_constructible_v<my_tree>);

	for (int i = 1; i <= 3; ++i) {
		for (int j = 1; j <= 3; ++j) {
			for (int k = 1; k <= 3; ++k) {
				kd.add<false>(
					{.i = i, .j = j, .k = k, .z = 3}, i, j, k
				);
				kd.add<false>(
					{.i = i, .j = j, .k = k, .z = 2}, i, j, k
				);
				kd.add<false>(
					{.i = i, .j = j, .k = k, .z = 1}, i, j, k
				);
			}
		}
	}

	CHECK_EQ(kd.size(), 81);

	SUBCASE("Print entire tree")
	{
		static constexpr std::string_view kd_str =
			"size: 81\n"
			"keys: 3\n"
			"├── 1\n"
			"│   size: 27\n"
			"│   keys: 3\n"
			"│   ├── 1\n"
			"│   │   size: 9\n"
			"│   │   keys: 3\n"
			"│   │   ├── 1\n"
			"│   │   │   ^ size: 3 3\n"
			"│   │   │   ├── (1 1 1 3)\n"
			"│   │   │   ├── (1 1 1 2)\n"
			"│   │   │   └── (1 1 1 1)\n"
			"│   │   ├── 2\n"
			"│   │   │   ^ size: 3 3\n"
			"│   │   │   ├── (1 1 2 3)\n"
			"│   │   │   ├── (1 1 2 2)\n"
			"│   │   │   └── (1 1 2 1)\n"
			"│   │   └── 3\n"
			"│   │       ^ size: 3 3\n"
			"│   │       ├── (1 1 3 3)\n"
			"│   │       ├── (1 1 3 2)\n"
			"│   │       └── (1 1 3 1)\n"
			"│   ├── 2\n"
			"│   │   size: 9\n"
			"│   │   keys: 3\n"
			"│   │   ├── 1\n"
			"│   │   │   ^ size: 3 3\n"
			"│   │   │   ├── (1 2 1 3)\n"
			"│   │   │   ├── (1 2 1 2)\n"
			"│   │   │   └── (1 2 1 1)\n"
			"│   │   ├── 2\n"
			"│   │   │   ^ size: 3 3\n"
			"│   │   │   ├── (1 2 2 3)\n"
			"│   │   │   ├── (1 2 2 2)\n"
			"│   │   │   └── (1 2 2 1)\n"
			"│   │   └── 3\n"
			"│   │       ^ size: 3 3\n"
			"│   │       ├── (1 2 3 3)\n"
			"│   │       ├── (1 2 3 2)\n"
			"│   │       └── (1 2 3 1)\n"
			"│   └── 3\n"
			"│       size: 9\n"
			"│       keys: 3\n"
			"│       ├── 1\n"
			"│       │   ^ size: 3 3\n"
			"│       │   ├── (1 3 1 3)\n"
			"│       │   ├── (1 3 1 2)\n"
			"│       │   └── (1 3 1 1)\n"
			"│       ├── 2\n"
			"│       │   ^ size: 3 3\n"
			"│       │   ├── (1 3 2 3)\n"
			"│       │   ├── (1 3 2 2)\n"
			"│       │   └── (1 3 2 1)\n"
			"│       └── 3\n"
			"│           ^ size: 3 3\n"
			"│           ├── (1 3 3 3)\n"
			"│           ├── (1 3 3 2)\n"
			"│           └── (1 3 3 1)\n"
			"├── 2\n"
			"│   size: 27\n"
			"│   keys: 3\n"
			"│   ├── 1\n"
			"│   │   size: 9\n"
			"│   │   keys: 3\n"
			"│   │   ├── 1\n"
			"│   │   │   ^ size: 3 3\n"
			"│   │   │   ├── (2 1 1 3)\n"
			"│   │   │   ├── (2 1 1 2)\n"
			"│   │   │   └── (2 1 1 1)\n"
			"│   │   ├── 2\n"
			"│   │   │   ^ size: 3 3\n"
			"│   │   │   ├── (2 1 2 3)\n"
			"│   │   │   ├── (2 1 2 2)\n"
			"│   │   │   └── (2 1 2 1)\n"
			"│   │   └── 3\n"
			"│   │       ^ size: 3 3\n"
			"│   │       ├── (2 1 3 3)\n"
			"│   │       ├── (2 1 3 2)\n"
			"│   │       └── (2 1 3 1)\n"
			"│   ├── 2\n"
			"│   │   size: 9\n"
			"│   │   keys: 3\n"
			"│   │   ├── 1\n"
			"│   │   │   ^ size: 3 3\n"
			"│   │   │   ├── (2 2 1 3)\n"
			"│   │   │   ├── (2 2 1 2)\n"
			"│   │   │   └── (2 2 1 1)\n"
			"│   │   ├── 2\n"
			"│   │   │   ^ size: 3 3\n"
			"│   │   │   ├── (2 2 2 3)\n"
			"│   │   │   ├── (2 2 2 2)\n"
			"│   │   │   └── (2 2 2 1)\n"
			"│   │   └── 3\n"
			"│   │       ^ size: 3 3\n"
			"│   │       ├── (2 2 3 3)\n"
			"│   │       ├── (2 2 3 2)\n"
			"│   │       └── (2 2 3 1)\n"
			"│   └── 3\n"
			"│       size: 9\n"
			"│       keys: 3\n"
			"│       ├── 1\n"
			"│       │   ^ size: 3 3\n"
			"│       │   ├── (2 3 1 3)\n"
			"│       │   ├── (2 3 1 2)\n"
			"│       │   └── (2 3 1 1)\n"
			"│       ├── 2\n"
			"│       │   ^ size: 3 3\n"
			"│       │   ├── (2 3 2 3)\n"
			"│       │   ├── (2 3 2 2)\n"
			"│       │   └── (2 3 2 1)\n"
			"│       └── 3\n"
			"│           ^ size: 3 3\n"
			"│           ├── (2 3 3 3)\n"
			"│           ├── (2 3 3 2)\n"
			"│           └── (2 3 3 1)\n"
			"└── 3\n"
			"    size: 27\n"
			"    keys: 3\n"
			"    ├── 1\n"
			"    │   size: 9\n"
			"    │   keys: 3\n"
			"    │   ├── 1\n"
			"    │   │   ^ size: 3 3\n"
			"    │   │   ├── (3 1 1 3)\n"
			"    │   │   ├── (3 1 1 2)\n"
			"    │   │   └── (3 1 1 1)\n"
			"    │   ├── 2\n"
			"    │   │   ^ size: 3 3\n"
			"    │   │   ├── (3 1 2 3)\n"
			"    │   │   ├── (3 1 2 2)\n"
			"    │   │   └── (3 1 2 1)\n"
			"    │   └── 3\n"
			"    │       ^ size: 3 3\n"
			"    │       ├── (3 1 3 3)\n"
			"    │       ├── (3 1 3 2)\n"
			"    │       └── (3 1 3 1)\n"
			"    ├── 2\n"
			"    │   size: 9\n"
			"    │   keys: 3\n"
			"    │   ├── 1\n"
			"    │   │   ^ size: 3 3\n"
			"    │   │   ├── (3 2 1 3)\n"
			"    │   │   ├── (3 2 1 2)\n"
			"    │   │   └── (3 2 1 1)\n"
			"    │   ├── 2\n"
			"    │   │   ^ size: 3 3\n"
			"    │   │   ├── (3 2 2 3)\n"
			"    │   │   ├── (3 2 2 2)\n"
			"    │   │   └── (3 2 2 1)\n"
			"    │   └── 3\n"
			"    │       ^ size: 3 3\n"
			"    │       ├── (3 2 3 3)\n"
			"    │       ├── (3 2 3 2)\n"
			"    │       └── (3 2 3 1)\n"
			"    └── 3\n"
			"        size: 9\n"
			"        keys: 3\n"
			"        ├── 1\n"
			"        │   ^ size: 3 3\n"
			"        │   ├── (3 3 1 3)\n"
			"        │   ├── (3 3 1 2)\n"
			"        │   └── (3 3 1 1)\n"
			"        ├── 2\n"
			"        │   ^ size: 3 3\n"
			"        │   ├── (3 3 2 3)\n"
			"        │   ├── (3 3 2 2)\n"
			"        │   └── (3 3 2 1)\n"
			"        └── 3\n"
			"            ^ size: 3 3\n"
			"            ├── (3 3 3 3)\n"
			"            ├── (3 3 3 2)\n"
			"            └── (3 3 3 1)\n";

		const std::string print_str = print_string(kd);
		CHECK_EQ(print_str, kd_str);
	}

	SUBCASE("Check range iterator bounds (1)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f3 = [](const int v) -> bool
		{
			return v == 2;
		};

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		++it;

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), false);

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), false);

		{
			const auto& d = *it;
			CHECK_EQ(d.i, 2);
			CHECK_EQ(d.j, 2);
			CHECK_EQ(d.k, 2);
			CHECK_EQ(d.z, 2);
		}

		++it;

		{
			const auto& d = *it;
			CHECK_EQ(d.i, 2);
			CHECK_EQ(d.j, 2);
			CHECK_EQ(d.k, 2);
			CHECK_EQ(d.z, 1);
		}

		++it;

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);

		--it;
		--it;
		--it;

		{
			const auto& d = *it;
			CHECK_EQ(d.i, 2);
			CHECK_EQ(d.j, 2);
			CHECK_EQ(d.k, 2);
			CHECK_EQ(d.z, 3);
		}

		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.end(), false);

		--it;

		CHECK_EQ(it.past_begin(), true);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), false);
	}

	SUBCASE("Check range iterator bounds (2)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 4;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f3 = [](const int v) -> bool
		{
			return v == 2;
		};

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);

		CHECK_EQ(it.past_begin(), true);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);
	}

	SUBCASE("Check range iterator bounds (3)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v == 4;
		};
		const auto f3 = [](const int v) -> bool
		{
			return v == 2;
		};

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);

		CHECK_EQ(it.past_begin(), true);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);
	}

	SUBCASE("Check range iterator bounds (4)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v == 2;
		};
		const auto f3 = [](const int v) -> bool
		{
			return v == 4;
		};

		auto it = kd.get_const_range_iterator_begin(f1, f2, f3);

		CHECK_EQ(it.past_begin(), true);
		CHECK_EQ(it.begin(), false);
		CHECK_EQ(it.end(), true);
	}
}

TEST_CASE("All elements (3)")
{
	typedef classtree::ctree<data_eq, void, int, int, int> my_tree;
	classtree::ctree<data_eq, void, int, int, int> kd;
	static_assert(std::is_nothrow_move_constructible_v<my_tree>);
	static_assert(std::is_move_constructible_v<my_tree>);

	for (int z = 1; z <= 10; ++z) {
		kd.add<false>(
			{.i = z, .j = z, .k = z, .z = 1}, z, z, z
		);
	}

	SUBCASE("Check range iterator bounds (1)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v % 2 == 0;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v % 2 == 0;
		};
		const auto f3 = [](const int v) -> bool
		{
			return v % 2 == 0;
		};

		auto it = kd.get_const_range_iterator(f1, f2, f3);
		CHECK_EQ(it.count(), 5);

		[[maybe_unused]] const bool _ = it.to_begin();

		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.end(), false);

		int i = 0;

		while (not it.end()) {
			++it;
			++i;
		}
		CHECK_EQ(i, 5);

		while (not it.past_begin()) {
			--it;
			--i;
		}
		CHECK_EQ(i, -1);
	}

	SUBCASE("Check range iterator bounds (2)")
	{
		const auto f1 = [](const int v) -> bool
		{
			return v % 2 == 0;
		};
		const auto f2 = [](const int v) -> bool
		{
			return v % 2 == 0;
		};
		const auto f3 = [](const int v) -> bool
		{
			return v == 4;
		};

		auto it = kd.get_const_range_iterator(f1, f2, f3);
		CHECK_EQ(it.count(), 1);

		[[maybe_unused]] const bool _ = it.to_begin();

		CHECK_EQ(it.begin(), true);
		CHECK_EQ(it.past_begin(), false);
		CHECK_EQ(it.end(), false);

		int i = 0;

		while (not it.end()) {
			++it;
			++i;
		}
		CHECK_EQ(i, 1);

		while (not it.past_begin()) {
			--it;
			--i;
		}
		CHECK_EQ(i, -1);
	}
}

int main(int argc, char **argv)
{
	doctest::Context context;
	context.applyCommandLine(argc, argv);

	return context.run(); // run doctest
}
